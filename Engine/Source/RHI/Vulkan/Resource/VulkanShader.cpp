#include "RHI/Vulkan/Resource/VulkanShader.h"

#include "RHI/Vulkan/Device/VulkanDevice.h"
#include "RHI/Vulkan/VulkanUtility.h"

#include <vulkan/vulkan.h>

namespace Nexora
{

VulkanShader::VulkanShader(const VulkanDevice& device, const ShaderBytecode& bytecode)
    : m_device(device),  //
      m_stage(bytecode.stage)
{
    const VkShaderModuleCreateInfo shaderModuleInfo{
        .sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = bytecode.code.size() * sizeof(uint32),
        .pCode    = bytecode.code.data(),
    };
    NX_VULKAN_CHECK(vkCreateShaderModule(m_device.logicalDevice(), &shaderModuleInfo, nullptr, &m_shaderModule));
}

VulkanShader::~VulkanShader()
{
    vkDestroyShaderModule(m_device.logicalDevice(), m_shaderModule, nullptr);
}

ShaderStage VulkanShader::stage() const
{
    return m_stage;
}

VkShaderModule_T* VulkanShader::handle() const
{
    return m_shaderModule;
}

}  // namespace Nexora
