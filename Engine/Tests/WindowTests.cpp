#include <entt/signal/dispatcher.hpp>
#include <gtest/gtest.h>

#include "Event/WindowEvents.h"
#include "Window/GLFW/GLFWWindow.h"

using namespace Nexora;

namespace
{
// hidden window, so tests do not flash on the screen
WindowSettings makeTestSettings()
{
    return {.title = "Test Window", .width = 640, .height = 480, .visible = false};
}

struct ResizeListener
{
    WindowResizedEvent last;
    void onResized(const WindowResizedEvent& event) { last = event; }
};
}  // namespace

TEST(GLFWWindowTest, WindowShouldApplySettings)
{
    entt::dispatcher events;
    GLFWWindow window(makeTestSettings(), events);
    ASSERT_TRUE(window.isValid());
    EXPECT_EQ(window.title(), "Test Window");
    EXPECT_EQ(window.width(), 640);
    EXPECT_FALSE(window.requiredVulkanExtensions().empty());

    window.setTitle("New Title");
    EXPECT_EQ(window.title(), "New Title");
}

TEST(GLFWWindowTest, WindowCommunicationTest)
{
    entt::dispatcher events;
    ResizeListener listener;
    events.sink<WindowResizedEvent>().connect<&ResizeListener::onResized>(listener);

    GLFWWindow window(makeTestSettings(), events);
    ASSERT_TRUE(window.isValid());

    window.setSize(800, 600);
    window.pollEvents();
    events.update();
    EXPECT_EQ(listener.last.width, 800);
    EXPECT_EQ(listener.last.height, 600);

    window.close();
    window.pollEvents();
    EXPECT_TRUE(window.shouldClose());
}

TEST(GLFWWindowTest, InvalidWindowShouldBeHandledSafely)
{
    // GLFW fails to create a window with zero size
    entt::dispatcher events;
    GLFWWindow window({.title = "Invalid", .width = 0, .height = 0, .visible = false}, events);

    EXPECT_FALSE(window.isValid());
    EXPECT_TRUE(window.shouldClose());

    window.setSize(100, 100);
    window.close();
    window.pollEvents();
}
