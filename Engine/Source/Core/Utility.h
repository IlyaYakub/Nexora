#pragma once

namespace Nexora
{

class NonCopyable
{
protected:
    NonCopyable()  = default;
    ~NonCopyable() = default;

    NonCopyable(const NonCopyable&)            = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
    NonCopyable(NonCopyable&&)                 = delete;
    NonCopyable& operator=(NonCopyable&&)      = delete;
};

}  // namespace Nexora

#if defined(_MSC_VER)
#define NX_DEBUG_BREAK() __debugbreak()
#else
#include <csignal>
#define NX_DEBUG_BREAK() std::raise(SIGTRAP)
#endif
