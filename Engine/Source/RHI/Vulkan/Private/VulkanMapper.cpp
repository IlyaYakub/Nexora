#include "RHI/Vulkan/Public/VulkanMapper.h"

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

VkAttachmentLoadOp toVkAttachmentLoadOp(RenderTargetLoadAction loadAction)
{
    switch (loadAction)
    {
        case RenderTargetLoadAction::Unknown: return VK_ATTACHMENT_LOAD_OP_MAX_ENUM;
        case RenderTargetLoadAction::Load: return VK_ATTACHMENT_LOAD_OP_LOAD;
        case RenderTargetLoadAction::Clear: return VK_ATTACHMENT_LOAD_OP_CLEAR;
        case RenderTargetLoadAction::DontCare: return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    }
    return VK_ATTACHMENT_LOAD_OP_MAX_ENUM;
}

std::string_view toString(VkResult result)
{
    switch (result)
    {
        case VK_SUCCESS: return "VK_SUCCESS";
        case VK_NOT_READY: return "VK_NOT_READY";
        case VK_TIMEOUT: return "VK_TIMEOUT";
        case VK_SUBOPTIMAL_KHR: return "VK_SUBOPTIMAL_KHR";
        case VK_ERROR_OUT_OF_HOST_MEMORY: return "VK_ERROR_OUT_OF_HOST_MEMORY";
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
        case VK_ERROR_INITIALIZATION_FAILED: return "VK_ERROR_INITIALIZATION_FAILED";
        case VK_ERROR_DEVICE_LOST: return "VK_ERROR_DEVICE_LOST";
        case VK_ERROR_LAYER_NOT_PRESENT: return "VK_ERROR_LAYER_NOT_PRESENT";
        case VK_ERROR_EXTENSION_NOT_PRESENT: return "VK_ERROR_EXTENSION_NOT_PRESENT";
        case VK_ERROR_FEATURE_NOT_PRESENT: return "VK_ERROR_FEATURE_NOT_PRESENT";
        case VK_ERROR_INCOMPATIBLE_DRIVER: return "VK_ERROR_INCOMPATIBLE_DRIVER";
        case VK_ERROR_SURFACE_LOST_KHR: return "VK_ERROR_SURFACE_LOST_KHR";
        case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
        case VK_ERROR_OUT_OF_DATE_KHR: return "VK_ERROR_OUT_OF_DATE_KHR";
        default: return "VK_RESULT_UNKNOWN";
    }
}

}  // namespace Nexora::VulkanMapper
