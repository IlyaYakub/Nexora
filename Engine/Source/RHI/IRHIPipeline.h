#pragma once

#include "Core/Types.h"
#include "Core/Utility.h"
#include "RHI/IRHITexture.h"

namespace Nexora
{

class IRHIShader;

enum class PipelineType : uint8
{
    Unknown,
    Graphics,
    Compute
};

struct PipelineSettings
{
    PipelineType      type{PipelineType::Unknown};
    const IRHIShader* vertexShader{nullptr};
    const IRHIShader* fragmentShader{nullptr};
    const IRHIShader* computeShader{nullptr};
    TextureFormat     colorFormat{TextureFormat::Unknown};
};

class IRHIPipeline : public NonCopyable
{
public:
    virtual ~IRHIPipeline() = default;

    // getters
    virtual PipelineType type() const = 0;
};

}  // namespace Nexora
