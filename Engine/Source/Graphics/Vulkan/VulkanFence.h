#pragma once

#include <memory>

#include <Volk/volk.h>

#include "Core/Utility.h"

namespace Nexora
{

class VulkanDevice;

class VulkanFence final : public NonCopyable
{
public:
    explicit VulkanFence(std::shared_ptr<VulkanDevice> device, bool signaled = false);
    ~VulkanFence();

    void wait() const;
    void reset();

    VkFence handle() const { return m_fence; }

private:
    std::shared_ptr<VulkanDevice> m_device;
    VkFence m_fence{VK_NULL_HANDLE};
};

}
