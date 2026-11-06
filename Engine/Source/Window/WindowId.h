#pragma once

#include "Core/Types.h"

namespace Nexora
{

// Opaque window handle. Comparison and std::hash come for free from the enum.
enum class WindowId : uint32
{
    Invalid = 0
};

}  // namespace Nexora
