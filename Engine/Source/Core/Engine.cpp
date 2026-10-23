#include "Core/Engine.h"

#include "Log/Log.h"

using namespace Nexora;

DEFINE_LOG_CATEGORY_STATIC(LogEngine)

Engine::Engine(const WindowSettings& windowSettings)
{
    // logger
    NX_LOG(LogEngine, Display, "Initializing Nexora Engine v{}", version());

    // event subscriptions
    m_events.sink<WindowClosedEvent>().connect<&Engine::onWindowClosed>(*this);
    m_events.sink<WindowResizedEvent>().connect<&Engine::onWindowResized>(*this);

    // window
    m_window = std::make_unique<GLFWWindow>(windowSettings, m_events);
    if (!m_window->isValid())
    {
        NX_LOG(LogEngine, Error, "Engine is not initialized: failed to create the window");
        return;
    }

    // graphics context
    m_graphicsContext = std::make_shared<VulkanGraphicsContext>(*m_window, windowSettings.vsync);

    // renderer
    m_renderer = std::make_unique<VulkanRenderer>(m_graphicsContext->device(), m_graphicsContext->swapchain());

    NX_LOG(LogEngine, Display, "Engine initialized");
}

Engine::~Engine()
{
    NX_LOG(LogEngine, Display, "Shutting down");
}

void Engine::run()
{
    if (!isInitialized())
    {
        NX_LOG(LogEngine, Error, "Cannot run: engine is not initialized");
        return;
    }

    while (!m_window->shouldClose())
    {
        m_window->pollEvents();
        m_events.update();

        m_renderer->drawFrame();
    }
}

void Engine::stop()
{
    m_window->close();
}

void Engine::onWindowClosed(const WindowClosedEvent& /*event*/)
{
    NX_LOG(LogEngine, Display, "Window closed");
}

void Engine::onWindowResized(const WindowResizedEvent& event)
{
    NX_LOG(LogEngine, Display, "Window resized: {}x{}", event.width, event.height);

    if (isInitialized() && event.width > 0 && event.height > 0)
    {
        m_graphicsContext->swapchain()->recreate({static_cast<uint32_t>(event.width), static_cast<uint32_t>(event.height)});
    }
}
