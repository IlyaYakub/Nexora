#pragma once

#include "RHI/IRHITexture.h"

#include <vulkan/vulkan.h>

namespace Nexora::VulkanMapper
{

[[nodiscard]] VkFormat toVkFormat(TextureFormat format);

}  // namespace Nexora::VulkanMapper
