#include "Core/Engine.h"

#include "Core/EngineConfig.h"
#include "Core/Timer.h"
#include "Log/Log.h"
#include "RHI/Vulkan/VulkanRHIDevice.h"
#include "Renderer/Renderer.h"
#include "Window/GLFW/GLFWWindowManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogEngine);

namespace Nexora
{

Engine::Engine()
    : m_windowManager(std::make_unique<GLFWWindowManager>()),  //
      m_timer(std::make_unique<Timer>()),                      //
      m_rhiDevice(std::make_unique<VulkanRHIDevice>()),        //
      m_renderer(std::make_unique<Renderer>(*m_windowManager, *m_rhiDevice))
{
    NX_LOG(LogEngine, Display, "Nexora {}", c_engineVersion);

    // create main window ->
    const WindowSettings settings{
        .title  = std::format("Nexora {}", c_engineVersion),
        .width  = 1600,
        .height = 900,
    };
    const WindowId                 windowId = m_windowManager->createWindow(settings);
    const std::shared_ptr<IWindow> window   = m_windowManager->getWindowById(windowId);
    window->windowEvent().add(this, &Engine::onWindowEvent);
}

Engine::~Engine() = default;

void Engine::run()
{
    while (!m_windowManager->areAllWindowsClosed())
    {
        m_timer->tick();
        m_windowManager->update();
        m_renderer->render();
    }
}

void Engine::onWindowEvent(const InputEvent& event)
{
    std::visit(
        Overloaded{
            [](const KeyEventData& data)                                                                                              //
            {                                                                                                                         //
                NX_LOG(LogEngine, Display, "Key: key={}, action={}", std::to_underlying(data.key), std::to_underlying(data.action));  //
            },                                                                                                                        //

            [](const MouseButtonEventData& data)                                                                                      //
            {                                                                                                                         //
                NX_LOG(LogEngine, Display, "Mouse button: button={}, action={}", std::to_underlying(data.button),                     //
                    std::to_underlying(data.action));                                                                                 //
            },                                                                                                                        //

            [](const MouseMoveEventData& data)                                                                                        //
            {                                                                                                                         //
                NX_LOG(LogEngine, Display, "Mouse move: x={}, y={}", data.x, data.y);                                                 //
            },                                                                                                                        //

            [](const MouseScrollEventData& data)                                                                                      //
            {                                                                                                                         //
                NX_LOG(LogEngine, Display, "Mouse scroll: x={}, y={}", data.xOffset, data.yOffset);                                   //
            },                                                                                                                        //

            [](const WindowResizeEventData& data)                                                                                     //
            {                                                                                                                         //
                NX_LOG(LogEngine, Display, "Resize: {}x{}", data.width, data.height);                                                 //
            },                                                                                                                        //

            [](const WindowCloseEventData&)                                                                                           //
            {                                                                                                                         //
                NX_LOG(LogEngine, Display, "Window closed");                                                                          //
            },                                                                                                                        //
        },
        event.data);
}

}  // namespace Nexora
