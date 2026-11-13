#include "RHI/Vulkan/VulkanViewport.h"

#include "Log/Log.h"
#include "RHI/Vulkan/VulkanFrame.h"
#include "RHI/Vulkan/VulkanRHIDevice.h"
#include "RHI/Vulkan/VulkanUtility.h"

#include <VkBootstrap.h>

namespace
{

constexpr Nexora::uint32 c_framesInFlightCount = 2;

}  // namespace

namespace Nexora
{

VulkanViewport::VulkanViewport(const VulkanRHIDevice& device, void* nativeWindowHandle, int32 width, int32 height)
    : m_device(device),  //
      m_surface(device.instance(), nativeWindowHandle)
{
    // check present support ->
    VkBool32 isPresentSupported = VK_FALSE;
    NX_VULKAN_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(m_device.physicalDevice(),  //
        m_device.graphicsQueueFamilyIndex(),                                         //
        m_surface.handle(),                                                          //
        &isPresentSupported));
    if (!isPresentSupported)
    {
        NX_LOG(LogVulkan, Fatal, "Graphics queue cannot present to the window surface");
    }

    // create swapchain and frames ->
    createSwapchain(width, height);
    for (uint32 i = 0; i < c_framesInFlightCount; ++i)
    {
        m_frames.push_back(std::make_unique<VulkanFrame>(m_device));
    }
}

VulkanViewport::~VulkanViewport()
{
    destroyRenderFinishedSemaphores();
    destroyImageViews();
    vkDestroySwapchainKHR(m_device.device(), m_swapchain, nullptr);
}

void VulkanViewport::resize(int32 width, int32 height)
{
    if (width == 0 || height == 0)
    {
        return;
    }

    NX_VULKAN_CHECK(vkDeviceWaitIdle(m_device.device()));
    destroyRenderFinishedSemaphores();
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

    // create render finished semaphores ->
    const VkSemaphoreCreateInfo semaphoreInfo{
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
    };
    m_renderFinishedSemaphores.resize(m_images.size());
    for (VkSemaphore_T*& semaphore : m_renderFinishedSemaphores)
    {
        NX_VULKAN_CHECK(vkCreateSemaphore(m_device.device(), &semaphoreInfo, nullptr, &semaphore));
    }
}

void VulkanViewport::destroyImageViews()
{
    for (VkImageView_T* imageView : m_imageViews)
    {
        vkDestroyImageView(m_device.device(), imageView, nullptr);
    }
    m_imageViews.clear();
}

void VulkanViewport::destroyRenderFinishedSemaphores()
{
    for (VkSemaphore_T* semaphore : m_renderFinishedSemaphores)
    {
        vkDestroySemaphore(m_device.device(), semaphore, nullptr);
    }
    m_renderFinishedSemaphores.clear();
}

}  // namespace Nexora
