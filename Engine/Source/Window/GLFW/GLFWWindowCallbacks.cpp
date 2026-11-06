#include "GLFWWindowCallbacks.h"

#include "Log/Log.h"

#include <GLFW/glfw3.h>

using namespace Nexora;

DEFINE_LOG_CATEGORY_STATIC(LogGLFWWindowCallbacks)

namespace
{

template <typename Enum>
constexpr bool sameAsGLFW(Enum value, int glfwValue)
{
    return value == static_cast<Enum>(glfwValue);
}

static_assert(sameAsGLFW(InputAction::Release, GLFW_RELEASE));
static_assert(sameAsGLFW(InputAction::Press, GLFW_PRESS));
static_assert(sameAsGLFW(InputAction::Repeat, GLFW_REPEAT));

static_assert(sameAsGLFW(MouseButton::Left, GLFW_MOUSE_BUTTON_LEFT));
static_assert(sameAsGLFW(MouseButton::Right, GLFW_MOUSE_BUTTON_RIGHT));
static_assert(sameAsGLFW(MouseButton::Middle, GLFW_MOUSE_BUTTON_MIDDLE));

static_assert(sameAsGLFW(KeyCode::Unknown, GLFW_KEY_UNKNOWN));
static_assert(sameAsGLFW(KeyCode::Space, GLFW_KEY_SPACE));

static_assert(sameAsGLFW(KeyCode::Num0, GLFW_KEY_0));
static_assert(sameAsGLFW(KeyCode::Num1, GLFW_KEY_1));
static_assert(sameAsGLFW(KeyCode::Num2, GLFW_KEY_2));
static_assert(sameAsGLFW(KeyCode::Num3, GLFW_KEY_3));
static_assert(sameAsGLFW(KeyCode::Num4, GLFW_KEY_4));
static_assert(sameAsGLFW(KeyCode::Num5, GLFW_KEY_5));
static_assert(sameAsGLFW(KeyCode::Num6, GLFW_KEY_6));
static_assert(sameAsGLFW(KeyCode::Num7, GLFW_KEY_7));
static_assert(sameAsGLFW(KeyCode::Num8, GLFW_KEY_8));
static_assert(sameAsGLFW(KeyCode::Num9, GLFW_KEY_9));

static_assert(sameAsGLFW(KeyCode::A, GLFW_KEY_A));
static_assert(sameAsGLFW(KeyCode::B, GLFW_KEY_B));
static_assert(sameAsGLFW(KeyCode::C, GLFW_KEY_C));
static_assert(sameAsGLFW(KeyCode::D, GLFW_KEY_D));
static_assert(sameAsGLFW(KeyCode::E, GLFW_KEY_E));
static_assert(sameAsGLFW(KeyCode::F, GLFW_KEY_F));
static_assert(sameAsGLFW(KeyCode::G, GLFW_KEY_G));
static_assert(sameAsGLFW(KeyCode::H, GLFW_KEY_H));
static_assert(sameAsGLFW(KeyCode::I, GLFW_KEY_I));
static_assert(sameAsGLFW(KeyCode::J, GLFW_KEY_J));
static_assert(sameAsGLFW(KeyCode::K, GLFW_KEY_K));
static_assert(sameAsGLFW(KeyCode::L, GLFW_KEY_L));
static_assert(sameAsGLFW(KeyCode::M, GLFW_KEY_M));
static_assert(sameAsGLFW(KeyCode::N, GLFW_KEY_N));
static_assert(sameAsGLFW(KeyCode::O, GLFW_KEY_O));
static_assert(sameAsGLFW(KeyCode::P, GLFW_KEY_P));
static_assert(sameAsGLFW(KeyCode::Q, GLFW_KEY_Q));
static_assert(sameAsGLFW(KeyCode::R, GLFW_KEY_R));
static_assert(sameAsGLFW(KeyCode::S, GLFW_KEY_S));
static_assert(sameAsGLFW(KeyCode::T, GLFW_KEY_T));
static_assert(sameAsGLFW(KeyCode::U, GLFW_KEY_U));
static_assert(sameAsGLFW(KeyCode::V, GLFW_KEY_V));
static_assert(sameAsGLFW(KeyCode::W, GLFW_KEY_W));
static_assert(sameAsGLFW(KeyCode::X, GLFW_KEY_X));
static_assert(sameAsGLFW(KeyCode::Y, GLFW_KEY_Y));
static_assert(sameAsGLFW(KeyCode::Z, GLFW_KEY_Z));

static_assert(sameAsGLFW(KeyCode::Escape, GLFW_KEY_ESCAPE));
static_assert(sameAsGLFW(KeyCode::Enter, GLFW_KEY_ENTER));
static_assert(sameAsGLFW(KeyCode::Tab, GLFW_KEY_TAB));
static_assert(sameAsGLFW(KeyCode::Backspace, GLFW_KEY_BACKSPACE));
static_assert(sameAsGLFW(KeyCode::Right, GLFW_KEY_RIGHT));
static_assert(sameAsGLFW(KeyCode::Left, GLFW_KEY_LEFT));
static_assert(sameAsGLFW(KeyCode::Down, GLFW_KEY_DOWN));
static_assert(sameAsGLFW(KeyCode::Up, GLFW_KEY_UP));

static_assert(sameAsGLFW(KeyCode::F1, GLFW_KEY_F1));
static_assert(sameAsGLFW(KeyCode::F2, GLFW_KEY_F2));
static_assert(sameAsGLFW(KeyCode::F3, GLFW_KEY_F3));
static_assert(sameAsGLFW(KeyCode::F4, GLFW_KEY_F4));
static_assert(sameAsGLFW(KeyCode::F5, GLFW_KEY_F5));
static_assert(sameAsGLFW(KeyCode::F6, GLFW_KEY_F6));
static_assert(sameAsGLFW(KeyCode::F7, GLFW_KEY_F7));
static_assert(sameAsGLFW(KeyCode::F8, GLFW_KEY_F8));
static_assert(sameAsGLFW(KeyCode::F9, GLFW_KEY_F9));
static_assert(sameAsGLFW(KeyCode::F10, GLFW_KEY_F10));
static_assert(sameAsGLFW(KeyCode::F11, GLFW_KEY_F11));
static_assert(sameAsGLFW(KeyCode::F12, GLFW_KEY_F12));

static_assert(sameAsGLFW(KeyCode::LeftShift, GLFW_KEY_LEFT_SHIFT));
static_assert(sameAsGLFW(KeyCode::LeftControl, GLFW_KEY_LEFT_CONTROL));
static_assert(sameAsGLFW(KeyCode::LeftAlt, GLFW_KEY_LEFT_ALT));
static_assert(sameAsGLFW(KeyCode::RightShift, GLFW_KEY_RIGHT_SHIFT));
static_assert(sameAsGLFW(KeyCode::RightControl, GLFW_KEY_RIGHT_CONTROL));
static_assert(sameAsGLFW(KeyCode::RightAlt, GLFW_KEY_RIGHT_ALT));

WindowEvents& eventsFrom(GLFWwindow* handle)
{
    return *static_cast<WindowEvents*>(glfwGetWindowUserPointer(handle));
}

}  // namespace

void Nexora::installWindowCallbacks(GLFWwindow* handle, WindowEvents& events)
{
    // GLFW callbacks cannot capture, so they find the events through the user pointer.
    glfwSetWindowUserPointer(handle, &events);

    glfwSetWindowCloseCallback(handle,
        [](GLFWwindow* window)
        {
            NX_LOG(LogGLFWWindowCallbacks, Display, "Close requested");
            eventsFrom(window).closeRequested.invoke();
        });

    glfwSetFramebufferSizeCallback(handle,
        [](GLFWwindow* window, int width, int height)
        {
            NX_LOG(LogGLFWWindowCallbacks, Display, "Framebuffer resized to {}x{}", width, height);
            eventsFrom(window).framebufferResized.invoke(FramebufferSize{width, height});
        });

    glfwSetKeyCallback(handle,
        [](GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/)
        {
            NX_LOG(LogGLFWWindowCallbacks, Display, "Key {} action {}", key, action);
            eventsFrom(window).key.invoke(KeyEvent{static_cast<KeyCode>(key), static_cast<InputAction>(action)});
        });

    glfwSetMouseButtonCallback(handle,
        [](GLFWwindow* window, int button, int action, int /*mods*/)
        {
            NX_LOG(LogGLFWWindowCallbacks, Display, "Mouse button {} action {}", button, action);
            eventsFrom(window).mouseButton.invoke(MouseButtonEvent{static_cast<MouseButton>(button), static_cast<InputAction>(action)});
        });

    glfwSetCursorPosCallback(handle,
        [](GLFWwindow* window, double x, double y)
        {
            NX_LOG(LogGLFWWindowCallbacks, Display, "Cursor moved to {}, {}", x, y);
            eventsFrom(window).cursorMoved.invoke(CursorPosition{x, y});
        });

    glfwSetScrollCallback(handle,
        [](GLFWwindow* window, double x, double y)
        {
            NX_LOG(LogGLFWWindowCallbacks, Display, "Scrolled {}, {}", x, y);
            eventsFrom(window).scrolled.invoke(ScrollOffset{x, y});
        });
}
