#pragma once

#include "Core/Utility.h"
#include "Window/IWindow.h"
#include "Window/IWindowManager.h"

#include <expected>
#include <memory>

struct GLFWwindow;

namespace Nexora
{

class GLFWWindow final : public IWindow, public NonCopyable
{
public:
    // GLFW must already be initialized (GLFWWindowManager does this).
    static std::expected<std::unique_ptr<GLFWWindow>, WindowError> create(WindowId id, const WindowSettings& settings);

    ~GLFWWindow() override;

    WindowId id() const override;
    void setTitle(const std::string& title) override;
    bool shouldClose() const override;
    void requestClose() override;
    FramebufferSize framebufferSize() const override;

private:
    GLFWWindow(WindowId id, GLFWwindow* handle);

    const WindowId m_id;
    GLFWwindow* const m_handle;
};

}  // namespace Nexora
