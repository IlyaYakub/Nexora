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
    void setPipeline(const IRHIPipeline& pipeline) override;
    void draw(uint32 vertexCount) override;
    void dispatch(uint32 groupCountX, uint32 groupCountY, uint32 groupCountZ) override;

private:
    const VulkanCommandBuffer& m_commandBuffer;
};

}  // namespace Nexora
