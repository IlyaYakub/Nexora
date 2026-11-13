#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "Core/Types.h"
#include "RHI/IRHIViewport.h"
#include "RHI/Vulkan/VulkanSurface.h"

namespace Nexora
{

class VulkanFrame;
class VulkanRHIDevice;
class VulkanSemaphore;
class VulkanSwapchain;

class VulkanViewport final : public IRHIViewport
{
public:
    VulkanViewport(const VulkanRHIDevice& device,              //
        void*                             nativeWindowHandle,  //
        int32                             width,               //
        int32                             height);
    ~VulkanViewport() override;

    // core
    void resize(int32 width, int32 height) override;
    void beginFrame() override;
    void endFrame() override;

private:
    void createFrames();
    void createRenderFinishedSemaphores();

    const VulkanRHIDevice&                        m_device;
    VulkanSurface                                 m_surface;
    std::unique_ptr<VulkanSwapchain>              m_swapchain;
    std::vector<std::unique_ptr<VulkanSemaphore>> m_renderFinishedSemaphores;
    std::vector<std::unique_ptr<VulkanFrame>>     m_frames;
    uint32                                        m_frameIndex{0};
    std::optional<uint32>                         m_imageIndex;
};

}  // namespace Nexora
