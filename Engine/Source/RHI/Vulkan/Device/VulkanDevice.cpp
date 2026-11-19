#include "RHI/Vulkan/Device/VulkanDevice.h"

#include "RHI/Vulkan/Device/VulkanInstance.h"
#include "RHI/Vulkan/Device/VulkanQueue.h"
#include "RHI/Vulkan/VulkanUtility.h"

#include <vulkan/vulkan.h>

namespace
{

VkPhysicalDevice selectPhysicalDevice(VkInstance instance)
{
    Nexora::uint32 physicalDeviceCount = 0;
    NX_VULKAN_CHECK(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr));
    std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
    NX_VULKAN_CHECK(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data()));

    for (const auto physicalDevice : physicalDevices)
    {
        VkPhysicalDeviceProperties properties{};
        vkGetPhysicalDeviceProperties(physicalDevice, &properties);
        if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && properties.apiVersion >= VK_API_VERSION_1_3)
        {
            NX_LOG(LogVulkan, Display, "Vulkan device selected: {}", properties.deviceName);
            return physicalDevice;
        }
    }

    NX_LOG(LogVulkan, Fatal, "Failed to find a discrete GPU with Vulkan 1.3");
    return VK_NULL_HANDLE;
}

Nexora::uint32 findGraphicsQueueFamilyIndex(VkPhysicalDevice physicalDevice)
{
    Nexora::uint32 queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

    for (Nexora::uint32 i = 0; i < queueFamilyCount; ++i)
    {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            return i;
        }
    }

    NX_LOG(LogVulkan, Fatal, "Failed to find a graphics queue family");
    return 0;
}

}  // namespace

namespace Nexora
{

VulkanDevice::VulkanDevice(const VulkanInstance& instance) : m_physicalDevice(selectPhysicalDevice(instance.handle()))
{
    const auto graphicsQueueFamilyIndex = findGraphicsQueueFamilyIndex(m_physicalDevice);

    // create ->
    const float                   queuePriority = 1.0f;
    const VkDeviceQueueCreateInfo queueInfo{
        .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = graphicsQueueFamilyIndex,
        .queueCount       = 1,
        .pQueuePriorities = &queuePriority,
    };
    VkPhysicalDeviceVulkan11Features features11{
        .sType                = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES,
        .shaderDrawParameters = VK_TRUE,
    };
    const VkPhysicalDeviceVulkan13Features features13{
        .sType            = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
        .pNext            = &features11,
        .synchronization2 = VK_TRUE,
        .dynamicRendering = VK_TRUE,
    };
    const char* const        swapchainExtension = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
    const VkDeviceCreateInfo deviceInfo{
        .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext                   = &features13,
        .queueCreateInfoCount    = 1,
        .pQueueCreateInfos       = &queueInfo,
        .enabledExtensionCount   = 1,
        .ppEnabledExtensionNames = &swapchainExtension,
    };
    NX_VULKAN_CHECK(vkCreateDevice(m_physicalDevice, &deviceInfo, nullptr, &m_logicalDevice));

    m_graphicsQueue = std::make_unique<VulkanQueue>(m_logicalDevice, graphicsQueueFamilyIndex);
}

VulkanDevice::~VulkanDevice()
{
    vkDestroyDevice(m_logicalDevice, nullptr);
}

VkPhysicalDevice_T* VulkanDevice::physicalDevice() const
{
    return m_physicalDevice;
}

VkDevice_T* VulkanDevice::logicalDevice() const
{
    return m_logicalDevice;
}

const VulkanQueue& VulkanDevice::graphicsQueue() const
{
    return *m_graphicsQueue;
}

}  // namespace Nexora
