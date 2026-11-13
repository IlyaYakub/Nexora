#pragma once

#include "Core/Utility.h"

struct VkFence_T;

namespace Nexora
{

class VulkanRHIDevice;

class VulkanFence final : public NonCopyable
{
public:
    explicit VulkanFence(const VulkanRHIDevice& device);
    ~VulkanFence();

    // core
    void wait() const;
    void reset() const;

    // getters
    VkFence_T* handle() const;

private:
    const VulkanRHIDevice& m_device;
    VkFence_T*             m_fence{nullptr};
};

}  // namespace Nexora
