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
    void beginRendering(IRHITexture& texture, const std::optional<std::array<float, 4>>& clearColor) override;
    void endRendering() override;

private:
    const VulkanCommandBuffer& m_commandBuffer;
};

}  // namespace Nexora
