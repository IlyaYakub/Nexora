#pragma once

#include <memory>

#include <Volk/volk.h>

#include "Core/Utility.h"

namespace Nexora
{

class VulkanDevice;

class VulkanCommandPool final : public NonCopyable
{
public:
    VulkanCommandPool(std::shared_ptr<VulkanDevice> device, uint32_t queueFamily,
        VkCommandPoolCreateFlags flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    ~VulkanCommandPool();

    void reset();

    VkCommandPool handle() const { return m_commandPool; }
    const VulkanDevice& device() const { return *m_device; }

private:
    std::shared_ptr<VulkanDevice> m_device;
    VkCommandPool m_commandPool{VK_NULL_HANDLE};
};

}  // namespace Nexora
