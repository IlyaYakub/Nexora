#include "RHI/Vulkan/VulkanRHIDevice.h"

#include "Log/Log.h"
#include "RHI/Vulkan/VulkanUtility.h"
#include "RHI/Vulkan/VulkanViewport.h"

#include <VkBootstrap.h>

namespace
{

#if defined(NDEBUG)
constexpr bool c_isValidationEnabled = false;
#else
constexpr bool c_isValidationEnabled = true;
#endif

constexpr VkDebugUtilsMessageSeverityFlagsEXT c_validationMessageSeverity =
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

void logValidationMessage(VkDebugUtilsMessageSeverityFlagBitsEXT severity, const char* message)
{
    switch (severity)
    {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: NX_LOG(LogVulkan, Log, "{}", message); break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: NX_LOG(LogVulkan, Log, "{}", message); break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: NX_LOG(LogVulkan, Warning, "{}", message); break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: NX_LOG(LogVulkan, Error, "{}", message); break;
    }
}

}  // namespace

namespace Nexora
{

VulkanRHIDevice::VulkanRHIDevice()
{
    // create instance ->
    vkb::InstanceBuilder builder;
    builder
        .set_app_name("Nexora")     //
        .set_engine_name("Nexora")  //
        .require_api_version(1, 3, 0);

    if constexpr (c_isValidationEnabled)
    {
        builder
            .request_validation_layers()                                       //
            .set_debug_messenger_severity(c_validationMessageSeverity)         //
            .set_debug_callback(                                               //
                [](VkDebugUtilsMessageSeverityFlagBitsEXT severity,            //
                    VkDebugUtilsMessageTypeFlagsEXT,                           //
                    const VkDebugUtilsMessengerCallbackDataEXT* callbackData,  //
                    void*) -> VkBool32                                         //
                {                                                              //
                    logValidationMessage(severity, callbackData->pMessage);    //
                    return VK_FALSE;                                           //
                });
    }

    const auto instance = builder.build();
    if (!instance)
    {
        NX_LOG(LogVulkan, Fatal, "Failed to create Vulkan instance: {}", instance.error().message());
    }
    m_instance       = instance->instance;
    m_debugMessenger = instance->debug_messenger;

    // select physical device ->
    const VkPhysicalDeviceVulkan13Features features13{
        .sType            = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
        .synchronization2 = VK_TRUE,
        .dynamicRendering = VK_TRUE,
    };
    vkb::PhysicalDeviceSelector selector{*instance};
    selector
        .set_minimum_version(1, 3)                                   //
        .set_required_features_13(features13)                        //
        .prefer_gpu_device_type(vkb::PreferredDeviceType::discrete)  //
        .allow_any_gpu_device_type(false)                            //
        .defer_surface_initialization();

    const auto physicalDevice = selector.select();
    if (!physicalDevice)
    {
        NX_LOG(LogVulkan, Fatal, "Failed to select discrete GPU: {}", physicalDevice.error().message());
    }
    m_physicalDevice = physicalDevice->physical_device;

    // create device ->
    vkb::DeviceBuilder deviceBuilder{*physicalDevice};
    const auto         device = deviceBuilder.build();
    if (!device)
    {
        NX_LOG(LogVulkan, Fatal, "Failed to create Vulkan device: {}", device.error().message());
    }
    m_device                   = device->device;
    m_graphicsQueue            = device->get_queue(vkb::QueueType::graphics).value();
    m_graphicsQueueFamilyIndex = device->get_queue_index(vkb::QueueType::graphics).value();

    // create command pool ->
    const VkCommandPoolCreateInfo commandPoolInfo{
        .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = m_graphicsQueueFamilyIndex,
    };
    NX_VULKAN_CHECK(vkCreateCommandPool(m_device, &commandPoolInfo, nullptr, &m_commandPool));

    NX_LOG(LogVulkan, Display, "Vulkan device created: {}", physicalDevice->name);
}

VulkanRHIDevice::~VulkanRHIDevice()
{
    // destroy in reverse order ->
    vkDestroyCommandPool(m_device, m_commandPool, nullptr);
    vkDestroyDevice(m_device, nullptr);
    vkb::destroy_debug_utils_messenger(m_instance, m_debugMessenger);
    vkDestroyInstance(m_instance, nullptr);
}

std::unique_ptr<IRHIViewport> VulkanRHIDevice::createViewport(void* nativeWindowHandle, int32 width, int32 height)
{
    return std::make_unique<VulkanViewport>(*this, nativeWindowHandle, width, height);
}

void VulkanRHIDevice::beginFrame() {}

void VulkanRHIDevice::endFrame() {}

VkInstance_T* VulkanRHIDevice::instance() const
{
    return m_instance;
}

VkPhysicalDevice_T* VulkanRHIDevice::physicalDevice() const
{
    return m_physicalDevice;
}

VkDevice_T* VulkanRHIDevice::device() const
{
    return m_device;
}

uint32 VulkanRHIDevice::graphicsQueueFamilyIndex() const
{
    return m_graphicsQueueFamilyIndex;
}

VkCommandPool_T* VulkanRHIDevice::commandPool() const
{
    return m_commandPool;
}

}  // namespace Nexora
