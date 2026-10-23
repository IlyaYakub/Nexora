#include "Log/Log.h"

#include <cstdlib>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

using namespace Nexora;

namespace
{

constexpr const char* LogPattern = "[%H:%M:%S.%e] [%^%l%$] %v";

spdlog::level::level_enum toSpdlogLevel(LogVerbosity verbosity)
{
    switch (verbosity)
    {
        case LogVerbosity::Display: return spdlog::level::info;
        case LogVerbosity::Warning: return spdlog::level::warn;
        case LogVerbosity::Error: return spdlog::level::err;
        case LogVerbosity::Fatal: return spdlog::level::critical;
        default: return spdlog::level::off;
    }
}

}

std::string Nexora::formatLogMessage(
    const LogCategory& category, std::string_view message, bool showLocation, const std::source_location& location)
{
    return showLocation ? std::format("[{}] [{}:{}] {}", category.name(), location.function_name(), location.line(), message)
                        : std::format("[{}] {}", category.name(), message);
}

Log& Log::getInstance()
{
    static Log instance;
    return instance;
}

Log::Log() : m_logger(std::make_unique<spdlog::logger>("console", std::make_shared<spdlog::sinks::stdout_color_sink_mt>()))
{
    m_logger->set_pattern(LogPattern);
}

Log::~Log() = default;

void Log::log(
    const LogCategory& category, LogVerbosity verbosity, std::string_view message, bool showLocation, std::source_location location) const
{
    if (verbosity == LogVerbosity::NoLogging) return;

    m_logger->log(toSpdlogLevel(verbosity), formatLogMessage(category, message, showLocation, location));

    if (verbosity == LogVerbosity::Fatal)
    {
        m_logger->flush();
        std::abort();
    }
}
