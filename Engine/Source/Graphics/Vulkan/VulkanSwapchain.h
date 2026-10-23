#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <Volk/volk.h>

#include "Core/Utility.h"
#include "Graphics/Vulkan/VulkanImage.h"
#include "Graphics/Vulkan/VulkanSemaphore.h"

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

    // nullopt: needs recreate
    std::optional<uint32_t> acquireNextImage(const std::shared_ptr<VulkanSemaphore>& acquireSemaphore);
    // false: needs recreate
    bool present(uint32_t imageIndex);

    VkSwapchainKHR handle() const { return m_swapchain; }
    VkFormat format() const { return m_format; }
    VkExtent2D extent() const { return m_extent; }
    VkPresentModeKHR presentMode() const { return m_presentMode; }
    bool isVsync() const { return m_vsync; }
    uint32_t imageCount() const { return static_cast<uint32_t>(m_images.size()); }
    const std::shared_ptr<VulkanImage>& image(uint32_t index) const { return m_images[index]; }
    const std::shared_ptr<VulkanSemaphore>& presentSemaphore(uint32_t index) const { return m_presentSemaphores[index]; }

private:
    std::shared_ptr<VulkanDevice> m_device;
    std::shared_ptr<VulkanSurface> m_surface;

    VkSwapchainKHR m_swapchain{VK_NULL_HANDLE};
    VkFormat m_format{VK_FORMAT_UNDEFINED};
    VkExtent2D m_extent{};
    VkPresentModeKHR m_presentMode{VK_PRESENT_MODE_FIFO_KHR};
    bool m_vsync{true};
    std::vector<std::shared_ptr<VulkanImage>> m_images;
    // per image: busy until the image is acquired again
    std::vector<std::shared_ptr<VulkanSemaphore>> m_presentSemaphores;

    void create(VkExtent2D extent);
};

}
