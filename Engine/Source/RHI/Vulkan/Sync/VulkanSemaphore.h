#pragma once

#include "Core/Utility.h"

struct VkSemaphore_T;

namespace Nexora
{

class VulkanDevice;

class VulkanSemaphore final : public NonCopyable
{
public:
    explicit VulkanSemaphore(const VulkanDevice& device);
    ~VulkanSemaphore();

    // getters
    VkSemaphore_T* handle() const;

private:
    const VulkanDevice& m_device;
    VkSemaphore_T*      m_semaphore{nullptr};
};

}  // namespace Nexora
