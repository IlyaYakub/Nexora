#pragma once

#include <memory>

#include <Volk/volk.h>

#include "Core/Utility.h"

namespace Nexora
{

class VulkanDevice;

class VulkanSemaphore final : public NonCopyable
{
public:
    explicit VulkanSemaphore(std::shared_ptr<VulkanDevice> device);
    ~VulkanSemaphore();

    VkSemaphore handle() const { return m_semaphore; }

private:
    std::shared_ptr<VulkanDevice> m_device;
    VkSemaphore m_semaphore{VK_NULL_HANDLE};
};

}
