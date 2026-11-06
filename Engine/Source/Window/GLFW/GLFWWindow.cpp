#include "GLFWWindow.h"

#include "GLFWWindowCallbacks.h"
#include "Log/Log.h"

#include <GLFW/glfw3.h>

using namespace Nexora;

DEFINE_LOG_CATEGORY_STATIC(LogGLFWWindow)

std::expected<std::unique_ptr<GLFWWindow>, WindowError> GLFWWindow::create(WindowId id, const WindowSettings& settings)
{
    // Vulkan renders to the window itself, so GLFW must not create an OpenGL context.
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_VISIBLE, settings.visible ? GLFW_TRUE : GLFW_FALSE);

    GLFWwindow* handle = glfwCreateWindow(settings.width, settings.height, settings.title.c_str(), nullptr, nullptr);
    if (!handle)
    {
        NX_LOG(LogGLFWWindow, Error, "Failed to create GLFW window \"{}\"", settings.title);
        return std::unexpected(WindowError::WindowCreationFailed);
    }

    glfwSetWindowPos(handle, settings.x, settings.y);
    return std::unique_ptr<GLFWWindow>(new GLFWWindow(id, handle));
}

GLFWWindow::GLFWWindow(WindowId id, GLFWwindow* handle) : m_id(id), m_handle(handle)
{
    installWindowCallbacks(m_handle, m_events);
}

GLFWWindow::~GLFWWindow()
{
    glfwDestroyWindow(m_handle);
}

WindowId GLFWWindow::id() const
{
    return m_id;
}

void GLFWWindow::setTitle(const std::string& title)
{
    glfwSetWindowTitle(m_handle, title.c_str());
}

bool GLFWWindow::shouldClose() const
{
    return glfwWindowShouldClose(m_handle) == GLFW_TRUE;
}

void GLFWWindow::requestClose()
{
    glfwSetWindowShouldClose(m_handle, GLFW_TRUE);
}

FramebufferSize GLFWWindow::framebufferSize() const
{
    FramebufferSize size;
    glfwGetFramebufferSize(m_handle, &size.width, &size.height);
    return size;
}

WindowEvents& GLFWWindow::events()
{
    return m_events;
}
