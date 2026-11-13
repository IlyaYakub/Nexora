#pragma once

#include <utility>

#include "Log/Log.h"

DEFINE_LOG_CATEGORY_STATIC(LogVulkan);

#define NX_VULKAN_CHECK(call)                                                                          \
    do                                                                                                 \
    {                                                                                                  \
        const VkResult result = call;                                                                  \
        if (result != VK_SUCCESS)                                                                      \
        {                                                                                              \
            NX_LOG(LogVulkan, Fatal, "{} failed with VkResult {}", #call, std::to_underlying(result)); \
        }                                                                                              \
    } while (0)
