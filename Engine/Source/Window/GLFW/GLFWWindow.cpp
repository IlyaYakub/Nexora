#include "Window/GLFW/GLFWWindow.h"

#include <GLFW/glfw3.h>
#include <entt/signal/dispatcher.hpp>

#include "Event/InputEvents.h"
#include "Event/WindowEvents.h"
#include "Graphics/Vulkan/VulkanUtility.h"
#include "Log/Log.h"

using namespace Nexora;

DEFINE_LOG_CATEGORY_STATIC(LogGLFWWindow)

namespace
{

entt::dispatcher& eventsOf(GLFWwindow* window)
{
    return *static_cast<entt::dispatcher*>(glfwGetWindowUserPointer(window));
}

void onError(int errorCode, const char* description)
{
    NX_LOG(LogGLFWWindow, Error, "GLFW error, code: {}, description: {}", errorCode, description);
}

void onClose(GLFWwindow* window)
{
    eventsOf(window).trigger(WindowClosedEvent{});
}

void onFramebufferResize(GLFWwindow* window, int width, int height)
{
    eventsOf(window).trigger(WindowResizedEvent{width, height});
}

void onKey(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/)
{
    if (action == GLFW_RELEASE)
    {
        eventsOf(window).trigger(KeyReleasedEvent{static_cast<Key>(key)});
        return;
    }
    eventsOf(window).trigger(KeyPressedEvent{static_cast<Key>(key), action == GLFW_REPEAT});
}

void onMouseButton(GLFWwindow* window, int button, int action, int /*mods*/)
{
    if (action == GLFW_PRESS)
    {
        eventsOf(window).trigger(MouseButtonPressedEvent{static_cast<MouseButton>(button)});
        return;
    }
    eventsOf(window).trigger(MouseButtonReleasedEvent{static_cast<MouseButton>(button)});
}

void onCursorMove(GLFWwindow* window, double x, double y)
{
    eventsOf(window).trigger(MouseMovedEvent{x, y});
}

void onScroll(GLFWwindow* window, double xOffset, double yOffset)
{
    eventsOf(window).trigger(MouseScrolledEvent{xOffset, yOffset});
}

}

GLFWWindow::GLFWWindow(const WindowSettings& settings, entt::dispatcher& events) : m_events(events)
{
    // GLFW
    glfwSetErrorCallback(onError);
    if (!glfwInit())
    {
        NX_LOG(LogGLFWWindow, Error, "Failed to initialize GLFW");
        return;
    }
    m_glfwInitialized = true;

    // window
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_VISIBLE, settings.visible ? GLFW_TRUE : GLFW_FALSE);
    m_window = glfwCreateWindow(settings.width, settings.height, settings.title.c_str(), nullptr, nullptr);
    if (!m_window)
    {
        NX_LOG(LogGLFWWindow, Error, "Failed to create GLFW window");
        return;
    }

    // events: the user pointer gives callbacks access to the events queue
    glfwSetWindowUserPointer(m_window, &m_events);
    glfwSetWindowCloseCallback(m_window, onClose);
    glfwSetFramebufferSizeCallback(m_window, onFramebufferResize);
    glfwSetKeyCallback(m_window, onKey);
    glfwSetMouseButtonCallback(m_window, onMouseButton);
    glfwSetCursorPosCallback(m_window, onCursorMove);
    glfwSetScrollCallback(m_window, onScroll);

    NX_LOG(LogGLFWWindow, Display, "Window created: '{}' {}x{}", settings.title, settings.width, settings.height);
}

GLFWWindow::~GLFWWindow()
{
    if (m_window)
    {
        glfwDestroyWindow(m_window);
    }

    if (m_glfwInitialized)
    {
        glfwTerminate();
    }
    glfwSetErrorCallback(nullptr);
}

bool GLFWWindow::shouldClose() const
{
    return !m_window || glfwWindowShouldClose(m_window);
}

void GLFWWindow::close()
{
    if (m_window)
    {
        glfwSetWindowShouldClose(m_window, GLFW_TRUE);
    }
}

void GLFWWindow::pollEvents()
{
    if (m_window)
    {
        glfwPollEvents();
    }
}

std::string GLFWWindow::title() const
{
    return m_window ? glfwGetWindowTitle(m_window) : "";
}

void GLFWWindow::setTitle(const std::string& title)
{
    if (m_window)
    {
        glfwSetWindowTitle(m_window, title.c_str());
    }
}

int GLFWWindow::width() const
{
    int width = 0;
    if (m_window)
    {
        glfwGetWindowSize(m_window, &width, nullptr);
    }
    return width;
}

int GLFWWindow::height() const
{
    int height = 0;
    if (m_window)
    {
        glfwGetWindowSize(m_window, nullptr, &height);
    }
    return height;
}

void GLFWWindow::setSize(int width, int height)
{
    if (m_window)
    {
        glfwSetWindowSize(m_window, width, height);
    }
}

std::vector<const char*> GLFWWindow::requiredVulkanExtensions() const
{
    if (!m_glfwInitialized)
    {
        return {};
    }

    uint32_t count          = 0;
    const char** extensions = glfwGetRequiredInstanceExtensions(&count);
    return extensions ? std::vector<const char*>(extensions, extensions + count) : std::vector<const char*>{};
}

VkSurfaceKHR GLFWWindow::createVulkanSurface(VkInstance instance) const
{
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    VK_CHECK(glfwCreateWindowSurface(instance, m_window, nullptr, &surface));
    return surface;
}
