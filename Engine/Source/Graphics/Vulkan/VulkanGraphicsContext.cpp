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
}  // namespace

VulkanGraphicsContext::VulkanGraphicsContext(const GLFWWindow& window)
    : m_instance({.extensions = window.requiredVulkanExtensions(), .enableValidation = EnableValidation}),
      m_surface(window.createVulkanSurface(m_instance.handle())), m_physicalDevice(m_instance.handle(), m_surface),
      m_device(m_physicalDevice)
{
}

VulkanGraphicsContext::~VulkanGraphicsContext()
{
    vkDestroySurfaceKHR(m_instance.handle(), m_surface, nullptr);
}
