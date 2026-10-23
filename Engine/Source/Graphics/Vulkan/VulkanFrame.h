#pragma once

#include <memory>

#include "Core/Utility.h"
#include "Graphics/Vulkan/VulkanCommandBuffer.h"
#include "Graphics/Vulkan/VulkanFence.h"
#include "Graphics/Vulkan/VulkanSemaphore.h"

namespace Nexora
{

class VulkanCommandPool;
class VulkanDevice;

// one frame in flight
class VulkanFrame final : public NonCopyable
{
public:
    explicit VulkanFrame(const std::shared_ptr<VulkanDevice>& device);

    const std::shared_ptr<VulkanCommandBuffer>& commandBuffer() const { return m_commandBuffer; }
    const std::shared_ptr<VulkanSemaphore>& acquireSemaphore() const { return m_acquireSemaphore; }
    const std::shared_ptr<VulkanFence>& fence() const { return m_fence; }

private:
    std::shared_ptr<VulkanCommandPool> m_commandPool;
    std::shared_ptr<VulkanCommandBuffer> m_commandBuffer;
    // image acquired
    std::shared_ptr<VulkanSemaphore> m_acquireSemaphore;
    // GPU done with frame
    std::shared_ptr<VulkanFence> m_fence;
};

}
