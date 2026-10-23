#include "Graphics/Vulkan/VulkanImage.h"

#include "Graphics/Vulkan/VulkanDevice.h"
#include "Graphics/Vulkan/VulkanUtility.h"

using namespace Nexora;

VulkanImage::VulkanImage(std::shared_ptr<VulkanDevice> device, const VulkanImageSettings& settings)
    : m_device(std::move(device)), m_format(settings.format), m_extent(settings.extent), m_aspect(settings.aspect)
{
    // 2D, 1 mip, 1 layer
    const VkImageCreateInfo imageInfo{
        .sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType     = VK_IMAGE_TYPE_2D,
        .format        = m_format,
        .extent        = {m_extent.width, m_extent.height, 1},
        .mipLevels     = 1,
        .arrayLayers   = 1,
        .samples       = VK_SAMPLE_COUNT_1_BIT,
        .tiling        = VK_IMAGE_TILING_OPTIMAL,
        .usage         = settings.usage,
        .sharingMode   = VK_SHARING_MODE_EXCLUSIVE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    };

    // allocate with VMA
    const VmaAllocationCreateInfo allocationInfo{.usage = VMA_MEMORY_USAGE_AUTO};
    VK_CHECK(vmaCreateImage(m_device->allocator(), &imageInfo, &allocationInfo, &m_image, &m_allocation, nullptr));

    // view
    createView();
}

VulkanImage::VulkanImage(std::shared_ptr<VulkanDevice> device, VkImage image, VkFormat format, VkExtent2D extent)
    : m_device(std::move(device)), m_image(image), m_format(format), m_extent(extent)
{
    // view only, image is not ours
    createView();
}

VulkanImage::~VulkanImage()
{
    // view always ours
    vkDestroyImageView(m_device->handle(), m_view, nullptr);

    // image only if owned
    if (ownsImage())
    {
        vmaDestroyImage(m_device->allocator(), m_image, m_allocation);
    }
}

void VulkanImage::createView()
{
    const VkImageViewCreateInfo viewInfo{
        .sType            = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image            = m_image,
        .viewType         = VK_IMAGE_VIEW_TYPE_2D,
        .format           = m_format,
        .subresourceRange = {.aspectMask = m_aspect, .levelCount = 1, .layerCount = 1},
    };
    VK_CHECK(vkCreateImageView(m_device->handle(), &viewInfo, nullptr, &m_view));
}
