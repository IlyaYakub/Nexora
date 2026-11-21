#pragma once

#include <expected>
#include <filesystem>
#include <string>

#include "Core/Types.h"
#include "Core/Utility.h"
#include "RHI/IRHIShader.h"

namespace Nexora
{

enum class ShaderCompileError : uint8
{
    Unknown,
    CompilationFailed
};

class IShaderCompiler : public NonCopyable
{
public:
    virtual ~IShaderCompiler() = default;

    // core
    virtual std::expected<ShaderBytecode, ShaderCompileError> compile(const std::filesystem::path& path, const std::string& entryPoint) = 0;
};

}  // namespace Nexora
