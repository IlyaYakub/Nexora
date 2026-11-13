#include "Log/Log.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

using namespace Nexora;

namespace
{

constexpr std::string_view c_logPattern    = "[%H:%M:%S.%e] %^%v%$";
constexpr std::string_view c_logDirectory  = "Saved/Logs";
constexpr std::string_view c_logFilePrefix = "Nexora";

[[nodiscard]] constexpr spdlog::level::level_enum toSpdlogLevel(LogVerbosity verbosity) noexcept
{
    switch (verbosity)
    {
        case LogVerbosity::Fatal: return spdlog::level::critical;
        case LogVerbosity::Error: return spdlog::level::err;
        case LogVerbosity::Warning: return spdlog::level::warn;
        case LogVerbosity::Display: return spdlog::level::info;
        case LogVerbosity::Log: return spdlog::level::debug;
    }
    return spdlog::level::info;
}

[[nodiscard]] constexpr bool shouldShowLocation(LogVerbosity verbosity) noexcept
{
    return verbosity <= LogVerbosity::Warning;
}

[[nodiscard]] constexpr std::string_view fileName(std::string_view path) noexcept
{
    const auto separator = path.find_last_of("/\\");
    return separator == std::string_view::npos ? path : path.substr(separator + 1);
}

[[nodiscard]] std::filesystem::path makeLogFilePath()
{
    const auto now = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());
    return std::filesystem::path(c_logDirectory) / std::format("{}-{:%Y.%m.%d-%H.%M.%S}.log", c_logFilePrefix, now);
}

}  // namespace

class Log::Impl
{
public:
    Impl()
    {
        // init console sink ->
        const auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        consoleSink->set_level(spdlog::level::info);

        // init file sink ->
        const auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(makeLogFilePath().string(), true);
        fileSink->set_level(spdlog::level::trace);

        // init logger ->
        m_logger = std::make_unique<spdlog::logger>("Nexora", spdlog::sinks_init_list{consoleSink, fileSink});
        m_logger->set_pattern(std::string(c_logPattern));
        m_logger->set_level(spdlog::level::trace);
        m_logger->flush_on(spdlog::level::warn);
    }

    void log(LogVerbosity verbosity, std::string_view message) const { m_logger->log(toSpdlogLevel(verbosity), message); }

private:
    std::unique_ptr<spdlog::logger> m_logger;
};

Log& Log::getInstance()
{
    static Log instance;
    return instance;
}

Log::Log() : m_impl(std::make_unique<Impl>()) {}

Log::~Log() = default;

void Log::log(const LogCategory& category,   //
    LogVerbosity                 verbosity,  //
    std::string_view             message,    //
    const std::source_location&  location) const
{
    std::string line = std::format("{}: {}: {}", category.name, toString(verbosity), message);
    if (shouldShowLocation(verbosity))
    {
        line += std::format(" ({}:{})", fileName(location.file_name()), location.line());
    }

    m_impl->log(verbosity, line);

    if (verbosity == LogVerbosity::Fatal)
    {
        NX_DEBUG_BREAK();
        std::abort();
    }
}
