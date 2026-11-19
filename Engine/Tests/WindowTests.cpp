#include "Window/GLFW/GLFWWindowManager.h"

#include <gtest/gtest.h>

using namespace Nexora;

TEST(WindowTest, ShouldCreateWindowWithRequestedSize)
{
    GLFWWindowManager    windowManager;
    const WindowSettings settings{
        .title  = "Test",
        .width  = 640,
        .height = 480,
    };

    const auto windowId = windowManager.createWindow(settings);
    const auto window   = windowManager.getWindowById(windowId);

    EXPECT_EQ(window->width(), 640);
    EXPECT_EQ(window->height(), 480);
}

TEST(WindowTest, ShouldNotifyWhenWindowCreated)
{
    GLFWWindowManager windowManager;
    WindowId          createdId;
    windowManager.windowCreatedEvent().add([&createdId](WindowId id) { createdId = id; });

    const auto windowId = windowManager.createWindow(WindowSettings{});

    EXPECT_EQ(createdId, windowId);
}

TEST(WindowTest, ShouldGiveEachWindowUniqueId)
{
    GLFWWindowManager windowManager;

    const auto firstId  = windowManager.createWindow(WindowSettings{});
    const auto secondId = windowManager.createWindow(WindowSettings{});

    EXPECT_NE(firstId, secondId);
}
