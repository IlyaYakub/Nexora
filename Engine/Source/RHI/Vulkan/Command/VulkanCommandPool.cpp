#include "RHI/Vulkan/Command/VulkanCommandPool.h"

#include "RHI/Vulkan/Device/VulkanDevice.h"
#include "RHI/Vulkan/VulkanUtility.h"

#include <vulkan/vulkan.h>

namespace Nexora
{

VulkanCommandPool::VulkanCommandPool(const VulkanDevice& device, uint32 queueFamilyIndex) : m_device(device)
{
    const VkCommandPoolCreateInfo commandPoolInfo{
        .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = queueFamilyIndex,
    };
    NX_VULKAN_CHECK(vkCreateCommandPool(m_device.logicalDevice(), &commandPoolInfo, nullptr, &m_commandPool));
}

VulkanCommandPool::~VulkanCommandPool()
{
    vkDestroyCommandPool(m_device.logicalDevice(), m_commandPool, nullptr);
}

VkCommandPool_T* VulkanCommandPool::handle() const
{
    return m_commandPool;
}

}  // namespace Nexora
