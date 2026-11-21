#include "RHI/Vulkan/Public/VulkanFrame.h"

namespace Nexora
{

VulkanFrame::VulkanFrame(const VulkanDevice& device, const VulkanCommandPool& commandPool)
    : m_commandBuffer(device, commandPool),  //
      m_commandList(m_commandBuffer),        //
      m_inFlightFence(device),               //
      m_imageAvailableSemaphore(device)
{
}

const VulkanCommandBuffer& VulkanFrame::commandBuffer() const
{
    return m_commandBuffer;
}

VulkanCommandList& VulkanFrame::commandList()
{
    return m_commandList;
}

const VulkanFence& VulkanFrame::inFlightFence() const
{
    return m_inFlightFence;
}

const VulkanSemaphore& VulkanFrame::imageAvailableSemaphore() const
{
    return m_imageAvailableSemaphore;
}

}  // namespace Nexora
