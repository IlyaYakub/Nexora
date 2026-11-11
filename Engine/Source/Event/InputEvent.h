#pragma once

#include <variant>

#include "Core/Types.h"
#include "Window/WindowId.h"

namespace Nexora
{

enum class InputAction : uint8
{
    Unknown,
    Release,
    Press,
    Repeat
};

enum class MouseButton : uint8
{
    Unknown,
    Left,
    Right,
    Middle
};

enum class Key : uint16
{
    Unknown,

    Escape,
    Enter,
    Tab,
    Space,
    Backspace,
    Insert,
    Delete,
    Home,
    End,
    PageUp,
    PageDown,

    Up,
    Down,
    Left,
    Right,

    LeftShift,
    RightShift,
    LeftControl,
    RightControl,
    LeftAlt,
    RightAlt,

    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,

    Key0,
    Key1,
    Key2,
    Key3,
    Key4,
    Key5,
    Key6,
    Key7,
    Key8,
    Key9,

    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z
};

struct KeyEventData
{
    Key key{Key::Unknown};
    InputAction action{InputAction::Unknown};
};

struct MouseButtonEventData
{
    MouseButton button{MouseButton::Unknown};
    InputAction action{InputAction::Unknown};
};

struct MouseMoveEventData
{
    double x{0.0};
    double y{0.0};
};

struct MouseScrollEventData
{
    double xOffset{0.0};
    double yOffset{0.0};
};

struct WindowResizeEventData
{
    int32 width{0};
    int32 height{0};
};

struct WindowCloseEventData
{
};

using InputEventData =
    std::variant<KeyEventData, MouseButtonEventData, MouseMoveEventData, MouseScrollEventData, WindowResizeEventData, WindowCloseEventData>;

struct InputEvent
{
    WindowId windowId;
    InputEventData data;
};

}  // namespace Nexora
