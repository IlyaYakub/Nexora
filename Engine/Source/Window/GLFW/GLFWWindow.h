#pragma once

#include "Window/IWindow.h"
#include "Window/WindowId.h"

struct GLFWwindow;

namespace Nexora
{

class GLFWWindow final : public IWindow
{
public:
    GLFWWindow(WindowId id, const WindowSettings& settings);
    ~GLFWWindow() override;

    bool shouldClose() const override;
    int32 width() const override;
    int32 height() const override;

    Event<const InputEvent&>& windowEvent() override;

private:
    WindowId m_id;
    GLFWwindow* m_window{nullptr};
    Event<const InputEvent&> m_windowEvent;
};

}  // namespace Nexora
