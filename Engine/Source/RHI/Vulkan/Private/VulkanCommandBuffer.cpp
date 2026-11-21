#include "RHI/Vulkan/Public/VulkanCommandBuffer.h"

#include "RHI/Vulkan/Public/VulkanCommandPool.h"
#include "RHI/Vulkan/Public/VulkanDevice.h"
#include "RHI/Vulkan/Public/VulkanUtility.h"

#include <vulkan/vulkan.h>

namespace Nexora
{

VulkanCommandBuffer::VulkanCommandBuffer(const VulkanDevice& device, const VulkanCommandPool& commandPool)
    : m_device(device),  //
      m_commandPool(commandPool)
{
    const VkCommandBufferAllocateInfo commandBufferInfo{
        .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool        = m_commandPool.handle(),
        .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1,
    };
    NX_VULKAN_CHECK(vkAllocateCommandBuffers(m_device.logicalDevice(), &commandBufferInfo, &m_commandBuffer));
}

VulkanCommandBuffer::~VulkanCommandBuffer()
{
    vkFreeCommandBuffers(m_device.logicalDevice(), m_commandPool.handle(), 1, &m_commandBuffer);
}

void VulkanCommandBuffer::begin() const
{
    const VkCommandBufferBeginInfo beginInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
    };
    NX_VULKAN_CHECK(vkResetCommandBuffer(m_commandBuffer, 0));
    NX_VULKAN_CHECK(vkBeginCommandBuffer(m_commandBuffer, &beginInfo));
}

void VulkanCommandBuffer::end() const
{
    NX_VULKAN_CHECK(vkEndCommandBuffer(m_commandBuffer));
}

VkCommandBuffer_T* VulkanCommandBuffer::handle() const
{
    return m_commandBuffer;
}

}  // namespace Nexora
