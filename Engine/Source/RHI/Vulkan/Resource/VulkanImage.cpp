#include "RHI/Vulkan/Resource/VulkanImage.h"

#include "RHI/Vulkan/Device/VulkanDevice.h"

#include <vulkan/vulkan.h>

namespace Nexora
{

VulkanImage::VulkanImage(const VulkanDevice& device, const VulkanImageSettings& settings)
    : m_device(device),           //
      m_image(settings.image),    //
      m_view(settings.view),      //
      m_width(settings.width),    //
      m_height(settings.height),  //
      m_type(settings.type)
{
}

VulkanImage::~VulkanImage()
{
    const VkDevice logicalDevice = m_device.logicalDevice();

    vkDestroyImageView(logicalDevice, m_view, nullptr);
    if (m_type != VulkanImageType::Swapchain)
    {
        vkDestroyImage(logicalDevice, m_image, nullptr);
    }
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

VulkanResourceState VulkanImage::state() const
{
    return m_state;
}

void VulkanImage::setState(VulkanResourceState state)
{
    m_state = state;
}

}  // namespace Nexora
