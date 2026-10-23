#include "Graphics/Vulkan/VulkanCommandBuffer.h"

#include "Graphics/Vulkan/VulkanCommandPool.h"
#include "Graphics/Vulkan/VulkanDevice.h"
#include "Graphics/Vulkan/VulkanUtility.h"

using namespace Nexora;

VulkanCommandBuffer::VulkanCommandBuffer(std::shared_ptr<VulkanCommandPool> commandPool) : m_commandPool(std::move(commandPool))
{
    const VkCommandBufferAllocateInfo allocateInfo{
        .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool        = m_commandPool->handle(),
        .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1,
    };
    VK_CHECK(vkAllocateCommandBuffers(m_commandPool->device()->handle(), &allocateInfo, &m_commandBuffer));
}

VulkanCommandBuffer::~VulkanCommandBuffer()
{
    vkFreeCommandBuffers(m_commandPool->device()->handle(), m_commandPool->handle(), 1, &m_commandBuffer);
}

void VulkanCommandBuffer::begin(VkCommandBufferUsageFlags flags)
{
    const VkCommandBufferBeginInfo beginInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = flags,
    };
    VK_CHECK(vkBeginCommandBuffer(m_commandBuffer, &beginInfo));
}

void VulkanCommandBuffer::end()
{
    VK_CHECK(vkEndCommandBuffer(m_commandBuffer));
}

void VulkanCommandBuffer::reset()
{
    VK_CHECK(vkResetCommandBuffer(m_commandBuffer, 0));
}
