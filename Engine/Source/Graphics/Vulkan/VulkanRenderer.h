#pragma once

#include <array>
#include <memory>

#include "Core/Utility.h"
#include "Graphics/Vulkan/VulkanFrame.h"

namespace Nexora
{

class VulkanDevice;
class VulkanImage;
class VulkanSwapchain;

class VulkanRenderer final : public NonCopyable
{
public:
    static constexpr uint32_t FramesInFlight = 2;

    VulkanRenderer(std::shared_ptr<VulkanDevice> device, std::shared_ptr<VulkanSwapchain> swapchain);
    ~VulkanRenderer();

    void drawFrame();

    // graphics queue: wait -> commands -> signal + fence
    void submit(const std::shared_ptr<VulkanCommandBuffer>& commandBuffer, const std::shared_ptr<VulkanSemaphore>& wait,
        const std::shared_ptr<VulkanSemaphore>& signal, const std::shared_ptr<VulkanFence>& fence) const;

private:
    std::shared_ptr<VulkanDevice> m_device;
    std::shared_ptr<VulkanSwapchain> m_swapchain;
    std::array<std::shared_ptr<VulkanFrame>, FramesInFlight> m_frames;
    uint32_t m_frameIndex{0};

    void record(const std::shared_ptr<VulkanCommandBuffer>& commandBuffer, const std::shared_ptr<VulkanImage>& target);
};

}
