#include "Engine.h"

#include "Graphics/Vulkan/VulkanContext.h"
#include "Log/Log.h"
#include "Window/GLFW/GLFWWindowManager.h"

using namespace Nexora;

DEFINE_LOG_CATEGORY_STATIC(LogEngine)

Engine::Engine()
{
    auto windowManager = GLFWWindowManager::create();
    if (!windowManager)
    {
        NX_LOG(LogEngine, Fatal, "Failed to create the window manager");
        return;
    }
    m_windowManager = std::move(*windowManager);

    auto graphicsContext = VulkanContext::create({});
    if (!graphicsContext)
    {
        NX_LOG(LogEngine, Fatal, "Failed to create the graphics context");
        return;
    }
    m_graphicsContext = std::move(*graphicsContext);
}

void Engine::run()
{
    if (m_windowManager->areAllWindowsClosed())
    {
        NX_LOG(LogEngine, Warning, "There are no windows to run the engine with. Create a window before calling run()");
        return;
    }

    while (!m_windowManager->areAllWindowsClosed())
    {
        m_windowManager->update();
    }
}
