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
    const VkPhysicalDevice physicalDevice   = m_device.physicalDevice();
    const VkDevice         logicalDevice    = m_device.device();
    const uint32           queueFamilyIndex = m_device.graphicsQueueFamilyIndex();

    checkPresentSupport(surface);

    // create ->
    vkb::SwapchainBuilder builder{physicalDevice, logicalDevice, surface, queueFamilyIndex, queueFamilyIndex};
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
    const VkDevice logicalDevice = m_device.device();

    // destroy ->
    for (VkImageView_T* imageView : m_imageViews)
    {
        vkDestroyImageView(logicalDevice, imageView, nullptr);
    }
    vkDestroySwapchainKHR(logicalDevice, m_swapchain, nullptr);
}

std::optional<uint32> VulkanSwapchain::acquireNextImage(VkSemaphore_T* signalSemaphore)
{
    uint32         imageIndex = 0;
    const VkResult result = vkAcquireNextImageKHR(m_device.device(), m_swapchain, UINT64_MAX, signalSemaphore, VK_NULL_HANDLE, &imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        return std::nullopt;
    }
    NX_VULKAN_CHECK(result);
    return imageIndex;
}

void VulkanSwapchain::present(VkQueue_T* queue, uint32 imageIndex, VkSemaphore_T* waitSemaphore)
{
    const VkPresentInfoKHR presentInfo{
        .sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores    = &waitSemaphore,
        .swapchainCount     = 1,
        .pSwapchains        = &m_swapchain,
        .pImageIndices      = &imageIndex,
    };
    const VkResult result = vkQueuePresentKHR(queue, &presentInfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        return;
    }
    NX_VULKAN_CHECK(result);
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
