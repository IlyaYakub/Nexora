#pragma once

#include <glm/vec4.hpp>

#include "Core/Types.h"
#include "Core/Utility.h"

namespace Nexora
{

class IRHIPipeline;
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
    virtual void beginRenderPass(const RenderPassInfo& info)                          = 0;
    virtual void endRenderPass()                                                      = 0;
    virtual void setPipeline(const IRHIPipeline& pipeline)                            = 0;
    virtual void draw(uint32 vertexCount)                                             = 0;
    virtual void dispatch(uint32 groupCountX, uint32 groupCountY, uint32 groupCountZ) = 0;
};

}  // namespace Nexora
