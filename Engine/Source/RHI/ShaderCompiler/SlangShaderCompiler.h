#pragma once

#include "RHI/ShaderCompiler/IShaderCompiler.h"

namespace slang
{
struct IGlobalSession;
struct ISession;
}  // namespace slang

namespace Nexora
{

class SlangShaderCompiler final : public IShaderCompiler
{
public:
    SlangShaderCompiler();
    ~SlangShaderCompiler() override;

    // core
    std::expected<ShaderBytecode, ShaderCompileError> compile(const std::filesystem::path& path, const std::string& entryPoint) override;

private:
    slang::IGlobalSession* m_globalSession{nullptr};
    slang::ISession*       m_session{nullptr};
};

}  // namespace Nexora
