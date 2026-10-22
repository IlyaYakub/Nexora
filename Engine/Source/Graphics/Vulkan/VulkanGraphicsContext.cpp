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
    : m_instance({.extensions = window.requiredVulkanExtensions(), .enableValidation = EnableValidation})
{
}
