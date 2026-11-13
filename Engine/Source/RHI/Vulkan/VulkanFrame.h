#pragma once

#include "Core/Utility.h"
#include "RHI/Vulkan/VulkanFence.h"
#include "RHI/Vulkan/VulkanSemaphore.h"

struct VkCommandBuffer_T;

namespace Nexora
{

class VulkanRHIDevice;

class VulkanFrame final : public NonCopyable
{
public:
    explicit VulkanFrame(const VulkanRHIDevice& device);
    ~VulkanFrame();

    // getters
    VkCommandBuffer_T*     commandBuffer() const;
    const VulkanFence&     inFlightFence() const;
    const VulkanSemaphore& imageAvailableSemaphore() const;

private:
    const VulkanRHIDevice& m_device;
    VkCommandBuffer_T*     m_commandBuffer{nullptr};
    VulkanFence            m_inFlightFence;
    VulkanSemaphore        m_imageAvailableSemaphore;
};

}  // namespace Nexora
