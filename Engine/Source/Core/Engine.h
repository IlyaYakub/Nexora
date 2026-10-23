#pragma once

#include <memory>
#include <string_view>

#include <entt/signal/dispatcher.hpp>

#include "Core/Utility.h"
#include "EngineConfig.h"
#include "Event/WindowEvents.h"
#include "Graphics/Vulkan/VulkanGraphicsContext.h"
#include "Graphics/Vulkan/VulkanRenderer.h"
#include "Window/GLFW/GLFWWindow.h"

namespace Nexora
{

class Engine final : public NonCopyable
{
public:
    explicit Engine(const WindowSettings& windowSettings = {.title = "Nexora Engine v" ENGINE_VERSION_STRING});
    ~Engine();

    bool isInitialized() const { return m_graphicsContext != nullptr; }

    void run();
    void stop();

    entt::dispatcher& events() { return m_events; }
    const GLFWWindow& window() const { return *m_window; }
    const std::shared_ptr<VulkanGraphicsContext>& graphicsContext() const { return m_graphicsContext; }

    static constexpr std::string_view version() { return ENGINE_VERSION_STRING; }

private:
    entt::dispatcher m_events;
    std::unique_ptr<GLFWWindow> m_window;
    std::shared_ptr<VulkanGraphicsContext> m_graphicsContext;
    std::unique_ptr<VulkanRenderer> m_renderer;

    void onWindowClosed(const WindowClosedEvent& event);
    void onWindowResized(const WindowResizedEvent& event);
};

}
