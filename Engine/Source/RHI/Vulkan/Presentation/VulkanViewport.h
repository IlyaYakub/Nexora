#pragma once

#include "Core/Types.h"
#include "RHI/IRHIViewport.h"
#include "RHI/Vulkan/Presentation/VulkanPresentationState.h"
#include "RHI/Vulkan/Presentation/VulkanSurface.h"

namespace Nexora
{

class VulkanRHIDevice;
class VulkanSemaphore;

class VulkanViewport final : public IRHIViewport
{
public:
    VulkanViewport(const VulkanRHIDevice& rhiDevice,           //
        void*                             nativeWindowHandle,  //
        int32                             width,               //
        int32                             height);
    ~VulkanViewport() override;

    // core
    void resize(int32 width, int32 height) override;
    void beginFrame() override;
    void endFrame() override;

    // getters
    IRHICommandList& commandList() override;
    IRHITexture&     backBuffer() override;

private:
    void   recreateSwapchain(int32 width, int32 height);
    uint32 acquireNextImage(const VulkanSemaphore& signalSemaphore);

    const VulkanRHIDevice&  m_rhiDevice;
    VulkanSurface           m_surface;
    VulkanPresentationState m_presentationState;
};

}  // namespace Nexora
