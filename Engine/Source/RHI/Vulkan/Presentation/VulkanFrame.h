#pragma once

#include "Core/Utility.h"
#include "RHI/Vulkan/Command/VulkanCommandBuffer.h"
#include "RHI/Vulkan/Command/VulkanCommandList.h"
#include "RHI/Vulkan/Sync/VulkanFence.h"
#include "RHI/Vulkan/Sync/VulkanSemaphore.h"

namespace Nexora
{

class VulkanCommandPool;
class VulkanDevice;

class VulkanFrame final : public NonCopyable
{
public:
    VulkanFrame(const VulkanDevice& device, const VulkanCommandPool& commandPool);

    // getters
    const VulkanCommandBuffer& commandBuffer() const;
    VulkanCommandList&         commandList();
    const VulkanFence&         inFlightFence() const;
    const VulkanSemaphore&     imageAvailableSemaphore() const;

private:
    VulkanCommandBuffer m_commandBuffer;
    VulkanCommandList   m_commandList;
    VulkanFence         m_inFlightFence;
    VulkanSemaphore     m_imageAvailableSemaphore;
};

}  // namespace Nexora
