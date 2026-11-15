#pragma once

#include "Core/Types.h"
#include "Event/InputEvent.h"

namespace Nexora::GLFWKeyMapper
{

[[nodiscard]] Key         toKey(int32 glfwKey);
[[nodiscard]] MouseButton toMouseButton(int32 glfwButton);
[[nodiscard]] InputAction toAction(int32 glfwAction);

}  // namespace Nexora::GLFWKeyMapper
