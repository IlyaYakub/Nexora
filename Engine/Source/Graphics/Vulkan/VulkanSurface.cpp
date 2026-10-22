#include "Graphics/Vulkan/VulkanSurface.h"

#include "Graphics/Vulkan/VulkanInstance.h"
#include "Window/GLFW/GLFWWindow.h"

using namespace Nexora;

VulkanSurface::VulkanSurface(std::shared_ptr<VulkanInstance> instance, const GLFWWindow& window)
    : m_instance(std::move(instance)), m_surface(window.createVulkanSurface(m_instance->handle()))
{
}

VulkanSurface::~VulkanSurface()
{
    vkDestroySurfaceKHR(m_instance->handle(), m_surface, nullptr);
}
