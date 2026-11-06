#include "Graphics/Vulkan/VulkanContext.h"

#include "Log/Log.h"

#include <cstring>
#include <vector>

using namespace Nexora;

DEFINE_LOG_CATEGORY_STATIC(LogVulkanContext)

namespace
{

constexpr uint32 c_apiVersion           = VK_API_VERSION_1_3;
constexpr const char* c_validationLayer = "VK_LAYER_KHRONOS_validation";

bool hasValidationLayer()
{
    uint32 count = 0;
    vkEnumerateInstanceLayerProperties(&count, nullptr);
    std::vector<VkLayerProperties> layers(count);
    vkEnumerateInstanceLayerProperties(&count, layers.data());

    for (const auto& layer : layers)
    {
        if (std::strcmp(layer.layerName, c_validationLayer) == 0) return true;
    }
    return false;
}

bool hasDebugUtilsExtension()
{
    uint32 count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
    std::vector<VkExtensionProperties> extensions(count);
    vkEnumerateInstanceExtensionProperties(nullptr, &count, extensions.data());

    for (const auto& extension : extensions)
    {
        if (std::strcmp(extension.extensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == 0) return true;
    }
    return false;
}

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT /*type*/,
    const VkDebugUtilsMessengerCallbackDataEXT* data, void* /*userData*/)
{
    switch (severity)
    {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: NX_LOG(LogVulkanContext, Error, "{}", data->pMessage); break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: NX_LOG(LogVulkanContext, Warning, "{}", data->pMessage); break;
        default: NX_LOG(LogVulkanContext, Log, "{}", data->pMessage); break;
    }
    return VK_FALSE;
}

}  // namespace

std::expected<std::unique_ptr<VulkanContext>, GraphicsError> VulkanContext::create(const GraphicsContextDesc& desc)
{
    if (volkInitialize() != VK_SUCCESS)
    {
        NX_LOG(LogVulkanContext, Error, "Failed to find the Vulkan loader");
        return std::unexpected(GraphicsError::LoaderNotFound);
    }

    std::unique_ptr<VulkanContext> context(new VulkanContext());

    if (auto result = context->createInstance(desc); !result) return std::unexpected(result.error());
    if (auto result = context->createDebugMessenger(); !result) return std::unexpected(result.error());

    return context;
}

std::expected<void, GraphicsError> VulkanContext::createInstance(const GraphicsContextDesc& desc)
{
    bool useValidation = desc.enableValidation;
    if (useValidation && (!hasValidationLayer() || !hasDebugUtilsExtension()))
    {
        NX_LOG(LogVulkanContext, Warning, "Validation was requested but {} or {} is not available. Continuing without it",
            c_validationLayer, VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        useValidation = false;
    }

    const VkApplicationInfo appInfo{
        .sType            = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "Nexora",
        .pEngineName      = "Nexora",
        .apiVersion       = c_apiVersion,
    };

    const char* layers[]     = {c_validationLayer};
    const char* extensions[] = {VK_EXT_DEBUG_UTILS_EXTENSION_NAME};

    const VkInstanceCreateInfo instanceInfo{
        .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo        = &appInfo,
        .enabledLayerCount       = useValidation ? 1u : 0u,
        .ppEnabledLayerNames     = useValidation ? layers : nullptr,
        .enabledExtensionCount   = useValidation ? 1u : 0u,
        .ppEnabledExtensionNames = useValidation ? extensions : nullptr,
    };

    const VkResult result = vkCreateInstance(&instanceInfo, nullptr, &m_instance);
    if (result != VK_SUCCESS)
    {
        NX_LOG(LogVulkanContext, Error, "vkCreateInstance failed with VkResult {}", static_cast<int>(result));
        return std::unexpected(GraphicsError::InstanceCreationFailed);
    }

    volkLoadInstance(m_instance);
    m_validationEnabled = useValidation;

    const uint32 version = volkGetInstanceVersion();
    NX_LOG(LogVulkanContext, Display, "Vulkan instance created. Instance version {}.{}.{}, validation {}", VK_VERSION_MAJOR(version),
        VK_VERSION_MINOR(version), VK_VERSION_PATCH(version), useValidation ? "on" : "off");
    return {};
}

std::expected<void, GraphicsError> VulkanContext::createDebugMessenger()
{
    if (!m_validationEnabled) return {};

    const VkDebugUtilsMessengerCreateInfoEXT messengerInfo{
        .sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        .pfnUserCallback = debugCallback,
    };

    const VkResult result = vkCreateDebugUtilsMessengerEXT(m_instance, &messengerInfo, nullptr, &m_debugMessenger);
    if (result != VK_SUCCESS)
    {
        NX_LOG(LogVulkanContext, Error, "vkCreateDebugUtilsMessengerEXT failed with VkResult {}", static_cast<int>(result));
        return std::unexpected(GraphicsError::DebugMessengerCreationFailed);
    }
    return {};
}

VulkanContext::~VulkanContext()
{
    if (m_debugMessenger != VK_NULL_HANDLE) vkDestroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger, nullptr);
    if (m_instance != VK_NULL_HANDLE)
    {
        vkDestroyInstance(m_instance, nullptr);
        NX_LOG(LogVulkanContext, Display, "Vulkan instance destroyed");
    }
}
