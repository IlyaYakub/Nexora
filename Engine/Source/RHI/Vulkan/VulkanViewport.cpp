#include "RHI/Vulkan/VulkanViewport.h"

#include "RHI/Vulkan/VulkanFrame.h"
#include "RHI/Vulkan/VulkanRHIDevice.h"
#include "RHI/Vulkan/VulkanSemaphore.h"
#include "RHI/Vulkan/VulkanSwapchain.h"
#include "RHI/Vulkan/VulkanUtility.h"

#include <vulkan/vulkan.h>

namespace
{

constexpr Nexora::uint32 c_framesInFlightCount = 2;

}  // namespace

namespace Nexora
{

VulkanViewport::VulkanViewport(const VulkanRHIDevice& device,              //
    void*                                             nativeWindowHandle,  //
    int32                                             width,               //
    int32                                             height)
    : m_device(device),                                                    //
      m_surface(device.instance(), nativeWindowHandle),                    //
      m_swapchain(std::make_unique<VulkanSwapchain>(device, m_surface.handle(), width, height))
{
    createFrames();
    createRenderFinishedSemaphores();
}

VulkanViewport::~VulkanViewport()
{
    NX_VULKAN_CHECK(vkDeviceWaitIdle(m_device.device()));
}

void VulkanViewport::resize(int32 width, int32 height)
{
    if (width == 0 || height == 0)
    {
        return;
    }

    NX_VULKAN_CHECK(vkDeviceWaitIdle(m_device.device()));
    m_renderFinishedSemaphores.clear();
    m_swapchain = std::make_unique<VulkanSwapchain>(m_device, m_surface.handle(), width, height, m_swapchain->handle());
    createRenderFinishedSemaphores();
}

void VulkanViewport::beginFrame()
{
    const VulkanFrame&    frame                   = *m_frames[m_frameIndex];
    const VkCommandBuffer commandBuffer           = frame.commandBuffer();
    const VulkanFence&    inFlightFence           = frame.inFlightFence();
    const VkSemaphore     imageAvailableSemaphore = frame.imageAvailableSemaphore().handle();

    // acquire image ->
    inFlightFence.wait();

    m_imageIndex = m_swapchain->acquireNextImage(imageAvailableSemaphore);
    if (!m_imageIndex)
    {
        return;
    }
    inFlightFence.reset();

    // begin command buffer ->
    const VkCommandBufferBeginInfo beginInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
    };
    NX_VULKAN_CHECK(vkResetCommandBuffer(commandBuffer, 0));
    NX_VULKAN_CHECK(vkBeginCommandBuffer(commandBuffer, &beginInfo));
}

void VulkanViewport::endFrame()
{
    if (!m_imageIndex)
    {
        return;
    }

    const uint32          imageIndex              = *m_imageIndex;
    const VkQueue         graphicsQueue           = m_device.graphicsQueue();
    const VulkanFrame&    frame                   = *m_frames[m_frameIndex];
    const VkCommandBuffer commandBuffer           = frame.commandBuffer();
    const VkFence         inFlightFence           = frame.inFlightFence().handle();
    const VkSemaphore     imageAvailableSemaphore = frame.imageAvailableSemaphore().handle();
    const VkSemaphore     renderFinishedSemaphore = m_renderFinishedSemaphores[imageIndex]->handle();
    const VkImage         image                   = m_swapchain->images()[imageIndex];

    // transition image to present ->
    const VkImageMemoryBarrier2 barrier{
        .sType            = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
        .srcStageMask     = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
        .srcAccessMask    = VK_ACCESS_2_MEMORY_WRITE_BIT,
        .dstStageMask     = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
        .dstAccessMask    = VK_ACCESS_2_MEMORY_READ_BIT | VK_ACCESS_2_MEMORY_WRITE_BIT,
        .oldLayout        = VK_IMAGE_LAYOUT_UNDEFINED,
        .newLayout        = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
        .image            = image,
        .subresourceRange = {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .levelCount = 1, .layerCount = 1},
    };
    const VkDependencyInfo dependencyInfo{
        .sType                   = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .imageMemoryBarrierCount = 1,
        .pImageMemoryBarriers    = &barrier,
    };
    vkCmdPipelineBarrier2(commandBuffer, &dependencyInfo);
    NX_VULKAN_CHECK(vkEndCommandBuffer(commandBuffer));

    // submit ->
    const VkSemaphoreSubmitInfo waitInfo{
        .sType     = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
        .semaphore = imageAvailableSemaphore,
        .stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
    };
    const VkCommandBufferSubmitInfo commandBufferInfo{
        .sType         = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
        .commandBuffer = commandBuffer,
    };
    const VkSemaphoreSubmitInfo signalInfo{
        .sType     = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
        .semaphore = renderFinishedSemaphore,
        .stageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
    };
    const VkSubmitInfo2 submitInfo{
        .sType                    = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
        .waitSemaphoreInfoCount   = 1,
        .pWaitSemaphoreInfos      = &waitInfo,
        .commandBufferInfoCount   = 1,
        .pCommandBufferInfos      = &commandBufferInfo,
        .signalSemaphoreInfoCount = 1,
        .pSignalSemaphoreInfos    = &signalInfo,
    };
    NX_VULKAN_CHECK(vkQueueSubmit2(graphicsQueue, 1, &submitInfo, inFlightFence));

    // present ->
    m_swapchain->present(graphicsQueue, imageIndex, renderFinishedSemaphore);
    m_frameIndex = (m_frameIndex + 1) % c_framesInFlightCount;
}

void VulkanViewport::createFrames()
{
    for (uint32 i = 0; i < c_framesInFlightCount; ++i)
    {
        m_frames.push_back(std::make_unique<VulkanFrame>(m_device));
    }
}

void VulkanViewport::createRenderFinishedSemaphores()
{
    for (uint32 i = 0; i < m_swapchain->imageCount(); ++i)
    {
        m_renderFinishedSemaphores.push_back(std::make_unique<VulkanSemaphore>(m_device));
    }
}

}  // namespace Nexora
