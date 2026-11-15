#pragma once

#include <memory>

#include "Core/Types.h"
#include "RHI/IRHIDevice.h"

namespace Nexora
{

class VulkanCommandPool;
class VulkanDevice;
class VulkanInstance;

class VulkanRHIDevice final : public IRHIDevice
{
public:
    VulkanRHIDevice();
    ~VulkanRHIDevice() override;

    // core
    std::unique_ptr<IRHIViewport> createViewport(void* nativeWindowHandle, int32 width, int32 height) override;

    // getters
    const VulkanInstance&    instance() const;
    const VulkanDevice&      device() const;
    const VulkanCommandPool& commandPool() const;

private:
    std::unique_ptr<VulkanInstance>    m_instance;
    std::unique_ptr<VulkanDevice>      m_device;
    std::unique_ptr<VulkanCommandPool> m_commandPool;
};

}  // namespace Nexora
