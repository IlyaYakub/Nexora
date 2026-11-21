#include "RHI/Vulkan/Public/VulkanInstance.h"

#include "RHI/Vulkan/Public/VulkanSurface.h"
#include "RHI/Vulkan/Public/VulkanUtility.h"

#include <vulkan/vulkan.h>

namespace
{

#if defined(NDEBUG)
constexpr bool c_isValidationEnabled = false;
#else
constexpr bool c_isValidationEnabled = true;
#endif

constexpr const char* c_validationLayerName = "VK_LAYER_KHRONOS_validation";

constexpr VkDebugUtilsMessageSeverityFlagsEXT c_validationMessageSeverity =
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

constexpr VkDebugUtilsMessageTypeFlagsEXT c_validationMessageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                                                    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                                                    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

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

template <typename Function>
Function loadInstanceFunction(VkInstance instance, const char* name)
{
    return reinterpret_cast<Function>(vkGetInstanceProcAddr(instance, name));
}

}  // namespace

namespace Nexora
{

VulkanInstance::VulkanInstance()
{
    // collect extensions ->
    auto extensions = VulkanSurface::requiredInstanceExtensions();
    if constexpr (c_isValidationEnabled)
    {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    // create ->
    const auto validationCallback = [](VkDebugUtilsMessageSeverityFlagBitsEXT severity,            //
                                        VkDebugUtilsMessageTypeFlagsEXT,                           //
                                        const VkDebugUtilsMessengerCallbackDataEXT* callbackData,  //
                                        void*) -> VkBool32                                         //
    {                                                                                              //
        logValidationMessage(severity, callbackData->pMessage);                                    //
        return VK_FALSE;                                                                           //
    };

    const VkDebugUtilsMessengerCreateInfoEXT messengerInfo{
        .sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .messageSeverity = c_validationMessageSeverity,
        .messageType     = c_validationMessageType,
        .pfnUserCallback = validationCallback,
    };
    const VkApplicationInfo applicationInfo{
        .sType            = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "Nexora",
        .pEngineName      = "Nexora",
        .apiVersion       = VK_API_VERSION_1_3,
    };
    const VkInstanceCreateInfo instanceInfo{
        .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext                   = c_isValidationEnabled ? &messengerInfo : nullptr,
        .pApplicationInfo        = &applicationInfo,
        .enabledLayerCount       = c_isValidationEnabled ? 1u : 0u,
        .ppEnabledLayerNames     = &c_validationLayerName,
        .enabledExtensionCount   = static_cast<uint32>(extensions.size()),
        .ppEnabledExtensionNames = extensions.data(),
    };
    NX_VULKAN_CHECK(vkCreateInstance(&instanceInfo, nullptr, &m_instance));

    // create debug messenger ->
    if constexpr (c_isValidationEnabled)
    {
        const auto createDebugMessenger =
            loadInstanceFunction<PFN_vkCreateDebugUtilsMessengerEXT>(m_instance, "vkCreateDebugUtilsMessengerEXT");
        NX_VULKAN_CHECK(createDebugMessenger(m_instance, &messengerInfo, nullptr, &m_debugMessenger));
    }
}

VulkanInstance::~VulkanInstance()
{
    // destroy debug messenger and instance ->
    if constexpr (c_isValidationEnabled)
    {
        const auto destroyDebugMessenger =
            loadInstanceFunction<PFN_vkDestroyDebugUtilsMessengerEXT>(m_instance, "vkDestroyDebugUtilsMessengerEXT");
        destroyDebugMessenger(m_instance, m_debugMessenger, nullptr);
    }
    vkDestroyInstance(m_instance, nullptr);
}

VkInstance_T* VulkanInstance::handle() const
{
    return m_instance;
}

}  // namespace Nexora
