#pragma once

#include <memory>

#include "Core/Utility.h"
#include "Graphics/Vulkan/VulkanDevice.h"
#include "Graphics/Vulkan/VulkanInstance.h"
#include "Graphics/Vulkan/VulkanPhysicalDevice.h"
#include "Graphics/Vulkan/VulkanSurface.h"
#include "Graphics/Vulkan/VulkanSwapchain.h"

namespace Nexora
{

class GLFWWindow;

class VulkanGraphicsContext final : public NonCopyable
{
public:
    VulkanGraphicsContext(const GLFWWindow& window, bool vsync);

    const std::shared_ptr<VulkanInstance>& instance() const { return m_instance; }
    const std::shared_ptr<VulkanSurface>& surface() const { return m_surface; }
    const std::shared_ptr<VulkanPhysicalDevice>& physicalDevice() const { return m_physicalDevice; }
    const std::shared_ptr<VulkanDevice>& device() const { return m_device; }
    const std::shared_ptr<VulkanSwapchain>& swapchain() const { return m_swapchain; }

private:
    std::shared_ptr<VulkanInstance> m_instance;
    std::shared_ptr<VulkanSurface> m_surface;
    std::shared_ptr<VulkanPhysicalDevice> m_physicalDevice;
    std::shared_ptr<VulkanDevice> m_device;
    std::shared_ptr<VulkanSwapchain> m_swapchain;
};

}
