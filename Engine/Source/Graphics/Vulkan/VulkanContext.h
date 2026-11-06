#pragma once

#include "Core/Utility.h"
#include "Graphics/IGraphicsContext.h"

#include <volk.h>

#include <expected>
#include <memory>

namespace Nexora
{

class VulkanContext final : public IGraphicsContext, public NonCopyable
{
public:
    static std::expected<std::unique_ptr<VulkanContext>, GraphicsError> create(const GraphicsContextDesc& desc);

    ~VulkanContext() override;

    GraphicsAPI api() const override { return GraphicsAPI::Vulkan; }

private:
    VulkanContext() = default;

    std::expected<void, GraphicsError> createInstance(const GraphicsContextDesc& desc);
    std::expected<void, GraphicsError> createDebugMessenger();

    VkInstance m_instance{VK_NULL_HANDLE};
    VkDebugUtilsMessengerEXT m_debugMessenger{VK_NULL_HANDLE};
    bool m_validationEnabled{false};
};

}  // namespace Nexora
