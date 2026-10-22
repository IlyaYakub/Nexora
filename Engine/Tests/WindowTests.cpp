#include <vector>

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
    std::vector<WindowResizedEvent> events;
    void onResized(const WindowResizedEvent& event) { events.push_back(event); }
};
}  // namespace

TEST(WindowSettingsTest, DefaultSettingsShouldBeValid)
{
    const WindowSettings settings;
    EXPECT_EQ(settings.title, "Nexora Engine");
    EXPECT_EQ(settings.width, 1280);
    EXPECT_EQ(settings.height, 720);
    EXPECT_TRUE(settings.visible);
}

TEST(GLFWWindowTest, WindowShouldBeCreatedWithSettings)
{
    entt::dispatcher events;
    const GLFWWindow window(makeTestSettings(), events);

    ASSERT_TRUE(window.isValid());
    EXPECT_EQ(window.title(), "Test Window");
    EXPECT_EQ(window.width(), 640);
    EXPECT_EQ(window.height(), 480);
    EXPECT_FALSE(window.shouldClose());
}

TEST(GLFWWindowTest, TitleShouldBeChanged)
{
    entt::dispatcher events;
    GLFWWindow window(makeTestSettings(), events);
    ASSERT_TRUE(window.isValid());

    window.setTitle("New Title");
    EXPECT_EQ(window.title(), "New Title");
}

TEST(GLFWWindowTest, SizeShouldBeChanged)
{
    entt::dispatcher events;
    GLFWWindow window(makeTestSettings(), events);
    ASSERT_TRUE(window.isValid());

    window.setSize(800, 600);
    EXPECT_EQ(window.width(), 800);
    EXPECT_EQ(window.height(), 600);
}

TEST(GLFWWindowTest, ResizeShouldBeQueuedAsEvent)
{
    entt::dispatcher events;
    ResizeListener listener;
    events.sink<WindowResizedEvent>().connect<&ResizeListener::onResized>(listener);

    GLFWWindow window(makeTestSettings(), events);
    ASSERT_TRUE(window.isValid());

    window.setSize(800, 600);
    window.pollEvents();
    EXPECT_TRUE(listener.events.empty()) << "events must be delivered only on update()";

    events.update();
    ASSERT_FALSE(listener.events.empty());
    EXPECT_EQ(listener.events.back().width, 800);
    EXPECT_EQ(listener.events.back().height, 600);
}

TEST(GLFWWindowTest, WindowShouldCloseAfterCloseRequest)
{
    entt::dispatcher events;
    GLFWWindow window(makeTestSettings(), events);
    ASSERT_TRUE(window.isValid());

    window.pollEvents();
    EXPECT_FALSE(window.shouldClose());

    window.close();
    window.pollEvents();
    EXPECT_TRUE(window.shouldClose());
}

TEST(GLFWWindowTest, WindowShouldBeRecreatedAfterDestruction)
{
    // GLFW is initialized and terminated together with the window, so it must work many times in a row
    entt::dispatcher events;
    for (int i = 0; i < 3; ++i)
    {
        const GLFWWindow window(makeTestSettings(), events);
        EXPECT_TRUE(window.isValid());
    }
}

TEST(GLFWWindowTest, InvalidWindowShouldBeHandledSafely)
{
    // GLFW fails to create a window with zero size
    entt::dispatcher events;
    GLFWWindow window({.title = "Invalid", .width = 0, .height = 0, .visible = false}, events);

    EXPECT_FALSE(window.isValid());
    EXPECT_TRUE(window.shouldClose());
    EXPECT_EQ(window.title(), "");
    EXPECT_EQ(window.width(), 0);
    EXPECT_EQ(window.height(), 0);

    window.setTitle("Ignored");
    window.setSize(100, 100);
    window.close();
    window.pollEvents();
}
