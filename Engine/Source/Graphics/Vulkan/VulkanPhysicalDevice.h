#pragma once

#include <Volk/volk.h>

#include "Core/Utility.h"

namespace Nexora
{

struct VulkanQueueFamilies
{
    uint32_t graphics{0};
    uint32_t present{0};
};

class VulkanPhysicalDevice final : public NonCopyable
{
public:
    VulkanPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);

    VkPhysicalDevice handle() const { return m_physicalDevice; }
    const VkPhysicalDeviceProperties& properties() const { return m_properties; }
    const VulkanQueueFamilies& queueFamilies() const { return m_queueFamilies; }

private:
    VkPhysicalDevice m_physicalDevice{VK_NULL_HANDLE};
    VkPhysicalDeviceProperties m_properties{};
    VulkanQueueFamilies m_queueFamilies;
};

}  // namespace Nexora
