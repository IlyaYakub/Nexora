#pragma once

#include "Core/Utility.h"
#include "Graphics/Vulkan/VulkanDevice.h"
#include "Graphics/Vulkan/VulkanInstance.h"
#include "Graphics/Vulkan/VulkanPhysicalDevice.h"

namespace Nexora
{

class GLFWWindow;

class VulkanGraphicsContext final : public NonCopyable
{
public:
    explicit VulkanGraphicsContext(const GLFWWindow& window);
    ~VulkanGraphicsContext();

    const VulkanInstance& instance() const { return m_instance; }
    VkSurfaceKHR surface() const { return m_surface; }
    const VulkanPhysicalDevice& physicalDevice() const { return m_physicalDevice; }
    const VulkanDevice& device() const { return m_device; }

private:
    VulkanInstance m_instance;
    VkSurfaceKHR m_surface{VK_NULL_HANDLE};
    VulkanPhysicalDevice m_physicalDevice;
    VulkanDevice m_device;
};

}  // namespace Nexora
