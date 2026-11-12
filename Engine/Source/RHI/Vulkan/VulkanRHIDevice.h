#pragma once

#include "RHI/IRHIDevice.h"

struct VkInstance_T;
struct VkDebugUtilsMessengerEXT_T;

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
};

}  // namespace Nexora
