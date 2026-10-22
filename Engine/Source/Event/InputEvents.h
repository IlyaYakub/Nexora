#pragma once

#include "Input/KeyCodes.h"

namespace Nexora
{

struct KeyPressedEvent
{
    Key key{Key::Unknown};
    bool repeat{false};  // true when the key is held down
};

struct KeyReleasedEvent
{
    Key key{Key::Unknown};
};

struct MouseMovedEvent
{
    double x{0.0};
    double y{0.0};
};

struct MouseButtonPressedEvent
{
    MouseButton button{MouseButton::Left};
};

struct MouseButtonReleasedEvent
{
    MouseButton button{MouseButton::Left};
};

struct MouseScrolledEvent
{
    double xOffset{0.0};
    double yOffset{0.0};
};

}  // namespace Nexora
