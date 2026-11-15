#pragma once

#include "Core/Utility.h"

struct VkInstance_T;
struct VkDebugUtilsMessengerEXT_T;

namespace Nexora
{

class VulkanInstance final : public NonCopyable
{
public:
    VulkanInstance();
    ~VulkanInstance();

    // getters
    VkInstance_T* handle() const;

private:
    VkInstance_T*               m_instance{nullptr};
    VkDebugUtilsMessengerEXT_T* m_debugMessenger{nullptr};
};

}  // namespace Nexora
