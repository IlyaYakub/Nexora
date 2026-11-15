#pragma once

#include <optional>

#include <glm/vec4.hpp>

#include "Core/Utility.h"

namespace Nexora
{

class IRHITexture;

class IRHICommandList : public NonCopyable
{
public:
    virtual ~IRHICommandList() = default;

    // core
    virtual void beginRendering(IRHITexture& texture, const std::optional<glm::vec4>& clearColor) = 0;
    virtual void endRendering()                                                                   = 0;
};

}  // namespace Nexora
