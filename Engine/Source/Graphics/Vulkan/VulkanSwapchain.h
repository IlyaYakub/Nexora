#pragma once

#include <memory>
#include <vector>

#include <Volk/volk.h>

#include "Core/Utility.h"

namespace Nexora
{

class VulkanDevice;
class VulkanSurface;

class VulkanSwapchain final : public NonCopyable
{
public:
    VulkanSwapchain(std::shared_ptr<VulkanDevice> device, std::shared_ptr<VulkanSurface> surface, VkExtent2D extent, bool vsync);
    ~VulkanSwapchain();

    void recreate(VkExtent2D extent);
    void setVsync(bool vsync);

    VkSwapchainKHR handle() const { return m_swapchain; }
    VkFormat format() const { return m_format; }
    VkExtent2D extent() const { return m_extent; }
    VkPresentModeKHR presentMode() const { return m_presentMode; }
    bool isVsync() const { return m_vsync; }
    const std::vector<VkImage>& images() const { return m_images; }
    const std::vector<VkImageView>& imageViews() const { return m_imageViews; }

private:
    std::shared_ptr<VulkanDevice> m_device;
    std::shared_ptr<VulkanSurface> m_surface;

    VkSwapchainKHR m_swapchain{VK_NULL_HANDLE};
    VkFormat m_format{VK_FORMAT_UNDEFINED};
    VkExtent2D m_extent{};
    VkPresentModeKHR m_presentMode{VK_PRESENT_MODE_FIFO_KHR};
    bool m_vsync{true};
    std::vector<VkImage> m_images;
    std::vector<VkImageView> m_imageViews;

    void create(VkExtent2D extent);
    void destroyImageViews();
};

}  // namespace Nexora
