#pragma once

#include "Core/Types.h"

namespace Nexora
{

enum class VulkanResourceState : uint8
{
    Unknown,
    ColorAttachment,
    Present,
};

}  // namespace Nexora
