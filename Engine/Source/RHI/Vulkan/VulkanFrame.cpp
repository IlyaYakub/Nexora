#include "RHI/Vulkan/VulkanFrame.h"

#include "RHI/Vulkan/VulkanRHIDevice.h"
#include "RHI/Vulkan/VulkanUtility.h"

#include <vulkan/vulkan.h>

namespace Nexora
{

VulkanFrame::VulkanFrame(const VulkanRHIDevice& device) : m_device(device)
{
    const VkDevice      logicalDevice = m_device.device();
    const VkCommandPool commandPool   = m_device.commandPool();

    // allocate command buffer ->
    const VkCommandBufferAllocateInfo commandBufferInfo{
        .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool        = commandPool,
        .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1,
    };
    NX_VULKAN_CHECK(vkAllocateCommandBuffers(logicalDevice, &commandBufferInfo, &m_commandBuffer));

    // create sync objects ->
    const VkFenceCreateInfo fenceInfo{
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT,
    };
    NX_VULKAN_CHECK(vkCreateFence(logicalDevice, &fenceInfo, nullptr, &m_inFlightFence));

    const VkSemaphoreCreateInfo semaphoreInfo{
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
    };
    NX_VULKAN_CHECK(vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &m_imageAvailableSemaphore));
}

VulkanFrame::~VulkanFrame()
{
    const VkDevice      logicalDevice = m_device.device();
    const VkCommandPool commandPool   = m_device.commandPool();

    // destroy sync objects and free command buffer ->
    vkDestroySemaphore(logicalDevice, m_imageAvailableSemaphore, nullptr);
    vkDestroyFence(logicalDevice, m_inFlightFence, nullptr);
    vkFreeCommandBuffers(logicalDevice, commandPool, 1, &m_commandBuffer);
}

VkCommandBuffer_T* VulkanFrame::commandBuffer() const
{
    return m_commandBuffer;
}

VkFence_T* VulkanFrame::inFlightFence() const
{
    return m_inFlightFence;
}

VkSemaphore_T* VulkanFrame::imageAvailableSemaphore() const
{
    return m_imageAvailableSemaphore;
}

}  // namespace Nexora
