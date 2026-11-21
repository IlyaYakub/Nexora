#include "RHI/Vulkan/Public/VulkanSwapchain.h"

#include "RHI/Vulkan/Public/VulkanDevice.h"
#include "RHI/Vulkan/Public/VulkanQueue.h"
#include "RHI/Vulkan/Public/VulkanImage.h"
#include "RHI/Vulkan/Public/VulkanMapper.h"
#include "RHI/Vulkan/Public/VulkanUtility.h"

namespace
{

VkExtent2D chooseExtent(const VkSurfaceCapabilitiesKHR& capabilities, Nexora::int32 width, Nexora::int32 height)
{
    if (capabilities.currentExtent.width != UINT32_MAX)
    {
        return capabilities.currentExtent;
    }
    return {.width = static_cast<Nexora::uint32>(width), .height = static_cast<Nexora::uint32>(height)};
}

}  // namespace

namespace Nexora
{

VulkanSwapchain::VulkanSwapchain(const VulkanDevice& device, const VulkanSwapchainSettings& settings)
    : m_device(device),  //
      m_format(settings.format)
{
    const auto physicalDevice = m_device.physicalDevice();
    const auto logicalDevice  = m_device.logicalDevice();
    const auto surface        = settings.surface;

    checkPresentSupport(surface);

    // create ->
    VkSurfaceCapabilitiesKHR capabilities{};
    NX_VULKAN_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities));
    const auto extent = chooseExtent(capabilities, settings.width, settings.height);

    const VkSwapchainCreateInfoKHR swapchainInfo{
        .sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface          = surface,
        .minImageCount    = capabilities.minImageCount + 1,
        .imageFormat      = VulkanMapper::toVkFormat(m_format),
        .imageColorSpace  = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
        .imageExtent      = extent,
        .imageArrayLayers = 1,
        .imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .preTransform     = capabilities.currentTransform,
        .compositeAlpha   = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode      = VK_PRESENT_MODE_FIFO_KHR,
        .clipped          = VK_TRUE,
        .oldSwapchain     = settings.oldSwapchain,
    };
    NX_VULKAN_CHECK(vkCreateSwapchainKHR(logicalDevice, &swapchainInfo, nullptr, &m_swapchain));

    m_width  = static_cast<int32>(extent.width);
    m_height = static_cast<int32>(extent.height);

    // collect images ->
    uint32 imageCount = 0;
    NX_VULKAN_CHECK(vkGetSwapchainImagesKHR(logicalDevice, m_swapchain, &imageCount, nullptr));
    std::vector<VkImage> images(imageCount);
    NX_VULKAN_CHECK(vkGetSwapchainImagesKHR(logicalDevice, m_swapchain, &imageCount, images.data()));

    for (const auto image : images)
    {
        const VulkanImageSettings imageSettings{
            .image  = image,
            .width  = extent.width,
            .height = extent.height,
            .format = m_format,
        };
        m_images.push_back(std::make_unique<VulkanImage>(m_device, imageSettings));
    }
}

VulkanSwapchain::~VulkanSwapchain()
{
    // destroy images and swapchain ->
    m_images.clear();
    vkDestroySwapchainKHR(m_device.logicalDevice(), m_swapchain, nullptr);
}

VkSwapchainKHR_T* VulkanSwapchain::handle() const
{
    return m_swapchain;
}

int32 VulkanSwapchain::width() const
{
    return m_width;
}

int32 VulkanSwapchain::height() const
{
    return m_height;
}

TextureFormat VulkanSwapchain::format() const
{
    return m_format;
}

const std::vector<std::unique_ptr<VulkanImage>>& VulkanSwapchain::images() const
{
    return m_images;
}

void VulkanSwapchain::checkPresentSupport(VkSurfaceKHR_T* surface) const
{
    VkBool32 isPresentSupported = VK_FALSE;
    NX_VULKAN_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(m_device.physicalDevice(),  //
        m_device.graphicsQueue().familyIndex(),                                      //
        surface,                                                                     //
        &isPresentSupported));
    if (!isPresentSupported)
    {
        NX_LOG(LogVulkan, Fatal, "Graphics queue cannot present to the window surface");
    }
}

}  // namespace Nexora
