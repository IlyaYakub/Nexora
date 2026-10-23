#include "Graphics/Vulkan/VulkanSemaphore.h"

#include "Graphics/Vulkan/VulkanDevice.h"
#include "Graphics/Vulkan/VulkanUtility.h"

using namespace Nexora;

VulkanSemaphore::VulkanSemaphore(std::shared_ptr<VulkanDevice> device) : m_device(std::move(device))
{
    const VkSemaphoreCreateInfo createInfo{.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
    VK_CHECK(vkCreateSemaphore(m_device->handle(), &createInfo, nullptr, &m_semaphore));
}

VulkanSemaphore::~VulkanSemaphore()
{
    vkDestroySemaphore(m_device->handle(), m_semaphore, nullptr);
}
