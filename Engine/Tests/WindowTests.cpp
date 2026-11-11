#include "Window/GLFW/GLFWWindowManager.h"

#include <gtest/gtest.h>

using namespace Nexora;

TEST(WindowTest, ShouldCreateWindowWithRequestedSize)
{
    GLFWWindowManager windowManager;
    const WindowSettings settings{.title = "Test", .width = 640, .height = 480};

    const WindowId windowId               = windowManager.createWindow(settings);
    const std::shared_ptr<IWindow> window = windowManager.getWindowById(windowId);

    EXPECT_EQ(window->width(), 640);
    EXPECT_EQ(window->height(), 480);
}

TEST(WindowTest, ShouldGiveEachWindowUniqueId)
{
    GLFWWindowManager windowManager;

    const WindowId firstId  = windowManager.createWindow(WindowSettings{});
    const WindowId secondId = windowManager.createWindow(WindowSettings{});

    EXPECT_NE(firstId, secondId);
}
