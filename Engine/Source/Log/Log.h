#pragma once

#include "Core/Types.h"
#include "Core/Utility.h"

#include <concepts>
#include <format>
#include <memory>
#include <source_location>
#include <string_view>
#include <type_traits>

namespace Nexora
{

enum class LogVerbosity : uint8
{
    Fatal,
    Error,
    Warning,
    Display,
    Log
};

[[nodiscard]] constexpr std::string_view toString(LogVerbosity verbosity) noexcept
{
    switch (verbosity)
    {
        case LogVerbosity::Fatal: return "Fatal";
        case LogVerbosity::Error: return "Error";
        case LogVerbosity::Warning: return "Warning";
        case LogVerbosity::Display: return "Display";
        case LogVerbosity::Log: return "Log";
    }
    return "Unknown";
}

struct LogCategory
{
    std::string_view name;
};

constexpr LogVerbosity c_compileTimeVerbosity = LogVerbosity::Log;

class Log final : public NonCopyable
{
public:
    // getters
    static Log& getInstance();

    // core
    void log(const LogCategory&     category,   //
        LogVerbosity                verbosity,  //
        std::string_view            message,    //
        const std::source_location& location) const;

private:
    Log();
    ~Log();

    class Impl;
    std::unique_ptr<Impl> m_impl;
};

template <typename T>
concept ValidLogCategory = std::same_as<std::remove_cvref_t<T>, LogCategory>;

}  // namespace Nexora

#define DEFINE_LOG_CATEGORY_STATIC(categoryName)                       \
    namespace                                                          \
    {                                                                  \
    constexpr Nexora::LogCategory categoryName{.name = #categoryName}; \
    }

#define NX_LOG(categoryName, verbosity, formatString, ...)                                                                      \
    do                                                                                                                          \
    {                                                                                                                           \
        static_assert(Nexora::ValidLogCategory<decltype(categoryName)>, "Category must be declared with a log category macro"); \
        if constexpr (Nexora::LogVerbosity::verbosity <= Nexora::c_compileTimeVerbosity)                                        \
        {                                                                                                                       \
            Nexora::Log::getInstance().log(categoryName, Nexora::LogVerbosity::verbosity,                                       \
                std::format(formatString __VA_OPT__(, ) __VA_ARGS__), std::source_location::current());                         \
        }                                                                                                                       \
    } while (0)
