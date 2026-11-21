#pragma once

#include "Core/Types.h"
#include "Core/Utility.h"

struct VkCommandPool_T;

namespace Nexora
{

class VulkanDevice;

class VulkanCommandPool final : public NonCopyable
{
public:
    VulkanCommandPool(const VulkanDevice& device, uint32 queueFamilyIndex);
    ~VulkanCommandPool();

    // getters
    VkCommandPool_T* handle() const;

private:
    const VulkanDevice& m_device;
    VkCommandPool_T*    m_commandPool{nullptr};
};

}  // namespace Nexora
