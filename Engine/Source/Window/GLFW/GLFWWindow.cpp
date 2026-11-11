#include "Window/GLFW/GLFWWindow.h"

#include "Log/Log.h"

#include <GLFW/glfw3.h>

DEFINE_LOG_CATEGORY_STATIC(LogWindow);

namespace Nexora
{

GLFWWindow::GLFWWindow(const WindowSettings& settings)
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    // create GLFW window ->
    m_window = glfwCreateWindow(settings.width, settings.height, settings.title.c_str(), nullptr, nullptr);
    if (!m_window)
    {
        NX_LOG(LogWindow, Fatal, "Failed to create window");
    }
}

GLFWWindow::~GLFWWindow()
{
    glfwDestroyWindow(m_window);
}

bool GLFWWindow::shouldClose() const
{
    return glfwWindowShouldClose(m_window);
}

int32 GLFWWindow::width() const
{
    int32 width = 0;
    glfwGetWindowSize(m_window, &width, nullptr);
    return width;
}

int32 GLFWWindow::height() const
{
    int32 height = 0;
    glfwGetWindowSize(m_window, nullptr, &height);
    return height;
}

Event<const InputEvent&>& GLFWWindow::windowEvent()
{
    return m_windowEvent;
}

}  // namespace Nexora
