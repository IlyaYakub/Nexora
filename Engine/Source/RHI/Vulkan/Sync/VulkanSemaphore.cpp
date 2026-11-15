#include "RHI/Vulkan/Sync/VulkanSemaphore.h"

#include "RHI/Vulkan/Device/VulkanDevice.h"
#include "RHI/Vulkan/VulkanUtility.h"

#include <vulkan/vulkan.h>

namespace Nexora
{

VulkanSemaphore::VulkanSemaphore(const VulkanDevice& device) : m_device(device)
{
    const VkSemaphoreCreateInfo semaphoreInfo{
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
    };
    NX_VULKAN_CHECK(vkCreateSemaphore(m_device.logicalDevice(), &semaphoreInfo, nullptr, &m_semaphore));
}

VulkanSemaphore::~VulkanSemaphore()
{
    vkDestroySemaphore(m_device.logicalDevice(), m_semaphore, nullptr);
}

VkSemaphore_T* VulkanSemaphore::handle() const
{
    return m_semaphore;
}

}  // namespace Nexora
