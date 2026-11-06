#pragma once

#include "Core/Types.h"
#include "Core/Utility.h"
#include "Window/IWindowManager.h"

#include <expected>
#include <memory>
#include <unordered_map>

namespace Nexora
{

class GLFWWindow;

// Owns the GLFW library state and all windows. GLFW initialization is process-global,
// so only one manager may exist at a time.
class GLFWWindowManager final : public IWindowManager, public NonCopyable
{
public:
    static std::expected<std::unique_ptr<GLFWWindowManager>, WindowError> create();

    ~GLFWWindowManager() override;

    void update() override;
    bool areAllWindowsClosed() const override;

    std::expected<WindowId, WindowError> createWindow(const WindowSettings& settings) override;
    IWindow* getWindow(WindowId id) const override;

private:
    GLFWWindowManager();

    std::unordered_map<WindowId, std::unique_ptr<GLFWWindow>> m_windows;
    uint32 m_nextWindowId{1};
};

}  // namespace Nexora
