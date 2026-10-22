#include "Graphics/Vulkan/VulkanPhysicalDevice.h"

using namespace Nexora;

DEFINE_LOG_CATEGORY_STATIC(LogVulkanPhysicalDevice)

namespace
{
std::optional<VulkanQueueFamilies> findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    uint32_t count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);
    std::vector<VkQueueFamilyProperties> families(count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, families.data());

    std::optional<uint32_t> graphics;
    std::optional<uint32_t> present;
    for (uint32_t i = 0; i < count; ++i)
    {
        VkBool32 presentSupported = VK_FALSE;
        VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupported));
        const bool graphicsSupported = families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT;

        if (graphicsSupported && presentSupported)
        {
            return VulkanQueueFamilies{i, i};
        }
        if (graphicsSupported && !graphics)
        {
            graphics = i;
        }
        if (presentSupported && !present)
        {
            present = i;
        }
    }

    if (graphics && present)
    {
        return VulkanQueueFamilies{*graphics, *present};
    }
    return std::nullopt;
}
}  // namespace

VulkanPhysicalDevice::VulkanPhysicalDevice(VkInstance instance, VkSurfaceKHR surface)
{
    uint32_t count = 0;
    VK_CHECK(vkEnumeratePhysicalDevices(instance, &count, nullptr));
    std::vector<VkPhysicalDevice> devices(count);
    VK_CHECK(vkEnumeratePhysicalDevices(instance, &count, devices.data()));

    for (VkPhysicalDevice device : devices)
    {
        const std::optional<VulkanQueueFamilies> families = findQueueFamilies(device, surface);
        if (!families)
        {
            continue;
        }

        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(device, &properties);
        const bool discrete = properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
        if (m_physicalDevice && !discrete)
        {
            continue;
        }

        m_physicalDevice = device;
        m_properties = properties;
        m_queueFamilies = *families;
        if (discrete)
        {
            break;
        }
    }

    if (!m_physicalDevice)
    {
        NX_LOG(LogVulkanPhysicalDevice, Fatal, "No GPU with graphics and present support");
    }

    NX_LOG(LogVulkanPhysicalDevice, Display, "Selected GPU: {} ({}), queue families: graphics {}, present {}", m_properties.deviceName,
        string_VkPhysicalDeviceType(m_properties.deviceType), m_queueFamilies.graphics, m_queueFamilies.present);
}
