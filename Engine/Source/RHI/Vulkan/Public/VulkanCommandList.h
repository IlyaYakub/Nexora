#pragma once

#include "RHI/IRHICommandList.h"

namespace Nexora
{

class VulkanCommandBuffer;

class VulkanCommandList final : public IRHICommandList
{
public:
    explicit VulkanCommandList(const VulkanCommandBuffer& commandBuffer);

    // core
    void beginRenderPass(const RenderPassInfo& info) override;
    void endRenderPass() override;
    void setPipeline(const IRHIGraphicsPipeline& pipeline) override;
    void draw(uint32 vertexCount) override;

private:
    const VulkanCommandBuffer& m_commandBuffer;
};

}  // namespace Nexora
