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

    // core
    void     update() override;
    WindowId createWindow(const WindowSettings& settings) override;

    // getters
    bool                     areAllWindowsClosed() const override;
    std::shared_ptr<IWindow> getWindowById(WindowId id) const override;
    Event<WindowId>&         windowCreatedEvent() override;

private:
    void removeClosedWindows();

    std::unordered_map<WindowId, std::shared_ptr<IWindow>> m_windows;
    WindowId                                               m_nextWindowId{1};
    Event<WindowId>                                        m_windowCreatedEvent;
};

}  // namespace Nexora
