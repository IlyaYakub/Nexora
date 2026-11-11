#pragma once

#include "Window/IWindow.h"

struct GLFWwindow;

namespace Nexora
{

class GLFWWindow final : public IWindow
{
public:
    explicit GLFWWindow(const WindowSettings& settings);
    ~GLFWWindow() override;

    bool shouldClose() const override;
    int32 width() const override;
    int32 height() const override;

    Event<const InputEvent&>& windowEvent() override;

private:
    GLFWwindow* m_window{nullptr};
    Event<const InputEvent&> m_windowEvent;
};

}  // namespace Nexora
