#pragma once

#include <memory>
#include <vector>

#include "Core/Types.h"
#include "RHI/IRHIViewport.h"
#include "RHI/Vulkan/VulkanSurface.h"

struct VkSwapchainKHR_T;
struct VkImage_T;
struct VkImageView_T;
struct VkSemaphore_T;

namespace Nexora
{

class VulkanFrame;
class VulkanRHIDevice;

class VulkanViewport final : public IRHIViewport
{
public:
    VulkanViewport(const VulkanRHIDevice& device, void* nativeWindowHandle, int32 width, int32 height);
    ~VulkanViewport() override;

    void resize(int32 width, int32 height) override;

private:
    void createSwapchain(int32 width, int32 height);
    void destroyImageViews();
    void destroyRenderFinishedSemaphores();

    const VulkanRHIDevice&                    m_device;
    VulkanSurface                             m_surface;
    VkSwapchainKHR_T*                         m_swapchain{nullptr};
    std::vector<VkImage_T*>                   m_images;
    std::vector<VkImageView_T*>               m_imageViews;
    std::vector<VkSemaphore_T*>               m_renderFinishedSemaphores;
    std::vector<std::unique_ptr<VulkanFrame>> m_frames;
};

}  // namespace Nexora
