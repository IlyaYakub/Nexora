#include "RHI/Vulkan/Public/VulkanViewport.h"

#include "RHI/Vulkan/Public/VulkanCommandBuffer.h"
#include "RHI/Vulkan/Public/VulkanCommandList.h"
#include "RHI/Vulkan/Public/VulkanDevice.h"
#include "RHI/Vulkan/Public/VulkanInstance.h"
#include "RHI/Vulkan/Public/VulkanQueue.h"
#include "RHI/Vulkan/Public/VulkanFrame.h"
#include "RHI/Vulkan/Public/VulkanSwapchain.h"
#include "RHI/Vulkan/Public/VulkanImage.h"
#include "RHI/Vulkan/Public/VulkanFence.h"
#include "RHI/Vulkan/Public/VulkanSemaphore.h"
#include "RHI/Vulkan/Public/VulkanSync.h"
#include "RHI/Vulkan/Public/VulkanRHIDevice.h"
#include "RHI/Vulkan/Public/VulkanUtility.h"

#include <vulkan/vulkan.h>

namespace Nexora
{

VulkanViewport::VulkanViewport(const VulkanRHIDevice& rhiDevice, const ViewportSettings& settings)
    : m_rhiDevice(rhiDevice),                                                 //
      m_surface(rhiDevice.instance().handle(), settings.nativeWindowHandle),  //
      m_presentationState(rhiDevice.device(), rhiDevice.commandPool())
{
    const auto& device  = m_rhiDevice.device();
    const auto  surface = m_surface.handle();

    // create swapchain ->
    const VulkanSwapchainSettings swapchainSettings{
        .surface = surface,
        .width   = settings.width,
        .height  = settings.height,
        .format  = settings.format,
    };
    m_presentationState.setSwapchain(std::make_unique<VulkanSwapchain>(device, swapchainSettings));
}

VulkanViewport::~VulkanViewport()
{
    NX_VULKAN_CHECK(vkDeviceWaitIdle(m_rhiDevice.device().logicalDevice()));
}

void VulkanViewport::resize(int32 width, int32 height)
{
    recreateSwapchain(width, height);
}

void VulkanViewport::beginFrame()
{
    const auto& frame                   = m_presentationState.currentFrame();
    const auto& commandBuffer           = frame.commandBuffer();
    const auto& inFlightFence           = frame.inFlightFence();
    const auto& imageAvailableSemaphore = frame.imageAvailableSemaphore();

    // acquire image ->
    inFlightFence.wait();
    m_presentationState.setImageIndex(acquireNextImage(imageAvailableSemaphore));
    inFlightFence.reset();

    commandBuffer.begin();
}

void VulkanViewport::endFrame()
{
    const auto& graphicsQueue           = m_rhiDevice.device().graphicsQueue();
    const auto& swapchain               = m_presentationState.currentSwapchain();
    const auto  imageIndex              = m_presentationState.imageIndex();
    auto&       image                   = m_presentationState.currentImage();
    const auto& renderFinishedSemaphore = m_presentationState.currentRenderFinishedSemaphore();
    const auto& frame                   = m_presentationState.currentFrame();
    const auto& commandBuffer           = frame.commandBuffer();
    const auto& inFlightFence           = frame.inFlightFence();
    const auto& imageAvailableSemaphore = frame.imageAvailableSemaphore();

    // end recording ->
    VulkanSync::transitionImage(commandBuffer, image, VulkanResourceState::Present);
    commandBuffer.end();

    // submit and present ->
    graphicsQueue.submit(commandBuffer, imageAvailableSemaphore, renderFinishedSemaphore, inFlightFence);
    graphicsQueue.present(swapchain, imageIndex, renderFinishedSemaphore);
    m_presentationState.advanceFrame();
}

IRHICommandList& VulkanViewport::commandList()
{
    return m_presentationState.currentFrame().commandList();
}

IRHITexture& VulkanViewport::backBuffer()
{
    return m_presentationState.currentImage();
}

void VulkanViewport::recreateSwapchain(int32 width, int32 height)
{
    const auto& device       = m_rhiDevice.device();
    const auto  surface      = m_surface.handle();
    const auto& oldSwapchain = m_presentationState.currentSwapchain();

    NX_VULKAN_CHECK(vkDeviceWaitIdle(device.logicalDevice()));

    // recreate swapchain ->
    const VulkanSwapchainSettings settings{
        .surface      = surface,
        .width        = width,
        .height       = height,
        .format       = oldSwapchain.format(),
        .oldSwapchain = oldSwapchain.handle(),
    };
    m_presentationState.setSwapchain(std::make_unique<VulkanSwapchain>(device, settings));
}

uint32 VulkanViewport::acquireNextImage(const VulkanSemaphore& signalSemaphore)
{
    const auto logicalDevice = m_rhiDevice.device().logicalDevice();
    const auto semaphore     = signalSemaphore.handle();

    while (true)
    {
        const auto& swapchain  = m_presentationState.currentSwapchain();
        uint32      imageIndex = 0;
        const auto  result = vkAcquireNextImageKHR(logicalDevice, swapchain.handle(), UINT64_MAX, semaphore, VK_NULL_HANDLE, &imageIndex);
        if (result != VK_ERROR_OUT_OF_DATE_KHR)
        {
            NX_VULKAN_CHECK(result);
            return imageIndex;
        }
        recreateSwapchain(swapchain.width(), swapchain.height());
    }
}

}  // namespace Nexora
