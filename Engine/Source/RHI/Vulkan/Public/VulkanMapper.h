#pragma once

#include "RHI/IRHICommandList.h"
#include "RHI/IRHIPipeline.h"
#include "RHI/IRHIShader.h"
#include "RHI/IRHITexture.h"

#include <string_view>

#include <vulkan/vulkan.h>

namespace Nexora::VulkanMapper
{

[[nodiscard]] VkFormat              toVkFormat(TextureFormat format);
[[nodiscard]] VkShaderStageFlagBits toVkShaderStage(ShaderStage stage);
[[nodiscard]] VkAttachmentLoadOp    toVkAttachmentLoadOp(RenderTargetLoadAction loadAction);
[[nodiscard]] VkPipelineBindPoint   toVkPipelineBindPoint(PipelineType type);
[[nodiscard]] std::string_view      toString(VkResult result);

}  // namespace Nexora::VulkanMapper
