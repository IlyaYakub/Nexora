#include "RHI/Vulkan/VulkanRHIDevice.h"

#include "RHI/Vulkan/Command/VulkanCommandPool.h"
#include "RHI/Vulkan/Device/VulkanDevice.h"
#include "RHI/Vulkan/Device/VulkanInstance.h"
#include "RHI/Vulkan/Device/VulkanQueue.h"
#include "RHI/Vulkan/Presentation/VulkanViewport.h"

namespace Nexora
{

VulkanRHIDevice::VulkanRHIDevice()
{
    m_instance    = std::make_unique<VulkanInstance>();
    m_device      = std::make_unique<VulkanDevice>(*m_instance);
    m_commandPool = std::make_unique<VulkanCommandPool>(*m_device, m_device->graphicsQueue().familyIndex());
}

VulkanRHIDevice::~VulkanRHIDevice() = default;

std::unique_ptr<IRHIViewport> VulkanRHIDevice::createViewport(void* nativeWindowHandle, int32 width, int32 height)
{
    return std::make_unique<VulkanViewport>(*this, nativeWindowHandle, width, height);
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
