#pragma once

#include <vector>

#include "Core/Types.h"
#include "Core/Utility.h"

namespace Nexora
{

enum class ShaderStage : uint8
{
    Unknown,
    Vertex,
    Fragment,
    Compute
};

struct ShaderBytecode
{
    ShaderStage         stage{ShaderStage::Unknown};
    std::vector<uint32> code;
};

class IRHIShader : public NonCopyable
{
public:
    virtual ~IRHIShader() = default;

    // getters
    virtual ShaderStage stage() const = 0;
};

}  // namespace Nexora
