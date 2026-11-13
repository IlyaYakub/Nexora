#include "RHI/Vulkan/VulkanSwapchain.h"

#include "RHI/Vulkan/VulkanRHIDevice.h"
#include "RHI/Vulkan/VulkanUtility.h"

#include <VkBootstrap.h>

namespace Nexora
{

VulkanSwapchain::VulkanSwapchain(const VulkanRHIDevice& device,   //
    VkSurfaceKHR_T*                                     surface,  //
    int32                                               width,    //
    int32                                               height,   //
    VkSwapchainKHR_T*                                   oldSwapchain)
    : m_device(device)
{
    checkPresentSupport(surface);

    // create ->
    vkb::SwapchainBuilder builder{m_device.physicalDevice(),  //
        m_device.device(),                                    //
        surface,                                              //
        m_device.graphicsQueueFamilyIndex(),                  //
        m_device.graphicsQueueFamilyIndex()};
    builder
        .set_desired_extent(static_cast<uint32>(width), static_cast<uint32>(height))  //
        .set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)                           //
        .set_old_swapchain(oldSwapchain);

    auto swapchain = builder.build();
    if (!swapchain)
    {
        NX_LOG(LogVulkan, Fatal, "Failed to create swapchain: {}", swapchain.error().message());
    }

    m_swapchain  = swapchain->swapchain;
    m_images     = swapchain->get_images().value();
    m_imageViews = swapchain->get_image_views().value();
}

VulkanSwapchain::~VulkanSwapchain()
{
    // destroy ->
    for (VkImageView_T* imageView : m_imageViews)
    {
        vkDestroyImageView(m_device.device(), imageView, nullptr);
    }
    vkDestroySwapchainKHR(m_device.device(), m_swapchain, nullptr);
}

VkSwapchainKHR_T* VulkanSwapchain::handle() const
{
    return m_swapchain;
}

uint32 VulkanSwapchain::imageCount() const
{
    return static_cast<uint32>(m_images.size());
}

const std::vector<VkImage_T*>& VulkanSwapchain::images() const
{
    return m_images;
}

const std::vector<VkImageView_T*>& VulkanSwapchain::imageViews() const
{
    return m_imageViews;
}

void VulkanSwapchain::checkPresentSupport(VkSurfaceKHR_T* surface) const
{
    VkBool32 isPresentSupported = VK_FALSE;
    NX_VULKAN_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(m_device.physicalDevice(),  //
        m_device.graphicsQueueFamilyIndex(),                                         //
        surface,                                                                     //
        &isPresentSupported));
    if (!isPresentSupported)
    {
        NX_LOG(LogVulkan, Fatal, "Graphics queue cannot present to the window surface");
    }
}

}  // namespace Nexora
