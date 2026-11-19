#pragma once

#include "RHI/IRHIShader.h"
#include "RHI/IRHITexture.h"

#include <vulkan/vulkan.h>

namespace Nexora::VulkanMapper
{

[[nodiscard]] VkFormat              toVkFormat(TextureFormat format);
[[nodiscard]] VkShaderStageFlagBits toVkShaderStage(ShaderStage stage);

}  // namespace Nexora::VulkanMapper
