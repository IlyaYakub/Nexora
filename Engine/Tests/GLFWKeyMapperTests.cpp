#include "Window/GLFW/GLFWKeyMapper.h"

#include <GLFW/glfw3.h>
#include <gtest/gtest.h>

using namespace Nexora;

TEST(GLFWKeyMapperTest, ShouldMapKey)
{
    const Key key = GLFWKeyMapper::toKey(GLFW_KEY_W);

    EXPECT_EQ(key, Key::W);
}

TEST(GLFWKeyMapperTest, ShouldMapMouseButton)
{
    const MouseButton button = GLFWKeyMapper::toMouseButton(GLFW_MOUSE_BUTTON_RIGHT);

    EXPECT_EQ(button, MouseButton::Right);
}

TEST(GLFWKeyMapperTest, ShouldMapAction)
{
    const InputAction action = GLFWKeyMapper::toAction(GLFW_PRESS);

    EXPECT_EQ(action, InputAction::Press);
}
