#include "RHI/Vulkan/VulkanFence.h"

#include "RHI/Vulkan/VulkanRHIDevice.h"
#include "RHI/Vulkan/VulkanUtility.h"

#include <vulkan/vulkan.h>

namespace Nexora
{

VulkanFence::VulkanFence(const VulkanRHIDevice& device) : m_device(device)
{
    const VkFenceCreateInfo fenceInfo{
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT,
    };
    NX_VULKAN_CHECK(vkCreateFence(m_device.device(), &fenceInfo, nullptr, &m_fence));
}

VulkanFence::~VulkanFence()
{
    vkDestroyFence(m_device.device(), m_fence, nullptr);
}

void VulkanFence::wait() const
{
    NX_VULKAN_CHECK(vkWaitForFences(m_device.device(), 1, &m_fence, VK_TRUE, UINT64_MAX));
}

void VulkanFence::reset() const
{
    NX_VULKAN_CHECK(vkResetFences(m_device.device(), 1, &m_fence));
}

VkFence_T* VulkanFence::handle() const
{
    return m_fence;
}

}  // namespace Nexora
