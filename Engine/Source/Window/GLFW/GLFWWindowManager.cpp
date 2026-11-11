#include "Window/GLFW/GLFWWindowManager.h"

#include "Log/Log.h"
#include "Window/GLFW/GLFWWindow.h"

#include <GLFW/glfw3.h>

DEFINE_LOG_CATEGORY_STATIC(LogWindowManager);

namespace Nexora
{

GLFWWindowManager::GLFWWindowManager()
{
    // init GLFW ->
    if (!glfwInit())
    {
        NX_LOG(LogWindowManager, Fatal, "Failed to initialize GLFW");
    }
}

GLFWWindowManager::~GLFWWindowManager()
{
    // destroy window and shutdown glfw ->
    m_windows.clear();
    glfwTerminate();
}

void GLFWWindowManager::update()
{
    glfwPollEvents();
    removeClosedWindows();
}

bool GLFWWindowManager::areAllWindowsClosed() const
{
    return m_windows.empty();
}

WindowId GLFWWindowManager::createWindow(const WindowSettings& settings)
{
    const WindowId id = m_nextWindowId;
    ++m_nextWindowId.value;
    m_windows.emplace(id, std::make_shared<GLFWWindow>(settings));
    return id;
}

std::shared_ptr<IWindow> GLFWWindowManager::getWindowById(WindowId id) const
{
    const auto it = m_windows.find(id);
    return it != m_windows.end() ? it->second : nullptr;
}

void GLFWWindowManager::removeClosedWindows()
{
    std::erase_if(m_windows, [](const auto& entry) { return entry.second->shouldClose(); });
}

}  // namespace Nexora
