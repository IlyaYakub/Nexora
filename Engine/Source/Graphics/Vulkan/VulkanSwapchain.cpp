#include "Graphics/Vulkan/VulkanSwapchain.h"

#include "Graphics/Vulkan/VulkanDevice.h"
#include "Graphics/Vulkan/VulkanPhysicalDevice.h"
#include "Graphics/Vulkan/VulkanSurface.h"
#include "Graphics/Vulkan/VulkanUtility.h"

using namespace Nexora;

DEFINE_LOG_CATEGORY_STATIC(LogVulkanSwapchain)

namespace
{

VkSurfaceFormatKHR chooseSurfaceFormat(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    const std::vector<VkSurfaceFormatKHR> formats = vkEnumerate<VkSurfaceFormatKHR>(vkGetPhysicalDeviceSurfaceFormatsKHR, device, surface);

    const auto it = std::ranges::find_if(formats, [](const VkSurfaceFormatKHR& format)
        { return format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR; });
    return it != formats.end() ? *it : formats.front();
}

VkPresentModeKHR choosePresentMode(VkPhysicalDevice device, VkSurfaceKHR surface, bool vsync)
{
    if (vsync)
    {
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    // FIFO is the only mode that is always supported
    const std::vector<VkPresentModeKHR> modes = vkEnumerate<VkPresentModeKHR>(vkGetPhysicalDeviceSurfacePresentModesKHR, device, surface);
    for (const VkPresentModeKHR mode : {VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR})
    {
        if (std::ranges::contains(modes, mode))
        {
            return mode;
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D chooseExtent(const VkSurfaceCapabilitiesKHR& capabilities, VkExtent2D extent)
{
    if (capabilities.currentExtent.width != UINT32_MAX)
    {
        return capabilities.currentExtent;
    }

    return {
        std::clamp(extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
        std::clamp(extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height),
    };
}

uint32_t chooseImageCount(const VkSurfaceCapabilitiesKHR& capabilities)
{
    const uint32_t imageCount = capabilities.minImageCount + 1;
    return capabilities.maxImageCount > 0 ? std::min(imageCount, capabilities.maxImageCount) : imageCount;
}

}

VulkanSwapchain::VulkanSwapchain(
    std::shared_ptr<VulkanDevice> device, std::shared_ptr<VulkanSurface> surface, VkExtent2D extent, bool vsync)
    : m_device(std::move(device)), m_surface(std::move(surface)), m_vsync(vsync)
{
    create(extent);
}

VulkanSwapchain::~VulkanSwapchain()
{
    // views before swapchain
    m_images.clear();
    vkDestroySwapchainKHR(m_device->handle(), m_swapchain, nullptr);
}

void VulkanSwapchain::recreate(VkExtent2D extent)
{
    VK_CHECK(vkDeviceWaitIdle(m_device->handle()));
    create(extent);
}

void VulkanSwapchain::setVsync(bool vsync)
{
    if (m_vsync == vsync)
    {
        return;
    }

    m_vsync = vsync;
    recreate(m_extent);
}

std::optional<uint32_t> VulkanSwapchain::acquireNextImage(const std::shared_ptr<VulkanSemaphore>& acquireSemaphore)
{
    // acquire, semaphore signals when ready
    uint32_t imageIndex = 0;
    const VkResult result =
        vkAcquireNextImageKHR(m_device->handle(), m_swapchain, UINT64_MAX, acquireSemaphore->handle(), VK_NULL_HANDLE, &imageIndex);

    // out of date: recreate
    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        return std::nullopt;
    }
    // suboptimal: image still usable
    if (result != VK_SUBOPTIMAL_KHR)
    {
        VK_CHECK(result);
    }
    return imageIndex;
}

bool VulkanSwapchain::present(uint32_t imageIndex)
{
    // wait rendering done
    const VkSemaphore waitSemaphore = m_presentSemaphores[imageIndex]->handle();
    const VkPresentInfoKHR presentInfo{
        .sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores    = &waitSemaphore,
        .swapchainCount     = 1,
        .pSwapchains        = &m_swapchain,
        .pImageIndices      = &imageIndex,
    };

    // present
    const VkResult result = vkQueuePresentKHR(m_device->presentQueue(), &presentInfo);

    // needs recreate
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
    {
        return false;
    }
    VK_CHECK(result);
    return true;
}

void VulkanSwapchain::create(VkExtent2D extent)
{
    const VkDevice device                 = m_device->handle();
    const VkPhysicalDevice physicalDevice = m_device->physicalDevice()->handle();
    const VkSurfaceKHR surface            = m_surface->handle();

    // surface properties
    VkSurfaceCapabilitiesKHR capabilities;
    VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities));

    const VkSurfaceFormatKHR surfaceFormat = chooseSurfaceFormat(physicalDevice, surface);
    m_format                               = surfaceFormat.format;
    m_presentMode                          = choosePresentMode(physicalDevice, surface, m_vsync);
    m_extent                               = chooseExtent(capabilities, extent);

    // queue sharing
    const VulkanQueueFamilies& families = m_device->physicalDevice()->queueFamilies();
    const uint32_t familyIndices[]      = {families.graphics, families.present};
    const bool concurrent               = families.graphics != families.present;

    // swapchain
    const VkSwapchainKHR oldSwapchain = m_swapchain;
    const VkSwapchainCreateInfoKHR createInfo{
        .sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface               = surface,
        .minImageCount         = chooseImageCount(capabilities),
        .imageFormat           = surfaceFormat.format,
        .imageColorSpace       = surfaceFormat.colorSpace,
        .imageExtent           = m_extent,
        .imageArrayLayers      = 1,
        .imageUsage            = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode      = concurrent ? VK_SHARING_MODE_CONCURRENT : VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = concurrent ? 2u : 0u,
        .pQueueFamilyIndices   = familyIndices,
        .preTransform          = capabilities.currentTransform,
        .compositeAlpha        = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode           = m_presentMode,
        .clipped               = VK_TRUE,
        .oldSwapchain          = oldSwapchain,
    };

    // drop old views
    m_images.clear();

    // new swapchain, destroy old
    VK_CHECK(vkCreateSwapchainKHR(device, &createInfo, nullptr, &m_swapchain));
    vkDestroySwapchainKHR(device, oldSwapchain, nullptr);

    // wrap swapchain images (not owned)
    for (const VkImage image : vkEnumerate<VkImage>(vkGetSwapchainImagesKHR, device, m_swapchain))
    {
        m_images.push_back(std::make_shared<VulkanImage>(m_device, image, m_format, m_extent));
    }

    // present semaphores, old ones kept
    while (m_presentSemaphores.size() < m_images.size())
    {
        m_presentSemaphores.push_back(std::make_shared<VulkanSemaphore>(m_device));
    }

    NX_LOG(LogVulkanSwapchain, Display, "Swapchain created: {}x{}, {} images, {}", m_extent.width, m_extent.height, m_images.size(),
        string_VkPresentModeKHR(m_presentMode));
}
