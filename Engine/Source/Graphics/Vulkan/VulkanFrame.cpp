#include "Graphics/Vulkan/VulkanFrame.h"

#include "Graphics/Vulkan/VulkanCommandPool.h"
#include "Graphics/Vulkan/VulkanDevice.h"
#include "Graphics/Vulkan/VulkanPhysicalDevice.h"

using namespace Nexora;

VulkanFrame::VulkanFrame(const std::shared_ptr<VulkanDevice>& device)
{
    // commands
    m_commandPool   = std::make_shared<VulkanCommandPool>(device, device->physicalDevice()->queueFamilies().graphics);
    m_commandBuffer = std::make_shared<VulkanCommandBuffer>(m_commandPool);

    // sync, fence signaled so first wait passes
    m_acquireSemaphore = std::make_shared<VulkanSemaphore>(device);
    m_fence            = std::make_shared<VulkanFence>(device, true);
}
