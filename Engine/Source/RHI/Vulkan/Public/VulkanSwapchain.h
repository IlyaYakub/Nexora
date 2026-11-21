#pragma once

#include <memory>
#include <vector>

#include "Core/Types.h"
#include "Core/Utility.h"
#include "RHI/IRHITexture.h"

struct VkSurfaceKHR_T;
struct VkSwapchainKHR_T;

namespace Nexora
{

class VulkanDevice;
class VulkanImage;

struct VulkanSwapchainSettings
{
    VkSurfaceKHR_T*   surface{nullptr};
    int32             width{0};
    int32             height{0};
    TextureFormat     format{TextureFormat::Unknown};
    VkSwapchainKHR_T* oldSwapchain{nullptr};
};

class VulkanSwapchain final : public NonCopyable
{
public:
    VulkanSwapchain(const VulkanDevice& device, const VulkanSwapchainSettings& settings);
    ~VulkanSwapchain();

    // getters
    VkSwapchainKHR_T*                                handle() const;
    int32                                            width() const;
    int32                                            height() const;
    TextureFormat                                    format() const;
    const std::vector<std::unique_ptr<VulkanImage>>& images() const;

private:
    void checkPresentSupport(VkSurfaceKHR_T* surface) const;

    const VulkanDevice&                       m_device;
    TextureFormat                             m_format{TextureFormat::Unknown};
    VkSwapchainKHR_T*                         m_swapchain{nullptr};
    int32                                     m_width{0};
    int32                                     m_height{0};
    std::vector<std::unique_ptr<VulkanImage>> m_images;
};

}  // namespace Nexora
