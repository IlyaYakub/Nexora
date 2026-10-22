#pragma once

#include <memory>

#include <Volk/volk.h>

#include "Core/Utility.h"

namespace Nexora
{

class VulkanCommandPool;

class VulkanCommandBuffer final : public NonCopyable
{
public:
    explicit VulkanCommandBuffer(std::shared_ptr<VulkanCommandPool> commandPool);
    ~VulkanCommandBuffer();

    void begin(VkCommandBufferUsageFlags flags = 0);
    void end();
    void reset();

    VkCommandBuffer handle() const { return m_commandBuffer; }

private:
    std::shared_ptr<VulkanCommandPool> m_commandPool;
    VkCommandBuffer m_commandBuffer{VK_NULL_HANDLE};
};

}  // namespace Nexora
