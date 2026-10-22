#pragma once

#include <Volk/volk.h>

#include "Core/Utility.h"

namespace Nexora
{

class VulkanPhysicalDevice;

class VulkanDevice final : public NonCopyable
{
public:
    explicit VulkanDevice(const VulkanPhysicalDevice& physicalDevice);
    ~VulkanDevice();

    VkDevice handle() const { return m_device; }
    VkQueue graphicsQueue() const { return m_graphicsQueue; }
    VkQueue presentQueue() const { return m_presentQueue; }

private:
    VkDevice m_device{VK_NULL_HANDLE};
    VkQueue m_graphicsQueue{VK_NULL_HANDLE};
    VkQueue m_presentQueue{VK_NULL_HANDLE};
};

}  // namespace Nexora
