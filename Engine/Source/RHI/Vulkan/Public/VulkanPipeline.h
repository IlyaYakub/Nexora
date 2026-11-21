#pragma once

#include "RHI/IRHIPipeline.h"

struct VkPipeline_T;
struct VkPipelineLayout_T;

namespace Nexora
{

class VulkanDevice;

class VulkanPipeline final : public IRHIPipeline
{
public:
    VulkanPipeline(const VulkanDevice& device, const PipelineSettings& settings);
    ~VulkanPipeline() override;

    // getters
    PipelineType  type() const override;
    VkPipeline_T* pipeline() const;

private:
    const VulkanDevice& m_device;
    PipelineType        m_type{PipelineType::Unknown};
    VkPipelineLayout_T* m_pipelineLayout{nullptr};
    VkPipeline_T*       m_pipeline{nullptr};
};

}  // namespace Nexora
