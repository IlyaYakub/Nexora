#pragma once

#include <string>

#include "Core/Types.h"
#include "Core/Utility.h"
#include "Event/Event.h"
#include "Event/InputEvent.h"

namespace Nexora
{

struct WindowSettings
{
    std::string title{"Nexora"};
    int32 width{1280};
    int32 height{720};
};

class IWindow : public NonCopyable
{
public:
    virtual ~IWindow() = default;

    virtual bool shouldClose() const = 0;
    virtual int32 width() const      = 0;
    virtual int32 height() const     = 0;

    virtual Event<const InputEvent&>& windowEvent() = 0;
};

}  // namespace Nexora
