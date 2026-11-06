#include "Log/Log.h"

#include <gtest/gtest.h>

using namespace Nexora;

// concepts

TEST(LogConceptTest, LogCategoryTypeShouldBeValid)
{
    static_assert(ValidLogCategory<LogCategory>);
    static_assert(ValidLogCategory<const LogCategory&>);
    static_assert(ValidLogCategory<std::string>);

    static_assert(!ValidLogCategory<int>);
}

TEST(LogConceptTest, LogMessageTypeShouldBeValid)
{
    static_assert(LoggableMessage<std::string>);
    static_assert(LoggableMessage<const std::string&>);
    static_assert(LoggableMessage<std::string_view>);
    static_assert(LoggableMessage<const char*>);

    static_assert(!LoggableMessage<int>);
    static_assert(!LoggableMessage<float>);
    static_assert(!LoggableMessage<bool>);
    static_assert(!LoggableMessage<void*>);
}

TEST(LogConceptTest, LogVerbosityShouldBeValid)
{
    static_assert(ValidVerbosityLevel<LogVerbosity::NoLogging>);
    static_assert(ValidVerbosityLevel<LogVerbosity::Display>);
    static_assert(ValidVerbosityLevel<LogVerbosity::Warning>);
    static_assert(ValidVerbosityLevel<LogVerbosity::Error>);
    static_assert(ValidVerbosityLevel<LogVerbosity::Log>);
    static_assert(ValidVerbosityLevel<LogVerbosity::Fatal>);
}