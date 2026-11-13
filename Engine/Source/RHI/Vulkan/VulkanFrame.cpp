#include "RHI/Vulkan/VulkanFrame.h"

#include "RHI/Vulkan/VulkanRHIDevice.h"
#include "RHI/Vulkan/VulkanUtility.h"

#include <vulkan/vulkan.h>

namespace Nexora
{

VulkanFrame::VulkanFrame(const VulkanRHIDevice& device)
    : m_device(device),         //
      m_inFlightFence(device),  //
      m_imageAvailableSemaphore(device)
{
    const VkCommandBufferAllocateInfo commandBufferInfo{
        .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool        = m_device.commandPool(),
        .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1,
    };
    NX_VULKAN_CHECK(vkAllocateCommandBuffers(m_device.device(), &commandBufferInfo, &m_commandBuffer));
}

VulkanFrame::~VulkanFrame()
{
    vkFreeCommandBuffers(m_device.device(), m_device.commandPool(), 1, &m_commandBuffer);
}

VkCommandBuffer_T* VulkanFrame::commandBuffer() const
{
    return m_commandBuffer;
}

const VulkanFence& VulkanFrame::inFlightFence() const
{
    return m_inFlightFence;
}

const VulkanSemaphore& VulkanFrame::imageAvailableSemaphore() const
{
    return m_imageAvailableSemaphore;
}

}  // namespace Nexora
