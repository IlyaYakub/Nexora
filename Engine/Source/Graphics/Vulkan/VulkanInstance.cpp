#include "Graphics/Vulkan/VulkanInstance.h"

#include "EngineConfig.h"
#include "Graphics/Vulkan/VulkanUtility.h"
#include "Log/Log.h"

using namespace Nexora;

DEFINE_LOG_CATEGORY_STATIC(LogVulkanInstance)

namespace
{

constexpr const char* ValidationLayerName = "VK_LAYER_KHRONOS_validation";
constexpr uint32_t EngineVersion          = VK_MAKE_API_VERSION(0, ENGINE_VERSION_MAJOR, ENGINE_VERSION_MINOR, ENGINE_VERSION_PATCH);

bool isLayerAvailable(const char* layerName)
{
    const std::vector<VkLayerProperties> layers = vkEnumerate<VkLayerProperties>(vkEnumerateInstanceLayerProperties);
    return std::ranges::any_of(
        layers, [layerName](const VkLayerProperties& layer) { return std::strcmp(layer.layerName, layerName) == 0; });
}

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT /*types*/,
    const VkDebugUtilsMessengerCallbackDataEXT* data, void* /*userData*/)
{
    if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
    {
        NX_LOG(LogVulkanInstance, Error, "{}", data->pMessage);
    }
    else
    {
        NX_LOG(LogVulkanInstance, Warning, "{}", data->pMessage);
    }

    return VK_FALSE;
}

VkDebugUtilsMessengerCreateInfoEXT makeDebugMessengerInfo()
{
    return {
        .sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        .pfnUserCallback = debugCallback,
    };
}

}

VulkanInstance::VulkanInstance(const VulkanInstanceSettings& settings)
{
    // loader
    VK_CHECK(volkInitialize());

    // validation
    bool enableValidation = settings.enableValidation;
    if (enableValidation && !isLayerAvailable(ValidationLayerName))
    {
        NX_LOG(LogVulkanInstance, Warning, "{} is not available, validation is disabled", ValidationLayerName);
        enableValidation = false;
    }

    // layers and extensions
    std::vector<const char*> extensions = settings.extensions;
    std::vector<const char*> layers;
    if (enableValidation)
    {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        layers.push_back(ValidationLayerName);
    }

    // instance
    const VkApplicationInfo applicationInfo{
        .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName   = "Nexora Engine",
        .applicationVersion = EngineVersion,
        .pEngineName        = "Nexora Engine",
        .engineVersion      = EngineVersion,
        .apiVersion         = VK_API_VERSION_1_3,
    };

    const VkDebugUtilsMessengerCreateInfoEXT debugMessengerInfo = makeDebugMessengerInfo();

    const VkInstanceCreateInfo createInfo{
        .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext                   = enableValidation ? &debugMessengerInfo : nullptr,
        .pApplicationInfo        = &applicationInfo,
        .enabledLayerCount       = static_cast<uint32_t>(layers.size()),
        .ppEnabledLayerNames     = layers.data(),
        .enabledExtensionCount   = static_cast<uint32_t>(extensions.size()),
        .ppEnabledExtensionNames = extensions.data(),
    };
    VK_CHECK(vkCreateInstance(&createInfo, nullptr, &m_instance));
    volkLoadInstance(m_instance);

    // debug messenger
    if (enableValidation)
    {
        VK_CHECK(vkCreateDebugUtilsMessengerEXT(m_instance, &debugMessengerInfo, nullptr, &m_debugMessenger));
    }

    NX_LOG(LogVulkanInstance, Display, "Vulkan instance created, validation: {}", isValidationEnabled() ? "on" : "off");
}

VulkanInstance::~VulkanInstance()
{
    if (m_debugMessenger)
    {
        vkDestroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger, nullptr);
    }

    vkDestroyInstance(m_instance, nullptr);
}
