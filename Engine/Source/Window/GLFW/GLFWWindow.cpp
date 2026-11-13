#include "Window/GLFW/GLFWWindow.h"

#include "Log/Log.h"
#include "Window/GLFW/GLFWKeyMapper.h"

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

DEFINE_LOG_CATEGORY_STATIC(LogWindow);

namespace Nexora
{

GLFWWindow::GLFWWindow(WindowId id, const WindowSettings& settings) : m_id(id)
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    // create ->
    m_window = glfwCreateWindow(settings.width, settings.height, settings.title.c_str(), nullptr, nullptr);
    if (!m_window)
    {
        NX_LOG(LogWindow, Fatal, "Failed to create window");
    }

    // bind input callbacks ->
    glfwSetWindowUserPointer(m_window, this);

    glfwSetKeyCallback(m_window,                                                                                   //
        [](GLFWwindow* window, int32 key, int32, int32 action, int32)                                              //
        {                                                                                                          //
            auto*              self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));                  //
            const KeyEventData data{.key = GLFWKeyMapper::toKey(key), .action = GLFWKeyMapper::toAction(action)};  //
            const InputEvent   event{.windowId = self->m_id, .data = data};                                        //
            self->m_windowEvent.invoke(event);                                                                     //
        });

    glfwSetMouseButtonCallback(m_window,                                                                                                 //
        [](GLFWwindow* window, int32 button, int32 action, int32)                                                                        //
        {                                                                                                                                //
            auto*                      self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));                                //
            const MouseButtonEventData data{.button = GLFWKeyMapper::toMouseButton(button), .action = GLFWKeyMapper::toAction(action)};  //
            const InputEvent           event{.windowId = self->m_id, .data = data};                                                      //
            self->m_windowEvent.invoke(event);                                                                                           //
        });

    glfwSetCursorPosCallback(m_window,                                                                   //
        [](GLFWwindow* window, double x, double y)                                                       //
        {                                                                                                //
            auto*                    self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));  //
            const MouseMoveEventData data{.x = x, .y = y};                                               //
            const InputEvent         event{.windowId = self->m_id, .data = data};                        //
            self->m_windowEvent.invoke(event);                                                           //
        });

    glfwSetScrollCallback(m_window,                                                                        //
        [](GLFWwindow* window, double xOffset, double yOffset)                                             //
        {                                                                                                  //
            auto*                      self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));  //
            const MouseScrollEventData data{.xOffset = xOffset, .yOffset = yOffset};                       //
            const InputEvent           event{.windowId = self->m_id, .data = data};                        //
            self->m_windowEvent.invoke(event);                                                             //
        });

    glfwSetWindowSizeCallback(m_window,                                                                     //
        [](GLFWwindow* window, int32 width, int32 height)                                                   //
        {                                                                                                   //
            auto*                       self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));  //
            const WindowResizeEventData data{.width = width, .height = height};                             //
            const InputEvent            event{.windowId = self->m_id, .data = data};                        //
            self->m_windowEvent.invoke(event);                                                              //
        });

    glfwSetWindowCloseCallback(m_window,                                                         //
        [](GLFWwindow* window)                                                                   //
        {                                                                                        //
            auto*            self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));  //
            const InputEvent event{.windowId = self->m_id, .data = WindowCloseEventData{}};      //
            self->m_windowEvent.invoke(event);                                                   //
        });
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

void* GLFWWindow::nativeHandle() const
{
    return glfwGetWin32Window(m_window);
}

Event<const InputEvent&>& GLFWWindow::windowEvent()
{
    return m_windowEvent;
}

}  // namespace Nexora
