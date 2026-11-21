#pragma once

#include "Log/Log.h"
#include "RHI/Vulkan/Public/VulkanMapper.h"

DEFINE_LOG_CATEGORY_STATIC(LogVulkan);

#define NX_VULKAN_CHECK(call)                                                                               \
    do                                                                                                      \
    {                                                                                                       \
        const auto vulkanResult = call;                                                                     \
        if (vulkanResult < 0)                                                                               \
        {                                                                                                   \
            NX_LOG(LogVulkan, Fatal, "{} failed: {}", #call, Nexora::VulkanMapper::toString(vulkanResult)); \
        }                                                                                                   \
    } while (0)
