#pragma once

#include <memory>
#include <unordered_map>

#include "Window/IWindowManager.h"

namespace Nexora
{

class GLFWWindowManager final : public IWindowManager
{
public:
    GLFWWindowManager();
    ~GLFWWindowManager() override;

    void update() override;
    bool areAllWindowsClosed() const override;
    WindowId createWindow(const WindowSettings& settings) override;
    std::shared_ptr<IWindow> getWindowById(WindowId id) const override;

private:
    void removeClosedWindows();

    std::unordered_map<WindowId, std::shared_ptr<IWindow>> m_windows;
    WindowId m_nextWindowId{1};
};

}  // namespace Nexora
