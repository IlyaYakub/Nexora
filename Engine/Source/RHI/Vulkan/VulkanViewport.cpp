#include "RHI/Vulkan/VulkanViewport.h"

#include "Log/Log.h"
#include "RHI/Vulkan/VulkanRHIDevice.h"

#include <VkBootstrap.h>

DEFINE_LOG_CATEGORY_STATIC(LogVulkan);

namespace Nexora
{

VulkanViewport::VulkanViewport(const VulkanRHIDevice& device, void* nativeWindowHandle, int32 width, int32 height)
    : m_device(device),  //
      m_surface(device.instance(), nativeWindowHandle)
{
    // check present support ->
    VkBool32 isPresentSupported = VK_FALSE;
    vkGetPhysicalDeviceSurfaceSupportKHR(m_device.physicalDevice(),  //
        m_device.graphicsQueueFamilyIndex(),                         //
        m_surface.handle(),                                          //
        &isPresentSupported);
    if (!isPresentSupported)
    {
        NX_LOG(LogVulkan, Fatal, "Graphics queue cannot present to the window surface");
    }

    // create swapchain ->
    createSwapchain(width, height);
}

VulkanViewport::~VulkanViewport()
{
    destroyImageViews();
    vkDestroySwapchainKHR(m_device.device(), m_swapchain, nullptr);
}

void VulkanViewport::resize(int32 width, int32 height)
{
    if (width == 0 || height == 0)
    {
        return;
    }

    vkDeviceWaitIdle(m_device.device());
    destroyImageViews();
    createSwapchain(width, height);
}

void VulkanViewport::createSwapchain(int32 width, int32 height)
{
    // build swapchain ->
    vkb::SwapchainBuilder builder{m_device.physicalDevice(),  //
        m_device.device(),                                    //
        m_surface.handle(),                                   //
        m_device.graphicsQueueFamilyIndex(),                  //
        m_device.graphicsQueueFamilyIndex()};
    builder
        .set_desired_extent(static_cast<uint32>(width), static_cast<uint32>(height))  //
        .set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)                           //
        .set_old_swapchain(m_swapchain);

    auto swapchain = builder.build();
    if (!swapchain)
    {
        NX_LOG(LogVulkan, Fatal, "Failed to create swapchain: {}", swapchain.error().message());
    }

    // replace old swapchain ->
    vkDestroySwapchainKHR(m_device.device(), m_swapchain, nullptr);
    m_swapchain  = swapchain->swapchain;
    m_images     = swapchain->get_images().value();
    m_imageViews = swapchain->get_image_views().value();
}

void VulkanViewport::destroyImageViews()
{
    for (VkImageView_T* imageView : m_imageViews)
    {
        vkDestroyImageView(m_device.device(), imageView, nullptr);
    }
    m_imageViews.clear();
}

}  // namespace Nexora
