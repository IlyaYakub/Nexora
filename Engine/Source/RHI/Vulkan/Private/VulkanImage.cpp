#include "RHI/Vulkan/Public/VulkanImage.h"

#include "RHI/Vulkan/Public/VulkanDevice.h"
#include "RHI/Vulkan/Public/VulkanMapper.h"
#include "RHI/Vulkan/Public/VulkanUtility.h"

namespace Nexora
{

VulkanImage::VulkanImage(const VulkanDevice& device, const VulkanImageSettings& settings)
    : m_device(device),           //
      m_image(settings.image),    //
      m_width(settings.width),    //
      m_height(settings.height),  //
      m_format(settings.format)
{
    const VkImageViewCreateInfo viewInfo{
        .sType            = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image            = m_image,
        .viewType         = VK_IMAGE_VIEW_TYPE_2D,
        .format           = VulkanMapper::toVkFormat(m_format),
        .subresourceRange = {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .levelCount = 1, .layerCount = 1},
    };
    NX_VULKAN_CHECK(vkCreateImageView(m_device.logicalDevice(), &viewInfo, nullptr, &m_view));
}

VulkanImage::~VulkanImage()
{
    vkDestroyImageView(m_device.logicalDevice(), m_view, nullptr);
}

VkImage_T* VulkanImage::handle() const
{
    return m_image;
}

VkImageView_T* VulkanImage::view() const
{
    return m_view;
}

uint32 VulkanImage::width() const
{
    return m_width;
}

uint32 VulkanImage::height() const
{
    return m_height;
}

TextureFormat VulkanImage::format() const
{
    return m_format;
}

VulkanResourceState VulkanImage::state() const
{
    return m_state;
}

void VulkanImage::setState(VulkanResourceState state)
{
    m_state = state;
}

}  // namespace Nexora
