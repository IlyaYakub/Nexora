#pragma once

#include "Window/IWindow.h"

struct GLFWwindow;

namespace Nexora
{

// Forwards GLFW callbacks of the window to `events`. The caller keeps `events` alive as long as the window exists
// (the callbacks run inside glfwPollEvents()). Uses the GLFW user pointer of the window.
void installWindowCallbacks(GLFWwindow* handle, WindowEvents& events);

}  // namespace Nexora
