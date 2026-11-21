#include "RHI/Vulkan/Public/VulkanRHIDevice.h"

#include "RHI/Vulkan/Public/VulkanCommandPool.h"
#include "RHI/Vulkan/Public/VulkanDevice.h"
#include "RHI/Vulkan/Public/VulkanInstance.h"
#include "RHI/Vulkan/Public/VulkanPipeline.h"
#include "RHI/Vulkan/Public/VulkanQueue.h"
#include "RHI/Vulkan/Public/VulkanViewport.h"
#include "RHI/Vulkan/Public/VulkanShader.h"

namespace Nexora
{

VulkanRHIDevice::VulkanRHIDevice()
{
    m_instance    = std::make_unique<VulkanInstance>();
    m_device      = std::make_unique<VulkanDevice>(*m_instance);
    m_commandPool = std::make_unique<VulkanCommandPool>(*m_device, m_device->graphicsQueue().familyIndex());
}

VulkanRHIDevice::~VulkanRHIDevice() = default;

std::unique_ptr<IRHIViewport> VulkanRHIDevice::createViewport(const ViewportSettings& settings)
{
    return std::make_unique<VulkanViewport>(*this, settings);
}

std::unique_ptr<IRHIShader> VulkanRHIDevice::createShader(const ShaderBytecode& bytecode)
{
    return std::make_unique<VulkanShader>(*m_device, bytecode);
}

std::unique_ptr<IRHIPipeline> VulkanRHIDevice::createPipeline(const PipelineSettings& settings)
{
    return std::make_unique<VulkanPipeline>(*m_device, settings);
}

const VulkanInstance& VulkanRHIDevice::instance() const
{
    return *m_instance;
}

const VulkanDevice& VulkanRHIDevice::device() const
{
    return *m_device;
}

const VulkanCommandPool& VulkanRHIDevice::commandPool() const
{
    return *m_commandPool;
}

}  // namespace Nexora
