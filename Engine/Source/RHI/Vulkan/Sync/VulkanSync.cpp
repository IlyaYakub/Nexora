#include "RHI/Vulkan/Sync/VulkanSync.h"

#include "RHI/Vulkan/Command/VulkanCommandBuffer.h"
#include "RHI/Vulkan/Resource/VulkanImage.h"

#include <vulkan/vulkan.h>

namespace
{

struct SyncState
{
    VkPipelineStageFlags2 stage{VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT};
    VkAccessFlags2        access{VK_ACCESS_2_NONE};
    VkImageLayout         layout{VK_IMAGE_LAYOUT_UNDEFINED};
};

SyncState toSyncState(Nexora::VulkanResourceState state)
{
    switch (state)
    {
        case Nexora::VulkanResourceState::Unknown: return {};
        case Nexora::VulkanResourceState::ColorAttachment:
            return {
                .stage  = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
                .access = VK_ACCESS_2_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
                .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            };
        case Nexora::VulkanResourceState::Present:
            return {
                .stage  = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
                .access = VK_ACCESS_2_NONE,
                .layout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
            };
    }
    return {};
}

}  // namespace

namespace Nexora
{

void VulkanSync::transitionImage(const VulkanCommandBuffer& commandBuffer, VulkanImage& image, VulkanResourceState newState)
{
    const SyncState oldSyncState = toSyncState(image.state());
    const SyncState newSyncState = toSyncState(newState);

    const VkImageMemoryBarrier2 barrier{
        .sType            = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
        .srcStageMask     = oldSyncState.stage,
        .srcAccessMask    = oldSyncState.access,
        .dstStageMask     = newSyncState.stage,
        .dstAccessMask    = newSyncState.access,
        .oldLayout        = oldSyncState.layout,
        .newLayout        = newSyncState.layout,
        .image            = image.handle(),
        .subresourceRange = {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .levelCount = 1, .layerCount = 1},
    };
    const VkDependencyInfo dependencyInfo{
        .sType                   = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .imageMemoryBarrierCount = 1,
        .pImageMemoryBarriers    = &barrier,
    };
    vkCmdPipelineBarrier2(commandBuffer.handle(), &dependencyInfo);

    image.setState(newState);
}

}  // namespace Nexora
