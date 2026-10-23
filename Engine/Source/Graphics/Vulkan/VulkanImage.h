#pragma once

#include <memory>

#include <Volk/volk.h>
#include <vma/vk_mem_alloc.h>

#include "Core/Utility.h"

namespace Nexora
{

class VulkanDevice;

struct VulkanImageSettings
{
    VkExtent2D extent{};
    VkFormat format{VK_FORMAT_UNDEFINED};
    VkImageUsageFlags usage{0};
    VkImageAspectFlags aspect{VK_IMAGE_ASPECT_COLOR_BIT};
};

// 2D image + view
// owned: VMA image, destroyed here
// external: swapchain image, only view destroyed here
class VulkanImage final : public NonCopyable
{
public:
    // owned
    VulkanImage(std::shared_ptr<VulkanDevice> device, const VulkanImageSettings& settings);
    // external
    VulkanImage(std::shared_ptr<VulkanDevice> device, VkImage image, VkFormat format, VkExtent2D extent);
    ~VulkanImage();

    // owned images have allocation
    bool ownsImage() const { return m_allocation != nullptr; }

    VkImage handle() const { return m_image; }
    VkImageView view() const { return m_view; }
    VkFormat format() const { return m_format; }
    VkExtent2D extent() const { return m_extent; }
    VkImageAspectFlags aspect() const { return m_aspect; }

    // layout after recorded commands
    VkImageLayout layout() const { return m_layout; }
    void setLayout(VkImageLayout layout) { m_layout = layout; }

private:
    std::shared_ptr<VulkanDevice> m_device;
    VkImage m_image{VK_NULL_HANDLE};
    VkImageView m_view{VK_NULL_HANDLE};
    VmaAllocation m_allocation{nullptr};
    VkFormat m_format{VK_FORMAT_UNDEFINED};
    VkExtent2D m_extent{};
    VkImageAspectFlags m_aspect{VK_IMAGE_ASPECT_COLOR_BIT};
    VkImageLayout m_layout{VK_IMAGE_LAYOUT_UNDEFINED};

    void createView();
};

}
