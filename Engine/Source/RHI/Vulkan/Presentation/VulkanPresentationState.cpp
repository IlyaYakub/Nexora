#include "RHI/Vulkan/Presentation/VulkanPresentationState.h"

#include "RHI/Vulkan/Presentation/VulkanFrame.h"
#include "RHI/Vulkan/Presentation/VulkanSwapchain.h"
#include "RHI/Vulkan/Resource/VulkanImage.h"
#include "RHI/Vulkan/Sync/VulkanSemaphore.h"

namespace
{

constexpr Nexora::uint32 c_framesInFlightCount = 2;

}  // namespace

namespace Nexora
{

VulkanPresentationState::VulkanPresentationState(const VulkanDevice& device, const VulkanCommandPool& commandPool) : m_device(device)
{
    // create frames ->
    for (uint32 i = 0; i < c_framesInFlightCount; ++i)
    {
        m_frames.push_back(std::make_unique<VulkanFrame>(device, commandPool));
    }
}

VulkanPresentationState::~VulkanPresentationState() = default;

void VulkanPresentationState::advanceFrame()
{
    m_frameIndex = (m_frameIndex + 1) % c_framesInFlightCount;
}

const VulkanSwapchain& VulkanPresentationState::currentSwapchain() const
{
    return *m_swapchain;
}

uint32 VulkanPresentationState::imageIndex() const
{
    return m_imageIndex;
}

VulkanImage& VulkanPresentationState::currentImage() const
{
    return *m_swapchain->images()[m_imageIndex];
}

const VulkanSemaphore& VulkanPresentationState::currentRenderFinishedSemaphore() const
{
    return *m_renderFinishedSemaphores[m_imageIndex];
}

VulkanFrame& VulkanPresentationState::currentFrame() const
{
    return *m_frames[m_frameIndex];
}

void VulkanPresentationState::setSwapchain(std::unique_ptr<VulkanSwapchain> swapchain)
{
    m_swapchain = std::move(swapchain);

    // create render finished semaphores ->
    m_renderFinishedSemaphores.clear();
    for (uint32 i = 0; i < m_swapchain->images().size(); ++i)
    {
        m_renderFinishedSemaphores.push_back(std::make_unique<VulkanSemaphore>(m_device));
    }
}

void VulkanPresentationState::setImageIndex(uint32 imageIndex)
{
    m_imageIndex = imageIndex;
}

}  // namespace Nexora
