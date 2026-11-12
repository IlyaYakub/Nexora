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

    std::unique_ptr<IRHIViewport> createViewport(void* nativeWindowHandle, int32 width, int32 height) override;

    void beginFrame() override;
    void endFrame() override;

    VkInstance_T* instance() const;
    VkPhysicalDevice_T* physicalDevice() const;
    VkDevice_T* device() const;
    uint32 graphicsQueueFamilyIndex() const;

private:
    VkInstance_T*               m_instance{nullptr};
    VkDebugUtilsMessengerEXT_T* m_debugMessenger{nullptr};
    VkPhysicalDevice_T*         m_physicalDevice{nullptr};
    VkDevice_T*                 m_device{nullptr};
    VkQueue_T*                  m_graphicsQueue{nullptr};
    uint32                      m_graphicsQueueFamilyIndex{0};
};

}  // namespace Nexora
