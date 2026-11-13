#pragma once

#include "Core/Utility.h"

struct VkCommandBuffer_T;
struct VkFence_T;
struct VkSemaphore_T;

namespace Nexora
{

class VulkanRHIDevice;

class VulkanFrame final : public NonCopyable
{
public:
    explicit VulkanFrame(const VulkanRHIDevice& device);
    ~VulkanFrame();

    // getters
    VkCommandBuffer_T* commandBuffer() const;
    VkFence_T*         inFlightFence() const;
    VkSemaphore_T*     imageAvailableSemaphore() const;

private:
    const VulkanRHIDevice& m_device;
    VkCommandBuffer_T*     m_commandBuffer{nullptr};
    VkFence_T*             m_inFlightFence{nullptr};
    VkSemaphore_T*         m_imageAvailableSemaphore{nullptr};
};

}  // namespace Nexora
