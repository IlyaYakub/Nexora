#include "Graphics/Vulkan/VulkanRenderer.h"

#include "Graphics/Vulkan/VulkanDevice.h"
#include "Graphics/Vulkan/VulkanImage.h"
#include "Graphics/Vulkan/VulkanSwapchain.h"
#include "Graphics/Vulkan/VulkanSynchronizer.h"
#include "Graphics/Vulkan/VulkanUtility.h"

using namespace Nexora;

VulkanRenderer::VulkanRenderer(std::shared_ptr<VulkanDevice> device, std::shared_ptr<VulkanSwapchain> swapchain)
    : m_device(std::move(device)), m_swapchain(std::move(swapchain))
{
    // frames in flight
    for (std::shared_ptr<VulkanFrame>& frame : m_frames)
    {
        frame = std::make_shared<VulkanFrame>(m_device);
    }
}

VulkanRenderer::~VulkanRenderer()
{
    // wait GPU done before frames die
    VK_CHECK(vkDeviceWaitIdle(m_device->handle()));
}

void VulkanRenderer::drawFrame()
{
    const std::shared_ptr<VulkanFrame> frame = m_frames[m_frameIndex];

    // wait GPU done with this frame
    frame->fence()->wait();

    // acquire image index
    const std::optional<uint32_t> imageIndex = m_swapchain->acquireNextImage(frame->acquireSemaphore());
    if (!imageIndex)
    {
        m_swapchain->recreate(m_swapchain->extent());
        return;
    }

    // reset fence only after acquire succeeded
    frame->fence()->reset();

    // reset command buffer
    const std::shared_ptr<VulkanCommandBuffer> commandBuffer = frame->commandBuffer();
    commandBuffer->reset();

    // record
    commandBuffer->begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    record(commandBuffer, m_swapchain->image(*imageIndex));
    commandBuffer->end();

    // submit
    submit(commandBuffer, frame->acquireSemaphore(), m_swapchain->presentSemaphore(*imageIndex), frame->fence());

    // present
    if (!m_swapchain->present(*imageIndex))
    {
        m_swapchain->recreate(m_swapchain->extent());
    }

    // next frame
    m_frameIndex = (m_frameIndex + 1) % FramesInFlight;
}

void VulkanRenderer::submit(const std::shared_ptr<VulkanCommandBuffer>& commandBuffer, const std::shared_ptr<VulkanSemaphore>& wait,
    const std::shared_ptr<VulkanSemaphore>& signal, const std::shared_ptr<VulkanFence>& fence) const
{
    // wait image acquired before color output
    const VkSemaphoreSubmitInfo waitInfo{
        .sType     = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
        .semaphore = wait->handle(),
        .stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
    };

    // signal when all commands done
    const VkSemaphoreSubmitInfo signalInfo{
        .sType     = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
        .semaphore = signal->handle(),
        .stageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
    };

    // command buffer
    const VkCommandBufferSubmitInfo commandBufferInfo{
        .sType         = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
        .commandBuffer = commandBuffer->handle(),
    };

    // submit, fence signals CPU
    const VkSubmitInfo2 submitInfo{
        .sType                    = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
        .waitSemaphoreInfoCount   = 1,
        .pWaitSemaphoreInfos      = &waitInfo,
        .commandBufferInfoCount   = 1,
        .pCommandBufferInfos      = &commandBufferInfo,
        .signalSemaphoreInfoCount = 1,
        .pSignalSemaphoreInfos    = &signalInfo,
    };
    VK_CHECK(vkQueueSubmit2(m_device->graphicsQueue(), 1, &submitInfo, fence->handle()));
}

void VulkanRenderer::record(const std::shared_ptr<VulkanCommandBuffer>& commandBuffer, const std::shared_ptr<VulkanImage>& target)
{
    // to color attachment
    VulkanSynchronizer::transitionImage(commandBuffer, target, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

    // clear color attachment
    const VkRenderingAttachmentInfo colorAttachment{
        .sType       = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
        .imageView   = target->view(),
        .imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        .loadOp      = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp     = VK_ATTACHMENT_STORE_OP_STORE,
        .clearValue  = {.color = {{0.05f, 0.05f, 0.08f, 1.0f}}},
    };

    // dynamic rendering, no render pass
    const VkRenderingInfo renderingInfo{
        .sType                = VK_STRUCTURE_TYPE_RENDERING_INFO,
        .renderArea           = {.extent = target->extent()},
        .layerCount           = 1,
        .colorAttachmentCount = 1,
        .pColorAttachments    = &colorAttachment,
    };
    vkCmdBeginRendering(commandBuffer->handle(), &renderingInfo);
    vkCmdEndRendering(commandBuffer->handle());

    // to present
    VulkanSynchronizer::transitionImage(commandBuffer, target, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
}
