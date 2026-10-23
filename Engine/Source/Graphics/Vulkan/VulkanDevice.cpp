#include "Graphics/Vulkan/VulkanDevice.h"

#include "Graphics/Vulkan/VulkanInstance.h"
#include "Graphics/Vulkan/VulkanPhysicalDevice.h"
#include "Graphics/Vulkan/VulkanUtility.h"

using namespace Nexora;

VulkanDevice::VulkanDevice(std::shared_ptr<VulkanPhysicalDevice> physicalDevice) : m_physicalDevice(std::move(physicalDevice))
{
    const VulkanQueueFamilies& families = m_physicalDevice->queueFamilies();

    // one queue per unique family
    std::vector<uint32_t> uniqueFamilies{families.graphics};
    if (families.present != families.graphics)
    {
        uniqueFamilies.push_back(families.present);
    }

    const float priority = 1.0f;
    std::vector<VkDeviceQueueCreateInfo> queueInfos;
    for (const uint32_t family : uniqueFamilies)
    {
        queueInfos.push_back({
            .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = family,
            .queueCount       = 1,
            .pQueuePriorities = &priority,
        });
    }

    // 1.3 features
    VkPhysicalDeviceVulkan13Features features13{
        .sType            = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
        .synchronization2 = VK_TRUE,
        .dynamicRendering = VK_TRUE,
    };

    // device
    const char* extensions[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    const VkDeviceCreateInfo createInfo{
        .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext                   = &features13,
        .queueCreateInfoCount    = static_cast<uint32_t>(queueInfos.size()),
        .pQueueCreateInfos       = queueInfos.data(),
        .enabledExtensionCount   = static_cast<uint32_t>(std::size(extensions)),
        .ppEnabledExtensionNames = extensions,
    };
    VK_CHECK(vkCreateDevice(m_physicalDevice->handle(), &createInfo, nullptr, &m_device));
    volkLoadDevice(m_device);

    // queues
    vkGetDeviceQueue(m_device, families.graphics, 0, &m_graphicsQueue);
    vkGetDeviceQueue(m_device, families.present, 0, &m_presentQueue);

    // memory allocator
    VmaAllocatorCreateInfo allocatorInfo{
        .physicalDevice   = m_physicalDevice->handle(),
        .device           = m_device,
        .instance         = m_physicalDevice->instance()->handle(),
        .vulkanApiVersion = VK_API_VERSION_1_3,
    };

    // vulkan functions from volk
    VmaVulkanFunctions functions{};
    VK_CHECK(vmaImportVulkanFunctionsFromVolk(&allocatorInfo, &functions));
    allocatorInfo.pVulkanFunctions = &functions;

    // allocator
    VK_CHECK(vmaCreateAllocator(&allocatorInfo, &m_allocator));
}

VulkanDevice::~VulkanDevice()
{
    // allocator before device
    vmaDestroyAllocator(m_allocator);
    vkDestroyDevice(m_device, nullptr);
}
