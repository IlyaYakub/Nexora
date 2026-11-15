#include "RHI/Vulkan/Command/VulkanCommandList.h"

#include "RHI/Vulkan/Command/VulkanCommandBuffer.h"
#include "RHI/Vulkan/Resource/VulkanImage.h"
#include "RHI/Vulkan/Sync/VulkanSync.h"

#include <vulkan/vulkan.h>

namespace Nexora
{

VulkanCommandList::VulkanCommandList(const VulkanCommandBuffer& commandBuffer) : m_commandBuffer(commandBuffer) {}

void VulkanCommandList::beginRendering(IRHITexture& texture, const std::optional<std::array<float, 4>>& clearColor)
{
    VulkanImage&               image = static_cast<VulkanImage&>(texture);
    const std::array<float, 4> color = clearColor.value_or(std::array<float, 4>{});

    VulkanSync::transitionImage(m_commandBuffer, image, VulkanResourceState::ColorAttachment);

    const VkRenderingAttachmentInfo colorAttachment{
        .sType       = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
        .imageView   = image.view(),
        .imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        .loadOp      = clearColor.has_value() ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD,
        .storeOp     = VK_ATTACHMENT_STORE_OP_STORE,
        .clearValue  = {.color = {.float32 = {color[0], color[1], color[2], color[3]}}},
    };
    const VkRenderingInfo renderingInfo{
        .sType                = VK_STRUCTURE_TYPE_RENDERING_INFO,
        .renderArea           = {.extent = {.width = image.width(), .height = image.height()}},
        .layerCount           = 1,
        .colorAttachmentCount = 1,
        .pColorAttachments    = &colorAttachment,
    };
    vkCmdBeginRendering(m_commandBuffer.handle(), &renderingInfo);
}

void VulkanCommandList::endRendering()
{
    vkCmdEndRendering(m_commandBuffer.handle());
}

}  // namespace Nexora
