#pragma once

namespace Nexora
{
class NonCopyable
{
protected:
    NonCopyable() = default;
    ~NonCopyable() = default;

    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;

    NonCopyable(NonCopyable&&) = delete;
    NonCopyable& operator=(NonCopyable&&) = delete;
};
}  // namespace Nexora

#ifndef NDEBUG
#define VK_CHECK(call)                                                                                                    \
    {                                                                                                                     \
        DEFINE_LOG_CATEGORY_STATIC(LogVulkan) const VkResult vkCheckResult = call;                                        \
        if (vkCheckResult != VK_SUCCESS)                                                                                  \
        {                                                                                                                 \
            NX_LOG(LogVulkan, Fatal, "{} failed: {} ({}:{})", #call, string_VkResult(vkCheckResult), __FILE__, __LINE__); \
        }                                                                                                                 \
    }
#else
#define VK_CHECK(call) call
#endif
