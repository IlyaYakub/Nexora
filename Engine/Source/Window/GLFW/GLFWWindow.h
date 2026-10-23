#pragma once

#include <string>
#include <vector>

#include <Volk/volk.h>
#include <entt/signal/fwd.hpp>

#include "Core/Utility.h"

struct GLFWwindow;

namespace Nexora
{

struct WindowSettings
{
    std::string title{"Nexora Engine"};
    int width{1280};
    int height{720};
    bool visible{true};
    bool vsync{true};
};

class GLFWWindow final : public NonCopyable
{
public:
    GLFWWindow(const WindowSettings& settings, entt::dispatcher& events);
    ~GLFWWindow();

    bool isValid() const { return m_window != nullptr; }
    bool shouldClose() const;
    void close();
    void pollEvents();

    std::string title() const;
    void setTitle(const std::string& title);

    int width() const;
    int height() const;
    void setSize(int width, int height);

    std::vector<const char*> requiredVulkanExtensions() const;
    VkSurfaceKHR createVulkanSurface(VkInstance instance) const;

private:
    entt::dispatcher& m_events;
    GLFWwindow* m_window{nullptr};
    bool m_glfwInitialized{false};
};

}
