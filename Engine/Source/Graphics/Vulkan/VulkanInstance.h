#pragma once

#include <vector>

#include <Volk/volk.h>

#include "Core/Utility.h"

namespace Nexora
{

struct VulkanInstanceSettings
{
    std::vector<const char*> extensions;
    bool enableValidation{false};
};

class VulkanInstance final : public NonCopyable
{
public:
    explicit VulkanInstance(const VulkanInstanceSettings& settings);
    ~VulkanInstance();

    bool isValidationEnabled() const { return m_debugMessenger != VK_NULL_HANDLE; }

    VkInstance handle() const { return m_instance; }

private:
    VkInstance m_instance{VK_NULL_HANDLE};
    VkDebugUtilsMessengerEXT m_debugMessenger{VK_NULL_HANDLE};
};

}
