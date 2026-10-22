#include "Graphics/Vulkan/VulkanSwapchain.h"

#include "Graphics/Vulkan/VulkanDevice.h"
#include "Graphics/Vulkan/VulkanPhysicalDevice.h"
#include "Graphics/Vulkan/VulkanSurface.h"

using namespace Nexora;

DEFINE_LOG_CATEGORY_STATIC(LogVulkanSwapchain)

namespace
{
VkSurfaceFormatKHR chooseSurfaceFormat(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    uint32_t count = 0;
    VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, nullptr));
    std::vector<VkSurfaceFormatKHR> formats(count);
    VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, formats.data()));

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

    return VK_PRESENT_MODE_MAILBOX_KHR;
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
}  // namespace

VulkanSwapchain::VulkanSwapchain(
    std::shared_ptr<VulkanDevice> device, std::shared_ptr<VulkanSurface> surface, VkExtent2D extent, bool vsync)
    : m_device(std::move(device)), m_surface(std::move(surface)), m_vsync(vsync)
{
    create(extent);
}

VulkanSwapchain::~VulkanSwapchain()
{
    destroyImageViews();
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

void VulkanSwapchain::create(VkExtent2D extent)
{
    const VkDevice device = m_device->handle();
    const VkPhysicalDevice physicalDevice = m_device->physicalDevice().handle();
    const VkSurfaceKHR surface = m_surface->handle();

    VkSurfaceCapabilitiesKHR capabilities;
    VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities));

    const VkSurfaceFormatKHR surfaceFormat = chooseSurfaceFormat(physicalDevice, surface);
    m_format = surfaceFormat.format;
    m_presentMode = choosePresentMode(physicalDevice, surface, m_vsync);
    m_extent = chooseExtent(capabilities, extent);

    uint32_t imageCount = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0)
    {
        imageCount = std::min(imageCount, capabilities.maxImageCount);
    }

    const VulkanQueueFamilies& families = m_device->physicalDevice().queueFamilies();
    const uint32_t familyIndices[] = {families.graphics, families.present};
    const bool concurrent = families.graphics != families.present;

    const VkSwapchainKHR oldSwapchain = m_swapchain;
    const VkSwapchainCreateInfoKHR createInfo{
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = surface,
        .minImageCount = imageCount,
        .imageFormat = surfaceFormat.format,
        .imageColorSpace = surfaceFormat.colorSpace,
        .imageExtent = m_extent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode = concurrent ? VK_SHARING_MODE_CONCURRENT : VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = concurrent ? 2u : 0u,
        .pQueueFamilyIndices = familyIndices,
        .preTransform = capabilities.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = m_presentMode,
        .clipped = VK_TRUE,
        .oldSwapchain = oldSwapchain,
    };

    destroyImageViews();
    VK_CHECK(vkCreateSwapchainKHR(device, &createInfo, nullptr, &m_swapchain));
    vkDestroySwapchainKHR(device, oldSwapchain, nullptr);

    uint32_t count = 0;
    VK_CHECK(vkGetSwapchainImagesKHR(device, m_swapchain, &count, nullptr));
    m_images.resize(count);
    VK_CHECK(vkGetSwapchainImagesKHR(device, m_swapchain, &count, m_images.data()));

    m_imageViews.resize(count);
    for (uint32_t i = 0; i < count; ++i)
    {
        const VkImageViewCreateInfo viewInfo{
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image = m_images[i],
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = m_format,
            .subresourceRange = {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .levelCount = 1, .layerCount = 1},
        };
        VK_CHECK(vkCreateImageView(device, &viewInfo, nullptr, &m_imageViews[i]));
    }

    NX_LOG(LogVulkanSwapchain, Display, "Swapchain created: {}x{}, {} images, {}", m_extent.width, m_extent.height, count,
        string_VkPresentModeKHR(m_presentMode));
}

void VulkanSwapchain::destroyImageViews()
{
    for (const VkImageView view : m_imageViews)
    {
        vkDestroyImageView(m_device->handle(), view, nullptr);
    }
    m_imageViews.clear();
}
