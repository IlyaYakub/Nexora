#pragma once

#include <memory>
#include <vector>

#include "Core/Types.h"
#include "RHI/IRHIViewport.h"
#include "RHI/Vulkan/VulkanSurface.h"

struct VkSemaphore_T;

namespace Nexora
{

class VulkanFrame;
class VulkanRHIDevice;
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

private:
    void createRenderFinishedSemaphores();
    void destroyRenderFinishedSemaphores();

    const VulkanRHIDevice&                    m_device;
    VulkanSurface                             m_surface;
    std::unique_ptr<VulkanSwapchain>          m_swapchain;
    std::vector<VkSemaphore_T*>               m_renderFinishedSemaphores;
    std::vector<std::unique_ptr<VulkanFrame>> m_frames;
};

}  // namespace Nexora
