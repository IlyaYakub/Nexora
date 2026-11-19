#include "RHI/Slang/SlangShaderCompiler.h"

#include <gtest/gtest.h>

using namespace Nexora;

TEST(ShaderCompilerTest, ShouldCompileEntryPointToSpirv)
{
    SlangShaderCompiler shaderCompiler;
    const auto          path = std::filesystem::path(NX_TEST_SHADER_DIRECTORY) / "TestShader.slang";

    const auto bytecode = shaderCompiler.compile(path, "vertexMain");

    ASSERT_TRUE(bytecode.has_value());
    EXPECT_EQ(bytecode->stage, ShaderStage::Vertex);
    EXPECT_EQ(bytecode->code.front(), 0x07230203u);
}
