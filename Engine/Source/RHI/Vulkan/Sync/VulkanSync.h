#pragma once

#include "RHI/Vulkan/Sync/VulkanResourceState.h"

namespace Nexora
{

class VulkanCommandBuffer;
class VulkanImage;

class VulkanSync final
{
public:
    // core
    static void transitionImage(const VulkanCommandBuffer& commandBuffer, VulkanImage& image, VulkanResourceState newState);
};

}  // namespace Nexora
