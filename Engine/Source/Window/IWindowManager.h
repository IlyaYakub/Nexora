#pragma once

#include "Core/Types.h"
#include "Window/IWindow.h"
#include "Window/WindowId.h"

#include <expected>

namespace Nexora
{

enum class WindowError : uint8
{
    BackendInitFailed,
    WindowCreationFailed
};

class IWindowManager
{
public:
    virtual ~IWindowManager() = default;

    // Processes pending window events and destroys windows that were closed.
    virtual void update()                    = 0;
    virtual bool areAllWindowsClosed() const = 0;

    virtual std::expected<WindowId, WindowError> createWindow(const WindowSettings& settings) = 0;

    // Non-owning. Returns nullptr for an unknown id. The pointer stays valid until the next update().
    virtual IWindow* getWindow(WindowId id) const = 0;
};

}  // namespace Nexora
