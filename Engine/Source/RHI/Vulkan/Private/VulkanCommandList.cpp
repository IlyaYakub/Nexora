#include "RHI/Vulkan/Public/VulkanCommandList.h"

#include "RHI/Vulkan/Public/VulkanCommandBuffer.h"
#include "RHI/Vulkan/Public/VulkanGraphicsPipeline.h"
#include "RHI/Vulkan/Public/VulkanImage.h"
#include "RHI/Vulkan/Public/VulkanSync.h"
#include "RHI/Vulkan/Public/VulkanMapper.h"

namespace Nexora
{

VulkanCommandList::VulkanCommandList(const VulkanCommandBuffer& commandBuffer) : m_commandBuffer(commandBuffer) {}

void VulkanCommandList::beginRenderPass(const RenderPassInfo& info)
{
    auto&       image         = static_cast<VulkanImage&>(*info.colorTarget);
    const auto  commandBuffer = m_commandBuffer.handle();
    const auto  width         = image.width();
    const auto  height        = image.height();
    const auto& color         = info.clearColor;

    VulkanSync::transitionImage(m_commandBuffer, image, VulkanResourceState::ColorAttachment);

    // begin ->
    const VkRenderingAttachmentInfo colorAttachment{
        .sType       = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
        .imageView   = image.view(),
        .imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        .loadOp      = VulkanMapper::toVkAttachmentLoadOp(info.loadAction),
        .storeOp     = VK_ATTACHMENT_STORE_OP_STORE,
        .clearValue  = {.color = {.float32 = {color.r, color.g, color.b, color.a}}},
    };
    const VkRenderingInfo renderingInfo{
        .sType                = VK_STRUCTURE_TYPE_RENDERING_INFO,
        .renderArea           = {.extent = {.width = width, .height = height}},
        .layerCount           = 1,
        .colorAttachmentCount = 1,
        .pColorAttachments    = &colorAttachment,
    };
    vkCmdBeginRendering(commandBuffer, &renderingInfo);

    // set viewport and scissor ->
    const VkViewport viewport{
        .width    = static_cast<float>(width),
        .height   = static_cast<float>(height),
        .maxDepth = 1.0f,
    };
    const VkRect2D scissor{
        .extent = {.width = width, .height = height},
    };
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void VulkanCommandList::endRenderPass()
{
    vkCmdEndRendering(m_commandBuffer.handle());
}

void VulkanCommandList::setPipeline(const IRHIGraphicsPipeline& pipeline)
{
    const auto& vulkanPipeline = static_cast<const VulkanGraphicsPipeline&>(pipeline);
    vkCmdBindPipeline(m_commandBuffer.handle(), VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanPipeline.pipeline());
}

void VulkanCommandList::draw(uint32 vertexCount)
{
    vkCmdDraw(m_commandBuffer.handle(), vertexCount, 1, 0, 0);
}

}  // namespace Nexora
