#pragma once

#include <glm/vec4.hpp>

#include "Core/Types.h"
#include "Core/Utility.h"

namespace Nexora
{

class IRHIGraphicsPipeline;
class IRHITexture;

enum class RenderTargetLoadAction : uint8
{
    Unknown,
    Load,
    Clear,
    DontCare
};

struct RenderPassInfo
{
    IRHITexture*           colorTarget{nullptr};
    RenderTargetLoadAction loadAction{RenderTargetLoadAction::Unknown};
    glm::vec4              clearColor{0.0f, 0.0f, 0.0f, 1.0f};
};

class IRHICommandList : public NonCopyable
{
public:
    virtual ~IRHICommandList() = default;

    // core
    virtual void beginRenderPass(const RenderPassInfo& info)       = 0;
    virtual void endRenderPass()                                   = 0;
    virtual void setPipeline(const IRHIGraphicsPipeline& pipeline) = 0;
    virtual void draw(uint32 vertexCount)                          = 0;
};

}  // namespace Nexora
