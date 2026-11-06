#pragma once

#include "Core/Types.h"

namespace Nexora
{

enum class GraphicsAPI : uint8
{
    Vulkan
};

enum class GraphicsError : uint8
{
    LoaderNotFound,
    InstanceCreationFailed,
    DebugMessengerCreationFailed
};

struct GraphicsContextDesc
{
#ifdef NDEBUG
    bool enableValidation{false};
#else
    bool enableValidation{true};
#endif
};

class IGraphicsContext
{
public:
    virtual ~IGraphicsContext() = default;

    virtual GraphicsAPI api() const = 0;
};

}  // namespace Nexora
