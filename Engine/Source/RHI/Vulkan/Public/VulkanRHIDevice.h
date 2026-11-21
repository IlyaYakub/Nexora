#pragma once

#include <memory>

#include "RHI/IRHIDevice.h"

namespace Nexora
{

class VulkanCommandPool;
class VulkanDevice;
class VulkanInstance;

class VulkanRHIDevice final : public IRHIDevice
{
public:
    VulkanRHIDevice();
    ~VulkanRHIDevice() override;

    // core
    std::unique_ptr<IRHIViewport> createViewport(const ViewportSettings& settings) override;
    std::unique_ptr<IRHIShader>   createShader(const ShaderBytecode& bytecode) override;
    std::unique_ptr<IRHIPipeline> createPipeline(const PipelineSettings& settings) override;

    // getters
    const VulkanInstance&    instance() const;
    const VulkanDevice&      device() const;
    const VulkanCommandPool& commandPool() const;

private:
    std::unique_ptr<VulkanInstance>    m_instance;
    std::unique_ptr<VulkanDevice>      m_device;
    std::unique_ptr<VulkanCommandPool> m_commandPool;
};

}  // namespace Nexora
