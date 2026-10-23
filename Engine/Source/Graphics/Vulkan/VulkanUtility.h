#pragma once

#include <cstdint>
#include <type_traits>
#include <vector>

#include <Volk/volk.h>
#include <vulkan/vk_enum_string_helper.h>

#include "Log/Log.h"

#ifndef NDEBUG
#define VK_CHECK(call)                                                                                                    \
    do                                                                                                                    \
    {                                                                                                                     \
        DEFINE_LOG_CATEGORY_STATIC(LogVulkan)                                                                             \
        const VkResult vkCheckResult = call;                                                                              \
        if (vkCheckResult != VK_SUCCESS)                                                                                  \
        {                                                                                                                 \
            NX_LOG(LogVulkan, Fatal, "{} failed: {} ({}:{})", #call, string_VkResult(vkCheckResult), __FILE__, __LINE__); \
        }                                                                                                                 \
    } while (false)
#else
#define VK_CHECK(call) call
#endif

namespace Nexora
{

template <typename T, typename Function, typename... Args>
std::vector<T> vkEnumerate(Function function, Args... args)
{
    uint32_t count = 0;
    std::vector<T> items;

    if constexpr (std::is_void_v<std::invoke_result_t<Function, Args..., uint32_t*, T*>>)
    {
        function(args..., &count, nullptr);
        items.resize(count);
        function(args..., &count, items.data());
    }
    else
    {
        VK_CHECK(function(args..., &count, nullptr));
        items.resize(count);
        VK_CHECK(function(args..., &count, items.data()));
    }

    return items;
}

}
