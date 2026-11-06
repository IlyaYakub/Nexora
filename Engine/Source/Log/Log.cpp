#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

using namespace Nexora;

namespace
{
constexpr const char* c_loggerName = "Nexora";
constexpr const char* c_logPattern = "[%H:%M:%S.%e] [%^%l%$] %v";

spdlog::level::level_enum toSpdLevel(LogVerbosity verbosity)
{
    switch (verbosity)
    {
        case LogVerbosity::NoLogging: return spdlog::level::off;
        case LogVerbosity::Display: return spdlog::level::info;
        case LogVerbosity::Warning: return spdlog::level::warn;
        case LogVerbosity::Error: return spdlog::level::err;
        case LogVerbosity::Log: return spdlog::level::debug;
        case LogVerbosity::Fatal: return spdlog::level::critical;
    }
    return spdlog::level::off;
}
}  // namespace

// pImpl

class Log::Impl
{
public:
    Impl()
    {
        const auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        m_logger = std::make_shared<spdlog::logger>(c_loggerName, consoleSink);
        m_logger->set_level(spdlog::level::debug);
        m_logger->set_pattern(c_logPattern);
    }

    void log(LogVerbosity verbosity, const std::string& message) const
    {
        m_logger->log(toSpdLevel(verbosity), message);

        if (verbosity == LogVerbosity::Fatal)
        {
            m_logger->flush();
            NX_PLATFORM_BREAK();
        }
    }

private:
    std::shared_ptr<spdlog::logger> m_logger;
};

// interface

Log::Log() : m_pImpl(std::make_unique<Impl>()) {}
Log::~Log() = default;

void Log::log(const LogCategory& category, LogVerbosity verbosity, const std::string& message, bool showLocation,
    const std::source_location location) const
{
    // Warnings and errors also show the name of the source file the message came from.
    const bool showFileName = verbosity == LogVerbosity::Warning || verbosity == LogVerbosity::Error;

    std::string fmtMsg;
    if (showLocation)
    {
        fmtMsg = std::format("[{}] [{}:{}] {}", category.name(), location.function_name(), location.line(), message);
    }
    else if (showFileName)
    {
        fmtMsg = std::format("[{}] [{}] {}", category.name(), fileNameFromPath(location.file_name()), message);
    }
    else
    {
        fmtMsg = std::format("[{}] {}", category.name(), message);
    }

    m_pImpl->log(verbosity, fmtMsg);
}
