#include "RHI/Vulkan/Public/VulkanSurface.h"

#if defined(_WIN32)
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include <vulkan/vulkan.h>

#include "RHI/Vulkan/Public/VulkanUtility.h"

namespace
{

#if defined(_WIN32)
VkSurfaceKHR createPlatformSurface(VkInstance instance, void* nativeWindowHandle)
{
    const VkWin32SurfaceCreateInfoKHR surfaceInfo{
        .sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        .hinstance = GetModuleHandleW(nullptr),
        .hwnd      = static_cast<HWND>(nativeWindowHandle),
    };

    VkSurfaceKHR surface = VK_NULL_HANDLE;
    NX_VULKAN_CHECK(vkCreateWin32SurfaceKHR(instance, &surfaceInfo, nullptr, &surface));
    return surface;
}
#endif

}  // namespace

namespace Nexora
{

VulkanSurface::VulkanSurface(VkInstance_T* instance, void* nativeWindowHandle)
    : m_instance(instance),  //
      m_surface(createPlatformSurface(instance, nativeWindowHandle))
{
}

VulkanSurface::~VulkanSurface()
{
    vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
}

std::vector<const char*> VulkanSurface::requiredInstanceExtensions()
{
#if defined(_WIN32)
    return {VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME};
#endif
}

VkSurfaceKHR_T* VulkanSurface::handle() const
{
    return m_surface;
}

}  // namespace Nexora
