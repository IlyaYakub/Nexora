#pragma once

#include "Core/Utility.h"
#include "RHI/IRHITexture.h"

namespace Nexora
{

class IRHIShader;

struct GraphicsPipelineSettings
{
    const IRHIShader* vertexShader{nullptr};
    const IRHIShader* fragmentShader{nullptr};
    TextureFormat     colorFormat{TextureFormat::Unknown};
};

class IRHIGraphicsPipeline : public NonCopyable
{
public:
    virtual ~IRHIGraphicsPipeline() = default;
};

}  // namespace Nexora
