#pragma once

#include "Core/Types.h"
#include "Core/Utility.h"

struct VkDevice_T;
struct VkQueue_T;

namespace Nexora
{

class VulkanCommandBuffer;
class VulkanFence;
class VulkanSemaphore;
class VulkanSwapchain;

class VulkanQueue final : public NonCopyable
{
public:
    VulkanQueue(VkDevice_T* device, uint32 familyIndex);

    // core
    void submit(const VulkanCommandBuffer& commandBuffer,
        const VulkanSemaphore&             waitSemaphore,
        const VulkanSemaphore&             signalSemaphore,
        const VulkanFence&                 fence) const;
    void present(const VulkanSwapchain& swapchain, uint32 imageIndex, const VulkanSemaphore& waitSemaphore) const;

    // getters
    VkQueue_T* handle() const;
    uint32     familyIndex() const;

private:
    VkQueue_T* m_queue{nullptr};
    uint32     m_familyIndex{0};
};

}  // namespace Nexora
