#include "RHI/Vulkan/Public/VulkanQueue.h"

#include "RHI/Vulkan/Public/VulkanCommandBuffer.h"
#include "RHI/Vulkan/Public/VulkanSwapchain.h"
#include "RHI/Vulkan/Public/VulkanFence.h"
#include "RHI/Vulkan/Public/VulkanSemaphore.h"
#include "RHI/Vulkan/Public/VulkanUtility.h"

#include <vulkan/vulkan.h>

namespace Nexora
{

VulkanQueue::VulkanQueue(VkDevice_T* device, uint32 familyIndex) : m_familyIndex(familyIndex)
{
    vkGetDeviceQueue(device, m_familyIndex, 0, &m_queue);
}

void VulkanQueue::submit(const VulkanCommandBuffer& commandBuffer,
    const VulkanSemaphore&                          waitSemaphore,
    const VulkanSemaphore&                          signalSemaphore,
    const VulkanFence&                              fence) const
{
    const VkSemaphoreSubmitInfo waitInfo{
        .sType     = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
        .semaphore = waitSemaphore.handle(),
        .stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
    };
    const VkCommandBufferSubmitInfo commandBufferInfo{
        .sType         = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
        .commandBuffer = commandBuffer.handle(),
    };
    const VkSemaphoreSubmitInfo signalInfo{
        .sType     = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
        .semaphore = signalSemaphore.handle(),
        .stageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
    };
    const VkSubmitInfo2 submitInfo{
        .sType                    = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
        .waitSemaphoreInfoCount   = 1,
        .pWaitSemaphoreInfos      = &waitInfo,
        .commandBufferInfoCount   = 1,
        .pCommandBufferInfos      = &commandBufferInfo,
        .signalSemaphoreInfoCount = 1,
        .pSignalSemaphoreInfos    = &signalInfo,
    };
    NX_VULKAN_CHECK(vkQueueSubmit2(m_queue, 1, &submitInfo, fence.handle()));
}

void VulkanQueue::present(const VulkanSwapchain& swapchain, uint32 imageIndex, const VulkanSemaphore& waitSemaphore) const
{
    const auto swapchainHandle     = swapchain.handle();
    const auto waitSemaphoreHandle = waitSemaphore.handle();

    const VkPresentInfoKHR presentInfo{
        .sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores    = &waitSemaphoreHandle,
        .swapchainCount     = 1,
        .pSwapchains        = &swapchainHandle,
        .pImageIndices      = &imageIndex,
    };
    const auto result = vkQueuePresentKHR(m_queue, &presentInfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        return;
    }
    NX_VULKAN_CHECK(result);
}

VkQueue_T* VulkanQueue::handle() const
{
    return m_queue;
}

uint32 VulkanQueue::familyIndex() const
{
    return m_familyIndex;
}

}  // namespace Nexora
