#include "Graphics/Vulkan/VulkanFence.h"

#include "Graphics/Vulkan/VulkanDevice.h"
#include "Graphics/Vulkan/VulkanUtility.h"

using namespace Nexora;

VulkanFence::VulkanFence(std::shared_ptr<VulkanDevice> device, bool signaled) : m_device(std::move(device))
{
    const VkFenceCreateInfo createInfo{
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .flags = signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0u,
    };
    VK_CHECK(vkCreateFence(m_device->handle(), &createInfo, nullptr, &m_fence));
}

VulkanFence::~VulkanFence()
{
    vkDestroyFence(m_device->handle(), m_fence, nullptr);
}

void VulkanFence::wait() const
{
    VK_CHECK(vkWaitForFences(m_device->handle(), 1, &m_fence, VK_TRUE, UINT64_MAX));
}

void VulkanFence::reset()
{
    VK_CHECK(vkResetFences(m_device->handle(), 1, &m_fence));
}
