#include "Graphics/Vulkan/VulkanDevice.h"

#include "Graphics/Vulkan/VulkanPhysicalDevice.h"

using namespace Nexora;

VulkanDevice::VulkanDevice(const VulkanPhysicalDevice& physicalDevice)
{
    const VulkanQueueFamilies& families = physicalDevice.queueFamilies();
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
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = family,
            .queueCount = 1,
            .pQueuePriorities = &priority,
        });
    }

    const VkDeviceCreateInfo createInfo{
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount = static_cast<uint32_t>(queueInfos.size()),
        .pQueueCreateInfos = queueInfos.data(),
    };

    VK_CHECK(vkCreateDevice(physicalDevice.handle(), &createInfo, nullptr, &m_device));
    volkLoadDevice(m_device);

    vkGetDeviceQueue(m_device, families.graphics, 0, &m_graphicsQueue);
    vkGetDeviceQueue(m_device, families.present, 0, &m_presentQueue);
}

VulkanDevice::~VulkanDevice()
{
    vkDestroyDevice(m_device, nullptr);
}
