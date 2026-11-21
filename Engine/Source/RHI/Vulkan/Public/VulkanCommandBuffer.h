#pragma once

#include "Core/Utility.h"

struct VkCommandBuffer_T;

namespace Nexora
{

class VulkanCommandPool;
class VulkanDevice;

class VulkanCommandBuffer final : public NonCopyable
{
public:
    VulkanCommandBuffer(const VulkanDevice& device, const VulkanCommandPool& commandPool);
    ~VulkanCommandBuffer();

    // core
    void begin() const;
    void end() const;

    // getters
    VkCommandBuffer_T* handle() const;

private:
    const VulkanDevice&      m_device;
    const VulkanCommandPool& m_commandPool;
    VkCommandBuffer_T*       m_commandBuffer{nullptr};
};

}  // namespace Nexora
