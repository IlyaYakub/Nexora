#pragma once

#include <memory>

#include <Volk/volk.h>

#include "Core/Utility.h"

namespace Nexora
{

class VulkanPhysicalDevice;

class VulkanDevice final : public NonCopyable
{
public:
    explicit VulkanDevice(std::shared_ptr<VulkanPhysicalDevice> physicalDevice);
    ~VulkanDevice();

    VkDevice handle() const { return m_device; }
    const VulkanPhysicalDevice& physicalDevice() const { return *m_physicalDevice; }
    VkQueue graphicsQueue() const { return m_graphicsQueue; }
    VkQueue presentQueue() const { return m_presentQueue; }

private:
    std::shared_ptr<VulkanPhysicalDevice> m_physicalDevice;
    VkDevice m_device{VK_NULL_HANDLE};
    VkQueue m_graphicsQueue{VK_NULL_HANDLE};
    VkQueue m_presentQueue{VK_NULL_HANDLE};
};

}  // namespace Nexora
