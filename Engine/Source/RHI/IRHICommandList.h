#pragma once

#include <optional>

#include <glm/vec4.hpp>

#include "Core/Types.h"
#include "Core/Utility.h"

namespace Nexora
{

class IRHIGraphicsPipeline;
class IRHITexture;

class IRHICommandList : public NonCopyable
{
public:
    virtual ~IRHICommandList() = default;

    // core
    virtual void beginRendering(IRHITexture& texture, const std::optional<glm::vec4>& clearColor) = 0;
    virtual void endRendering()                                                                   = 0;
    virtual void setPipeline(const IRHIGraphicsPipeline& pipeline)                                = 0;
    virtual void draw(uint32 vertexCount)                                                         = 0;
};

}  // namespace Nexora
