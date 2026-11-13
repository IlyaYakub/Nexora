#include "RHI/Vulkan/VulkanSemaphore.h"

#include "RHI/Vulkan/VulkanRHIDevice.h"
#include "RHI/Vulkan/VulkanUtility.h"

#include <vulkan/vulkan.h>

namespace Nexora
{

VulkanSemaphore::VulkanSemaphore(const VulkanRHIDevice& device) : m_device(device)
{
    const VkSemaphoreCreateInfo semaphoreInfo{
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
    };
    NX_VULKAN_CHECK(vkCreateSemaphore(m_device.device(), &semaphoreInfo, nullptr, &m_semaphore));
}

VulkanSemaphore::~VulkanSemaphore()
{
    vkDestroySemaphore(m_device.device(), m_semaphore, nullptr);
}

VkSemaphore_T* VulkanSemaphore::handle() const
{
    return m_semaphore;
}

}  // namespace Nexora
