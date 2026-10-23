#pragma once

#include <memory>

#include <Volk/volk.h>

namespace Nexora
{

class VulkanCommandBuffer;
class VulkanImage;

class VulkanSynchronizer final
{
public:
    // barrier: image layout -> newLayout
    static void transitionImage(
        const std::shared_ptr<VulkanCommandBuffer>& commandBuffer, const std::shared_ptr<VulkanImage>& image, VkImageLayout newLayout);
};

}
