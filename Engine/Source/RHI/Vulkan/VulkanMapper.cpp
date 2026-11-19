#include "RHI/Vulkan/VulkanMapper.h"

namespace Nexora::VulkanMapper
{

VkFormat toVkFormat(TextureFormat format)
{
    switch (format)
    {
        case TextureFormat::Unknown: return VK_FORMAT_UNDEFINED;
        case TextureFormat::B8G8R8A8Srgb: return VK_FORMAT_B8G8R8A8_SRGB;
    }
    return VK_FORMAT_UNDEFINED;
}

VkShaderStageFlagBits toVkShaderStage(ShaderStage stage)
{
    switch (stage)
    {
        case ShaderStage::Unknown: return VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
        case ShaderStage::Vertex: return VK_SHADER_STAGE_VERTEX_BIT;
        case ShaderStage::Fragment: return VK_SHADER_STAGE_FRAGMENT_BIT;
    }
    return VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
}

}  // namespace Nexora::VulkanMapper
