#pragma once

#include <memory>

#include "Core/Utility.h"

struct VkPhysicalDevice_T;
struct VkDevice_T;

namespace Nexora
{

class VulkanInstance;
class VulkanQueue;

class VulkanDevice final : public NonCopyable
{
public:
    explicit VulkanDevice(const VulkanInstance& instance);
    ~VulkanDevice();

    // getters
    VkPhysicalDevice_T* physicalDevice() const;
    VkDevice_T*         logicalDevice() const;
    const VulkanQueue&  graphicsQueue() const;

private:
    VkPhysicalDevice_T*          m_physicalDevice{nullptr};
    VkDevice_T*                  m_logicalDevice{nullptr};
    std::unique_ptr<VulkanQueue> m_graphicsQueue;
};

}  // namespace Nexora
