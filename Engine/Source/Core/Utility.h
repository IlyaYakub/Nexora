#pragma once

#if defined(_MSC_VER)
#define NX_PLATFORM_BREAK() __debugbreak()
#else
#define NX_PLATFORM_BREAK() __builtin_trap()
#endif

#include <string_view>

namespace Nexora
{

// Returns only the file name of a path, e.g. "C:\dir\Log.cpp" -> "Log.cpp". Handles both '/' and '\' separators.
constexpr std::string_view fileNameFromPath(std::string_view path)
{
    const auto separator = path.find_last_of("/\\");
    return separator == std::string_view::npos ? path : path.substr(separator + 1);
}

class NonCopyable
{
public:
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;

protected:
    NonCopyable() = default;
    ~NonCopyable() = default;
};

}  // namespace Nexora
