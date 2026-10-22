#include "Graphics/Vulkan/VulkanPhysicalDevice.h"

#include "Graphics/Vulkan/VulkanInstance.h"
#include "Graphics/Vulkan/VulkanSurface.h"

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

bool supportsSwapchain(VkPhysicalDevice device)
{
    uint32_t count = 0;
    VK_CHECK(vkEnumerateDeviceExtensionProperties(device, nullptr, &count, nullptr));
    std::vector<VkExtensionProperties> extensions(count);
    VK_CHECK(vkEnumerateDeviceExtensionProperties(device, nullptr, &count, extensions.data()));

    return std::ranges::any_of(extensions,
        [](const VkExtensionProperties& extension) { return std::strcmp(extension.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0; });
}
}  // namespace

VulkanPhysicalDevice::VulkanPhysicalDevice(std::shared_ptr<VulkanInstance> instance, const VulkanSurface& surface)
    : m_instance(std::move(instance))
{
    uint32_t count = 0;
    VK_CHECK(vkEnumeratePhysicalDevices(m_instance->handle(), &count, nullptr));
    std::vector<VkPhysicalDevice> devices(count);
    VK_CHECK(vkEnumeratePhysicalDevices(m_instance->handle(), &count, devices.data()));

    for (VkPhysicalDevice device : devices)
    {
        const std::optional<VulkanQueueFamilies> families = findQueueFamilies(device, surface.handle());
        if (!families || !supportsSwapchain(device))
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
        NX_LOG(LogVulkanPhysicalDevice, Fatal, "No GPU with graphics, present and swapchain support");
    }

    NX_LOG(LogVulkanPhysicalDevice, Display, "Selected GPU: {} ({}), queue families: graphics {}, present {}", m_properties.deviceName,
        string_VkPhysicalDeviceType(m_properties.deviceType), m_queueFamilies.graphics, m_queueFamilies.present);
}
