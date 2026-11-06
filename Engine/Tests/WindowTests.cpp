#include "Window/GLFW/GLFWWindowManager.h"

#include <gtest/gtest.h>

using namespace Nexora;

// GLFW window manager (skipped when no display is available)

class GLFWWindowManagerTest : public testing::Test
{
protected:
    void SetUp() override
    {
        auto result = GLFWWindowManager::create();
        if (!result) GTEST_SKIP() << "GLFW cannot be initialized in this environment";
        m_manager = std::move(*result);
    }

    // Returns WindowId::Invalid when the environment cannot create windows.
    WindowId createHiddenWindow() const
    {
        WindowSettings settings;
        settings.visible = false;
        return m_manager->createWindow(settings).value_or(WindowId::Invalid);
    }

    std::unique_ptr<GLFWWindowManager> m_manager;
};

#define SKIP_IF_INVALID(id) \
    if ((id) == WindowId::Invalid) GTEST_SKIP() << "Window creation is not available in this environment"

TEST_F(GLFWWindowManagerTest, ManagerShouldStartWithoutWindows)
{
    EXPECT_TRUE(m_manager->areAllWindowsClosed());
}

TEST_F(GLFWWindowManagerTest, CreatedWindowShouldBeFoundById)
{
    const WindowId id = createHiddenWindow();
    SKIP_IF_INVALID(id);

    IWindow* window = m_manager->getWindow(id);
    ASSERT_NE(window, nullptr);
    EXPECT_EQ(window->id(), id);
    EXPECT_FALSE(m_manager->areAllWindowsClosed());
}

TEST_F(GLFWWindowManagerTest, UnknownIdShouldReturnNull)
{
    EXPECT_EQ(m_manager->getWindow(WindowId::Invalid), nullptr);
}

TEST_F(GLFWWindowManagerTest, ClosedWindowShouldBeRemovedOnUpdate)
{
    const WindowId id = createHiddenWindow();
    SKIP_IF_INVALID(id);

    m_manager->getWindow(id)->requestClose();
    m_manager->update();

    EXPECT_EQ(m_manager->getWindow(id), nullptr);
    EXPECT_TRUE(m_manager->areAllWindowsClosed());
}
