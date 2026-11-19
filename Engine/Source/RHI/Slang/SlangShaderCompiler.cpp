#include "RHI/Slang/SlangShaderCompiler.h"

#include "Log/Log.h"

#include <slang-com-ptr.h>
#include <slang.h>

DEFINE_LOG_CATEGORY_STATIC(LogShaderCompiler);

namespace
{

Nexora::ShaderStage toShaderStage(SlangStage stage)
{
    switch (stage)
    {
        case SLANG_STAGE_VERTEX: return Nexora::ShaderStage::Vertex;
        case SLANG_STAGE_FRAGMENT: return Nexora::ShaderStage::Fragment;
        default: return Nexora::ShaderStage::Unknown;
    }
}

std::unexpected<Nexora::ShaderCompileError> compilationFailed(slang::IBlob* diagnostics)
{
    NX_LOG(LogShaderCompiler, Error, "{}", static_cast<const char*>(diagnostics->getBufferPointer()));
    return std::unexpected(Nexora::ShaderCompileError::CompilationFailed);
}

}  // namespace

namespace Nexora
{

SlangShaderCompiler::SlangShaderCompiler()
{
    // create session ->
    if (SLANG_FAILED(slang::createGlobalSession(&m_globalSession)))
    {
        NX_LOG(LogShaderCompiler, Fatal, "Failed to create global session");
    }

    const slang::TargetDesc targetDesc{
        .format  = SLANG_SPIRV,
        .profile = m_globalSession->findProfile("spirv_1_6"),
    };
    const slang::SessionDesc sessionDesc{
        .targets     = &targetDesc,
        .targetCount = 1,
    };
    if (SLANG_FAILED(m_globalSession->createSession(sessionDesc, &m_session)))
    {
        NX_LOG(LogShaderCompiler, Fatal, "Failed to create session");
    }
}

SlangShaderCompiler::~SlangShaderCompiler()
{
    m_session->release();
    m_globalSession->release();
}

std::expected<ShaderBytecode, ShaderCompileError> SlangShaderCompiler::compile(const std::filesystem::path& path,
    const std::string&                                                                                      entryPoint)
{
    Slang::ComPtr<slang::IBlob> diagnostics;

    // load ->
    auto* module = m_session->loadModule(path.string().c_str(), diagnostics.writeRef());
    if (module == nullptr)
    {
        return compilationFailed(diagnostics);
    }

    Slang::ComPtr<slang::IEntryPoint> moduleEntryPoint;
    if (SLANG_FAILED(module->findEntryPointByName(entryPoint.c_str(), moduleEntryPoint.writeRef())))
    {
        NX_LOG(LogShaderCompiler, Error, "Entry point {} not found in {}", entryPoint, path.string());
        return std::unexpected(ShaderCompileError::CompilationFailed);
    }

    // link ->
    slang::IComponentType*               components[] = {module, moduleEntryPoint};
    Slang::ComPtr<slang::IComponentType> program;
    m_session->createCompositeComponentType(components, 2, program.writeRef(), diagnostics.writeRef());

    Slang::ComPtr<slang::IComponentType> linkedProgram;
    if (SLANG_FAILED(program->link(linkedProgram.writeRef(), diagnostics.writeRef())))
    {
        return compilationFailed(diagnostics);
    }

    // generate code ->
    Slang::ComPtr<slang::IBlob> code;
    if (SLANG_FAILED(linkedProgram->getEntryPointCode(0, 0, code.writeRef(), diagnostics.writeRef())))
    {
        return compilationFailed(diagnostics);
    }

    const auto  stage     = linkedProgram->getLayout()->getEntryPointByIndex(0)->getStage();
    const auto* words     = static_cast<const uint32*>(code->getBufferPointer());
    const auto  wordCount = code->getBufferSize() / sizeof(uint32);
    return ShaderBytecode{.stage = toShaderStage(stage), .code = std::vector<uint32>(words, words + wordCount)};
}

}  // namespace Nexora
