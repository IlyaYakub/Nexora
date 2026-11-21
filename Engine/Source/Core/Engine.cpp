#include "Core/Engine.h"

#include "Core/EngineConfig.h"
#include "Core/FrameStatistics.h"
#include "Core/Timer.h"
#include "Log/Log.h"
#include "RHI/Vulkan/Public/VulkanRHIDevice.h"
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
    const auto mainWindowId = m_windowManager->createWindow(settings);

    m_frameStatistics = std::make_unique<FrameStatistics>(*m_timer, *m_windowManager, mainWindowId);
}

Engine::~Engine() = default;

void Engine::run()
{
    while (!m_windowManager->areAllWindowsClosed())
    {
        m_timer->tick();
        m_frameStatistics->update();
        m_windowManager->update();
        m_renderer->render();
    }
}

}  // namespace Nexora
