#include "RHI/Vulkan/VulkanRHIDevice.h"

#include "Log/Log.h"

#include <VkBootstrap.h>

DEFINE_LOG_CATEGORY_STATIC(LogVulkan);

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

    NX_LOG(LogVulkan, Display, "Vulkan instance created");
}

VulkanRHIDevice::~VulkanRHIDevice()
{
    // destroy debug messenger and instance ->
    vkb::destroy_debug_utils_messenger(m_instance, m_debugMessenger);
    vkDestroyInstance(m_instance, nullptr);
}

void VulkanRHIDevice::beginFrame() {}

void VulkanRHIDevice::endFrame() {}

}  // namespace Nexora
