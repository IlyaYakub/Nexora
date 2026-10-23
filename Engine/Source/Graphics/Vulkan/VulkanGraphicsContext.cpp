#include "Graphics/Vulkan/VulkanGraphicsContext.h"

#include "Window/GLFW/GLFWWindow.h"

using namespace Nexora;

namespace
{

#ifdef NDEBUG
constexpr bool EnableValidation = false;
#else
constexpr bool EnableValidation = true;
#endif

}

VulkanGraphicsContext::VulkanGraphicsContext(const GLFWWindow& window, bool vsync)
{
    // instance and GPU
    const VulkanInstanceSettings instanceSettings{
        .extensions       = window.requiredVulkanExtensions(),
        .enableValidation = EnableValidation,
    };
    m_instance       = std::make_shared<VulkanInstance>(instanceSettings);
    m_surface        = std::make_shared<VulkanSurface>(m_instance, window);
    m_physicalDevice = std::make_shared<VulkanPhysicalDevice>(m_instance, m_surface);
    m_device         = std::make_shared<VulkanDevice>(m_physicalDevice);

    // swapchain
    const VkExtent2D extent{static_cast<uint32_t>(window.width()), static_cast<uint32_t>(window.height())};
    m_swapchain = std::make_shared<VulkanSwapchain>(m_device, m_surface, extent, vsync);
}
