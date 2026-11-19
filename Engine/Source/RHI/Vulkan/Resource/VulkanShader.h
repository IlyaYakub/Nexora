#pragma once

#include "RHI/IRHIShader.h"

struct VkShaderModule_T;

namespace Nexora
{

class VulkanDevice;

class VulkanShader final : public IRHIShader
{
public:
    VulkanShader(const VulkanDevice& device, const ShaderBytecode& bytecode);
    ~VulkanShader() override;

    // getters
    ShaderStage       stage() const override;
    VkShaderModule_T* handle() const;

private:
    const VulkanDevice& m_device;
    ShaderStage         m_stage{ShaderStage::Unknown};
    VkShaderModule_T*   m_shaderModule{nullptr};
};

}  // namespace Nexora
