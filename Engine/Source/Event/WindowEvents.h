#pragma once

namespace Nexora
{

// the user pressed the close button of the window
struct WindowClosedEvent
{
};

// framebuffer size in pixels
struct WindowResizedEvent
{
    int width{0};
    int height{0};
};

}  // namespace Nexora
