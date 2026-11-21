#pragma once

#include <memory>
#include <vector>

#include "Core/Types.h"
#include "Core/Utility.h"

namespace Nexora
{

class VulkanCommandPool;
class VulkanDevice;
class VulkanFrame;
class VulkanImage;
class VulkanSemaphore;
class VulkanSwapchain;

class VulkanPresentationState final : public NonCopyable
{
public:
    VulkanPresentationState(const VulkanDevice& device, const VulkanCommandPool& commandPool);
    ~VulkanPresentationState();

    // core
    void advanceFrame();

    // getters
    const VulkanSwapchain& currentSwapchain() const;
    uint32                 imageIndex() const;
    VulkanImage&           currentImage() const;
    const VulkanSemaphore& currentRenderFinishedSemaphore() const;
    VulkanFrame&           currentFrame() const;

    // setters
    void setSwapchain(std::unique_ptr<VulkanSwapchain> swapchain);
    void setImageIndex(uint32 imageIndex);

private:
    const VulkanDevice&                           m_device;
    std::unique_ptr<VulkanSwapchain>              m_swapchain;
    std::vector<std::unique_ptr<VulkanSemaphore>> m_renderFinishedSemaphores;
    std::vector<std::unique_ptr<VulkanFrame>>     m_frames;
    uint32                                        m_imageIndex{0};
    uint32                                        m_frameIndex{0};
};

}  // namespace Nexora
