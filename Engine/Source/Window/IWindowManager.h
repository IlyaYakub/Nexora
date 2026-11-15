#pragma once

#include <memory>

#include "Core/Utility.h"
#include "Event/Event.h"
#include "Window/IWindow.h"
#include "Window/WindowId.h"

namespace Nexora
{

class IWindowManager : public NonCopyable
{
public:
    virtual ~IWindowManager() = default;

    // core
    virtual void     update()                                     = 0;
    virtual WindowId createWindow(const WindowSettings& settings) = 0;

    // getters
    virtual bool                     areAllWindowsClosed() const      = 0;
    virtual std::shared_ptr<IWindow> getWindowById(WindowId id) const = 0;
    virtual Event<WindowId>&         windowCreatedEvent()             = 0;
};

}  // namespace Nexora
