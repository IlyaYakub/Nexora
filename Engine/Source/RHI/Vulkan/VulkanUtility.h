#pragma once

#include <utility>

#include "Log/Log.h"

DEFINE_LOG_CATEGORY_STATIC(LogVulkan);

#define NX_VULKAN_CHECK(call)                                                                                \
    do                                                                                                       \
    {                                                                                                        \
        const VkResult vulkanResult = call;                                                                  \
        if (vulkanResult < 0)                                                                                \
        {                                                                                                    \
            NX_LOG(LogVulkan, Fatal, "{} failed with VkResult {}", #call, std::to_underlying(vulkanResult)); \
        }                                                                                                    \
    } while (0)
