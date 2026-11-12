#pragma once

#include "Core/Utility.h"

struct VkInstance_T;
struct VkSurfaceKHR_T;

namespace Nexora
{

class VulkanSurface final : public NonCopyable
{
public:
    VulkanSurface(VkInstance_T* instance, void* nativeWindowHandle);
    ~VulkanSurface();

    VkSurfaceKHR_T* handle() const;

private:
    VkInstance_T* m_instance{nullptr};
    VkSurfaceKHR_T* m_surface{nullptr};
};

}  // namespace Nexora
