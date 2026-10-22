#include <format>
#include <source_location>
#include <string>

#include <gtest/gtest.h>

#include "Log/Log.h"

using namespace Nexora;

DEFINE_LOG_CATEGORY_STATIC(LogTest)

TEST(LogTest, MessageShouldBeFormatted)
{
    const auto location = std::source_location::current();
    const std::string withLocation = std::format("[LogTest] [{}:{}] Hello", location.function_name(), location.line());

    EXPECT_EQ(formatLogMessage(LogTest, "Hello", false, location), "[LogTest] Hello");
    EXPECT_EQ(formatLogMessage(LogTest, "Hello", true, location), withLocation);
}

TEST(LogDeathTest, FatalShouldAbortProgram)
{
    EXPECT_DEATH(NX_LOG(LogTest, Fatal, "Fatal message"), "");
}
