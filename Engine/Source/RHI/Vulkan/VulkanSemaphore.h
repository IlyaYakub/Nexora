#pragma once

#include "Core/Utility.h"

struct VkSemaphore_T;

namespace Nexora
{

class VulkanRHIDevice;

class VulkanSemaphore final : public NonCopyable
{
public:
    explicit VulkanSemaphore(const VulkanRHIDevice& device);
    ~VulkanSemaphore();

    // getters
    VkSemaphore_T* handle() const;

private:
    const VulkanRHIDevice& m_device;
    VkSemaphore_T*         m_semaphore{nullptr};
};

}  // namespace Nexora
