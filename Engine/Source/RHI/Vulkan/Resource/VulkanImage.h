#pragma once

#include "Core/Types.h"
#include "RHI/IRHITexture.h"
#include "RHI/Vulkan/Sync/VulkanResourceState.h"

struct VkImage_T;
struct VkImageView_T;

namespace Nexora
{

class VulkanDevice;

enum class VulkanImageType : uint8
{
    Unknown,
    Swapchain,
};

struct VulkanImageSettings
{
    VkImage_T*      image{nullptr};
    VkImageView_T*  view{nullptr};
    uint32          width{0};
    uint32          height{0};
    VulkanImageType type{VulkanImageType::Unknown};
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
    VulkanResourceState state() const;

    // setters
    void setState(VulkanResourceState state);

private:
    const VulkanDevice& m_device;
    VkImage_T*          m_image{nullptr};
    VkImageView_T*      m_view{nullptr};
    uint32              m_width{0};
    uint32              m_height{0};
    VulkanImageType     m_type{VulkanImageType::Unknown};
    VulkanResourceState m_state{VulkanResourceState::Unknown};
};

}  // namespace Nexora
