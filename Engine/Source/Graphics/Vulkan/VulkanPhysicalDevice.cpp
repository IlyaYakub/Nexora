#include "Graphics/Vulkan/VulkanPhysicalDevice.h"

#include "Graphics/Vulkan/VulkanInstance.h"
#include "Graphics/Vulkan/VulkanSurface.h"
#include "Graphics/Vulkan/VulkanUtility.h"

using namespace Nexora;

DEFINE_LOG_CATEGORY_STATIC(LogVulkanPhysicalDevice)

namespace
{

std::optional<VulkanQueueFamilies> findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    const std::vector<VkQueueFamilyProperties> families =
        vkEnumerate<VkQueueFamilyProperties>(vkGetPhysicalDeviceQueueFamilyProperties, device);

    std::optional<uint32_t> graphics;
    std::optional<uint32_t> present;
    for (uint32_t i = 0; i < families.size(); ++i)
    {
        VkBool32 presentSupported = VK_FALSE;
        VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupported));
        const bool graphicsSupported = families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT;

        // a family that can do both is the best choice
        if (graphicsSupported && presentSupported)
        {
            return VulkanQueueFamilies{i, i};
        }
        if (graphicsSupported && !graphics) graphics = i;
        if (presentSupported && !present) present = i;
    }

    if (graphics && present)
    {
        return VulkanQueueFamilies{*graphics, *present};
    }
    return std::nullopt;
}

bool supportsSwapchain(VkPhysicalDevice device)
{
    const std::vector<VkExtensionProperties> extensions =
        vkEnumerate<VkExtensionProperties>(vkEnumerateDeviceExtensionProperties, device, nullptr);

    return std::ranges::any_of(extensions,
        [](const VkExtensionProperties& extension) { return std::strcmp(extension.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0; });
}

}

VulkanPhysicalDevice::VulkanPhysicalDevice(std::shared_ptr<VulkanInstance> instance, const std::shared_ptr<VulkanSurface>& surface)
    : m_instance(std::move(instance))
{
    const std::vector<VkPhysicalDevice> devices = vkEnumerate<VkPhysicalDevice>(vkEnumeratePhysicalDevices, m_instance->handle());

    // the first suitable GPU, a discrete one wins
    for (VkPhysicalDevice device : devices)
    {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(device, &properties);

        // 1.3 for dynamic rendering and sync2
        const std::optional<VulkanQueueFamilies> families = findQueueFamilies(device, surface->handle());
        if (properties.apiVersion < VK_API_VERSION_1_3 || !families || !supportsSwapchain(device))
        {
            continue;
        }

        const bool discrete = properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
        if (m_physicalDevice && !discrete)
        {
            continue;
        }

        m_physicalDevice = device;
        m_properties     = properties;
        m_queueFamilies  = *families;
        if (discrete)
        {
            break;
        }
    }

    if (!m_physicalDevice)
    {
        NX_LOG(LogVulkanPhysicalDevice, Fatal, "No Vulkan 1.3 GPU with graphics, present and swapchain support");
    }

    NX_LOG(LogVulkanPhysicalDevice, Display, "Selected GPU: {} ({}), queue families: graphics {}, present {}", m_properties.deviceName,
        string_VkPhysicalDeviceType(m_properties.deviceType), m_queueFamilies.graphics, m_queueFamilies.present);
}
