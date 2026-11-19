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

}  // namespace Nexora::VulkanMapper
