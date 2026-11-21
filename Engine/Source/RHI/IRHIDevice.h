#pragma once

#include <memory>

#include "Core/Utility.h"

namespace Nexora
{

class IRHIPipeline;
class IRHIShader;
class IRHIViewport;
struct PipelineSettings;
struct ShaderBytecode;
struct ViewportSettings;

class IRHIDevice : public NonCopyable
{
public:
    virtual ~IRHIDevice() = default;

    // core
    virtual std::unique_ptr<IRHIViewport> createViewport(const ViewportSettings& settings) = 0;
    virtual std::unique_ptr<IRHIShader>   createShader(const ShaderBytecode& bytecode)     = 0;
    virtual std::unique_ptr<IRHIPipeline> createPipeline(const PipelineSettings& settings) = 0;
};

}  // namespace Nexora
