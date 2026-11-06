#include "GLFWWindowManager.h"

#include "GLFWWindow.h"
#include "Log/Log.h"

#include <GLFW/glfw3.h>

using namespace Nexora;

DEFINE_LOG_CATEGORY_STATIC(LogGLFWWindowManager)

std::expected<std::unique_ptr<GLFWWindowManager>, WindowError> GLFWWindowManager::create()
{
    glfwSetErrorCallback(
        [](int errorCode, const char* description) { NX_LOG(LogGLFWWindowManager, Error, "GLFW error {}: {}", errorCode, description); });

    if (!glfwInit())
    {
        glfwSetErrorCallback(nullptr);
        NX_LOG(LogGLFWWindowManager, Error, "Failed to initialize GLFW");
        return std::unexpected(WindowError::BackendInitFailed);
    }

    NX_LOG(LogGLFWWindowManager, Display, "GLFW initialized");
    return std::unique_ptr<GLFWWindowManager>(new GLFWWindowManager());
}

GLFWWindowManager::GLFWWindowManager() = default;

GLFWWindowManager::~GLFWWindowManager()
{
    // Windows must be destroyed before the library is terminated.
    m_windows.clear();
    glfwSetErrorCallback(nullptr);
    glfwTerminate();
    NX_LOG(LogGLFWWindowManager, Display, "GLFW terminated");
}

void GLFWWindowManager::update()
{
    glfwPollEvents();

    std::erase_if(m_windows,
        [](const auto& entry)
        {
            if (!entry.second->shouldClose()) return false;

            NX_LOG(LogGLFWWindowManager, Display, "Window {} closed", static_cast<uint32>(entry.first));
            return true;
        });
}

bool GLFWWindowManager::areAllWindowsClosed() const
{
    return m_windows.empty();
}

std::expected<WindowId, WindowError> GLFWWindowManager::createWindow(const WindowSettings& settings)
{
    const WindowId id{m_nextWindowId};

    auto window = GLFWWindow::create(id, settings);
    if (!window) return std::unexpected(window.error());

    ++m_nextWindowId;
    m_windows.emplace(id, std::move(*window));

    NX_LOG(LogGLFWWindowManager, Display, "Window {} created", static_cast<uint32>(id));
    return id;
}

IWindow* GLFWWindowManager::getWindow(WindowId id) const
{
    const auto it = m_windows.find(id);
    return it != m_windows.end() ? it->second.get() : nullptr;
}
