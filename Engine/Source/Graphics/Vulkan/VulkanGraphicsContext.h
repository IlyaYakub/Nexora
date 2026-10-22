#pragma once

#include "Core/Utility.h"
#include "Graphics/Vulkan/VulkanInstance.h"

namespace Nexora
{

class GLFWWindow;

class VulkanGraphicsContext final : public NonCopyable
{
public:
    explicit VulkanGraphicsContext(const GLFWWindow& window);

    const VulkanInstance& instance() const { return m_instance; }

private:
    VulkanInstance m_instance;
};

}  // namespace Nexora
