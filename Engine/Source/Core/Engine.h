#pragma once

#include <memory>
#include <string_view>

#include <entt/signal/dispatcher.hpp>

#include "Core/Utility.h"
#include "EngineConfig.h"
#include "Event/WindowEvents.h"
#include "Window/GLFW/GLFWWindow.h"

namespace Nexora
{

// Sets up everything the engine needs (log, events, window) right in the constructor
class Engine final : public NonCopyable
{
public:
    explicit Engine(const WindowSettings& windowSettings = {.title = "Nexora Engine v" ENGINE_VERSION_STRING});
    ~Engine();

    bool isInitialized() const { return m_window->isValid(); }

    // main loop, works until the window is closed; queued events are delivered once per frame
    void run();
    void stop();

    // subscribe: events().sink<KeyPressedEvent>().connect<&Weapon::onKeyPressed>(weapon)
    entt::dispatcher& events() { return m_events; }
    const GLFWWindow& window() const { return *m_window; }

    static constexpr std::string_view version() { return ENGINE_VERSION_STRING; }

private:
    // declared before the window, so the window is destroyed first
    entt::dispatcher m_events;
    std::unique_ptr<GLFWWindow> m_window;

    void onWindowClosed(const WindowClosedEvent& event);
    void onWindowResized(const WindowResizedEvent& event);
};

}  // namespace Nexora
