#pragma once

#include "RHI/IRHIDevice.h"

namespace Nexora
{

class VulkanRHIDevice final : public IRHIDevice
{
public:
    VulkanRHIDevice();
    ~VulkanRHIDevice() override;

    void beginFrame() override;
    void endFrame() override;
};

}  // namespace Nexora
