#pragma once

#include "Window/WindowId.h"

#include <string>

namespace Nexora
{

struct WindowSettings
{
    std::string title{"Nexora"};
    int width{1280};
    int height{720};
    int x{50};
    int y{50};
    bool visible{true};
};

struct FramebufferSize
{
    int width{0};
    int height{0};
};

class IWindow
{
public:
    virtual ~IWindow() = default;

    virtual WindowId id() const = 0;
    virtual void setTitle(const std::string& title) = 0;
    virtual bool shouldClose() const = 0;
    virtual void requestClose() = 0;
    virtual FramebufferSize framebufferSize() const = 0;
};

}  // namespace Nexora
