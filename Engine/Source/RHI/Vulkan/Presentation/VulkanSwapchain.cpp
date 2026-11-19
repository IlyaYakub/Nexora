#include "RHI/Vulkan/Presentation/VulkanSwapchain.h"

#include "RHI/Vulkan/Device/VulkanDevice.h"
#include "RHI/Vulkan/Device/VulkanQueue.h"
#include "RHI/Vulkan/Resource/VulkanImage.h"
#include "RHI/Vulkan/VulkanUtility.h"

#include <VkBootstrap.h>

namespace
{

VkFormat toVkFormat(Nexora::TextureFormat format)
{
    switch (format)
    {
        case Nexora::TextureFormat::Unknown: return VK_FORMAT_UNDEFINED;
        case Nexora::TextureFormat::B8G8R8A8Srgb: return VK_FORMAT_B8G8R8A8_SRGB;
    }
    return VK_FORMAT_UNDEFINED;
}

}  // namespace

namespace Nexora
{

VulkanSwapchain::VulkanSwapchain(const VulkanDevice& device, const VulkanSwapchainSettings& settings) : m_device(device)
{
    const auto physicalDevice   = m_device.physicalDevice();
    const auto logicalDevice    = m_device.logicalDevice();
    const auto queueFamilyIndex = m_device.graphicsQueue().familyIndex();
    const auto surface          = settings.surface;
    const auto format           = TextureFormat::B8G8R8A8Srgb;

    checkPresentSupport(surface);

    // create ->
    const VkSurfaceFormatKHR surfaceFormat{
        .format     = toVkFormat(format),
        .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
    };

    vkb::SwapchainBuilder builder{physicalDevice, logicalDevice, surface, queueFamilyIndex, queueFamilyIndex};
    builder
        .set_desired_format(surfaceFormat)                                                              //
        .set_desired_extent(static_cast<uint32>(settings.width), static_cast<uint32>(settings.height))  //
        .set_image_usage_flags(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT)   //
        .set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)                                             //
        .set_old_swapchain(settings.oldSwapchain);

    auto swapchain = builder.build();
    if (!swapchain)
    {
        NX_LOG(LogVulkan, Fatal, "Failed to create swapchain: {}", swapchain.error().message());
    }

    m_swapchain = swapchain->swapchain;
    m_width     = static_cast<int32>(swapchain->extent.width);
    m_height    = static_cast<int32>(swapchain->extent.height);

    // collect images ->
    const auto images     = swapchain->get_images().value();
    const auto imageViews = swapchain->get_image_views().value();
    for (uint32 i = 0; i < images.size(); ++i)
    {
        const VulkanImageSettings imageSettings{
            .image  = images[i],
            .view   = imageViews[i],
            .width  = swapchain->extent.width,
            .height = swapchain->extent.height,
            .format = format,
            .type   = VulkanImageType::Swapchain,
        };
        m_images.push_back(std::make_unique<VulkanImage>(m_device, imageSettings));
    }
}

VulkanSwapchain::~VulkanSwapchain()
{
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
