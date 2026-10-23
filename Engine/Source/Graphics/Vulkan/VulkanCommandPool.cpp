#include "Graphics/Vulkan/VulkanCommandPool.h"

#include "Graphics/Vulkan/VulkanDevice.h"
#include "Graphics/Vulkan/VulkanUtility.h"

using namespace Nexora;

VulkanCommandPool::VulkanCommandPool(std::shared_ptr<VulkanDevice> device, uint32_t queueFamily, VkCommandPoolCreateFlags flags)
    : m_device(std::move(device))
{
    const VkCommandPoolCreateInfo createInfo{
        .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags            = flags,
        .queueFamilyIndex = queueFamily,
    };
    VK_CHECK(vkCreateCommandPool(m_device->handle(), &createInfo, nullptr, &m_commandPool));
}

VulkanCommandPool::~VulkanCommandPool()
{
    vkDestroyCommandPool(m_device->handle(), m_commandPool, nullptr);
}

void VulkanCommandPool::reset()
{
    VK_CHECK(vkResetCommandPool(m_device->handle(), m_commandPool, 0));
}
