#pragma once

#include <memory>

#include <Volk/volk.h>

#include "Core/Utility.h"

namespace Nexora
{

class GLFWWindow;
class VulkanInstance;

class VulkanSurface final : public NonCopyable
{
public:
    VulkanSurface(std::shared_ptr<VulkanInstance> instance, const GLFWWindow& window);
    ~VulkanSurface();

    VkSurfaceKHR handle() const { return m_surface; }

private:
    std::shared_ptr<VulkanInstance> m_instance;
    VkSurfaceKHR m_surface{VK_NULL_HANDLE};
};

}  // namespace Nexora
