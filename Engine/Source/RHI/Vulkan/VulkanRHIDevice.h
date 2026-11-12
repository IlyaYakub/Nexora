#pragma once

#include "Core/Types.h"
#include "RHI/IRHIDevice.h"

struct VkInstance_T;
struct VkDebugUtilsMessengerEXT_T;
struct VkPhysicalDevice_T;
struct VkDevice_T;
struct VkQueue_T;

namespace Nexora
{

class VulkanRHIDevice final : public IRHIDevice
{
public:
    VulkanRHIDevice();
    ~VulkanRHIDevice() override;

    void beginFrame() override;
    void endFrame() override;

private:
    VkInstance_T* m_instance{nullptr};
    VkDebugUtilsMessengerEXT_T* m_debugMessenger{nullptr};
    VkPhysicalDevice_T* m_physicalDevice{nullptr};
    VkDevice_T* m_device{nullptr};
    VkQueue_T* m_graphicsQueue{nullptr};
    uint32 m_graphicsQueueFamilyIndex{0};
};

}  // namespace Nexora
