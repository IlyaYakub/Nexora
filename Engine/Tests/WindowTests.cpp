#include <entt/signal/dispatcher.hpp>
#include <gtest/gtest.h>

#include "Event/WindowEvents.h"
#include "Window/GLFW/GLFWWindow.h"

using namespace Nexora;

TEST(WindowLifecycleTest, WindowLifecycle)
{
    entt::dispatcher events;
    GLFWWindow window{{.title = "Test Window", .width = 640, .height = 480, .visible = false}, events};

    ASSERT_TRUE(window.isValid());
    EXPECT_EQ(window.title(), "Test Window");
    EXPECT_EQ(window.width(), 640);
    EXPECT_EQ(window.height(), 480);
    EXPECT_FALSE(window.shouldClose());

    window.pollEvents();
    window.close();

    EXPECT_TRUE(window.shouldClose());
}

TEST(WindowLifecycleTest, WindowResize)
{
    entt::dispatcher events;
    GLFWWindow window{{.title = "Test Window", .width = 640, .height = 480, .visible = false}, events};

    ASSERT_TRUE(window.isValid());

    window.setSize(800, 800);
    EXPECT_EQ(window.width(), 800);
    EXPECT_EQ(window.height(), 800);

    window.close();
}