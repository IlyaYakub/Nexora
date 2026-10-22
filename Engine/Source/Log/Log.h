#pragma once

#include <cstdint>
#include <format>
#include <memory>
#include <source_location>
#include <string>
#include <string_view>

#include "Core/Utility.h"

namespace spdlog
{
class logger;
}

namespace Nexora
{

enum class LogVerbosity : uint8_t
{
    NoLogging = 0,  // message is ignored
    Display,
    Warning,
    Error,
    Fatal  // message is printed, then the program is aborted
};

class LogCategory
{
public:
    explicit LogCategory(std::string name) : m_name(std::move(name)) {}
    const std::string& name() const { return m_name; }

private:
    std::string m_name;
};

// "[Category] message" or "[Category] [function:line] message"
std::string formatLogMessage(
    const LogCategory& category, std::string_view message, bool showLocation, const std::source_location& location);

class Log final : public NonCopyable
{
public:
    static Log& getInstance();

    void log(const LogCategory& category,  //
        LogVerbosity verbosity,            //
        std::string_view message,          //
        bool showLocation = false,         //
        std::source_location location = std::source_location::current()) const;

private:
    Log();
    ~Log();

    std::unique_ptr<spdlog::logger> m_logger;
};

}  // namespace Nexora

#define DEFINE_LOG_CATEGORY_STATIC(categoryName) static const Nexora::LogCategory categoryName(#categoryName);

#define NX_LOG_IMPL(categoryName, verbosity, showLocation, formatStr, ...) \
    Nexora::Log::getInstance().log(                                        \
        categoryName, Nexora::LogVerbosity::verbosity, std::format(formatStr __VA_OPT__(, ) __VA_ARGS__), showLocation)

#define NX_LOG(categoryName, verbosity, formatStr, ...) NX_LOG_IMPL(categoryName, verbosity, false, formatStr __VA_OPT__(, ) __VA_ARGS__)
#define NX_LOG_DEBUG(categoryName, verbosity, formatStr, ...) \
    NX_LOG_IMPL(categoryName, verbosity, true, formatStr __VA_OPT__(, ) __VA_ARGS__)
