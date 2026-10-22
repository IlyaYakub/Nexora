#pragma once

#include <memory>

#include <Volk/volk.h>

#include "Core/Utility.h"

namespace Nexora
{

class VulkanInstance;
class VulkanSurface;

struct VulkanQueueFamilies
{
    uint32_t graphics{0};
    uint32_t present{0};
};

class VulkanPhysicalDevice final : public NonCopyable
{
public:
    VulkanPhysicalDevice(std::shared_ptr<VulkanInstance> instance, const VulkanSurface& surface);

    VkPhysicalDevice handle() const { return m_physicalDevice; }
    const VkPhysicalDeviceProperties& properties() const { return m_properties; }
    const VulkanQueueFamilies& queueFamilies() const { return m_queueFamilies; }

private:
    std::shared_ptr<VulkanInstance> m_instance;
    VkPhysicalDevice m_physicalDevice{VK_NULL_HANDLE};
    VkPhysicalDeviceProperties m_properties{};
    VulkanQueueFamilies m_queueFamilies;
};

}  // namespace Nexora
