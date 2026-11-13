#pragma once

#include <optional>
#include <vector>

#include "Core/Types.h"
#include "Core/Utility.h"

struct VkSurfaceKHR_T;
struct VkSwapchainKHR_T;
struct VkImage_T;
struct VkImageView_T;
struct VkSemaphore_T;
struct VkQueue_T;

namespace Nexora
{

class VulkanRHIDevice;

class VulkanSwapchain final : public NonCopyable
{
public:
    VulkanSwapchain(const VulkanRHIDevice& device,   //
        VkSurfaceKHR_T*                    surface,  //
        int32                              width,    //
        int32                              height,   //
        VkSwapchainKHR_T*                  oldSwapchain = nullptr);
    ~VulkanSwapchain();

    // core
    std::optional<uint32> acquireNextImage(VkSemaphore_T* signalSemaphore);
    void                  present(VkQueue_T* queue, uint32 imageIndex, VkSemaphore_T* waitSemaphore);

    // getters
    VkSwapchainKHR_T*                  handle() const;
    uint32                             imageCount() const;
    const std::vector<VkImage_T*>&     images() const;
    const std::vector<VkImageView_T*>& imageViews() const;

private:
    void checkPresentSupport(VkSurfaceKHR_T* surface) const;

    const VulkanRHIDevice&      m_device;
    VkSwapchainKHR_T*           m_swapchain{nullptr};
    std::vector<VkImage_T*>     m_images;
    std::vector<VkImageView_T*> m_imageViews;
};

}  // namespace Nexora
