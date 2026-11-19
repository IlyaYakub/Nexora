#pragma once

#include "RHI/IRHIGraphicsPipeline.h"

struct VkPipeline_T;
struct VkPipelineLayout_T;

namespace Nexora
{

class VulkanDevice;

class VulkanGraphicsPipeline final : public IRHIGraphicsPipeline
{
public:
    VulkanGraphicsPipeline(const VulkanDevice& device, const GraphicsPipelineSettings& settings);
    ~VulkanGraphicsPipeline() override;

    // getters
    VkPipeline_T* pipeline() const;

private:
    const VulkanDevice& m_device;
    VkPipelineLayout_T* m_pipelineLayout{nullptr};
    VkPipeline_T*       m_pipeline{nullptr};
};

}  // namespace Nexora
