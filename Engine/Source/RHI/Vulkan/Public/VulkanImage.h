#pragma once

#include "Core/Types.h"
#include "RHI/IRHITexture.h"
#include "RHI/Vulkan/Public/VulkanResourceState.h"

struct VkImage_T;
struct VkImageView_T;

namespace Nexora
{

class VulkanDevice;

struct VulkanImageSettings
{
    VkImage_T*    image{nullptr};
    uint32        width{0};
    uint32        height{0};
    TextureFormat format{TextureFormat::Unknown};
};

class VulkanImage final : public IRHITexture
{
public:
    VulkanImage(const VulkanDevice& device, const VulkanImageSettings& settings);
    ~VulkanImage() override;

    // getters
    VkImage_T*          handle() const;
    VkImageView_T*      view() const;
    uint32              width() const;
    uint32              height() const;
    TextureFormat       format() const override;
    VulkanResourceState state() const;

    // setters
    void setState(VulkanResourceState state);

private:
    const VulkanDevice& m_device;
    VkImage_T*          m_image{nullptr};
    VkImageView_T*      m_view{nullptr};
    uint32              m_width{0};
    uint32              m_height{0};
    TextureFormat       m_format{TextureFormat::Unknown};
    VulkanResourceState m_state{VulkanResourceState::Unknown};
};

}  // namespace Nexora
