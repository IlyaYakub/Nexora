#pragma once

#include <compare>
#include <functional>

#include "Core/Types.h"

namespace Nexora
{

struct WindowId
{
    uint32 value{0};

    auto operator<=>(const WindowId&) const = default;
};

}  // namespace Nexora

template <>
struct std::hash<Nexora::WindowId>
{
    size_t operator()(const Nexora::WindowId& id) const noexcept { return std::hash<Nexora::uint32>{}(id.value); }
};
