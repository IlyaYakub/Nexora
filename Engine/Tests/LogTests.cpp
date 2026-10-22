#include <format>
#include <source_location>
#include <string>

#include <gtest/gtest.h>

#include "Log/Log.h"

using namespace Nexora;

DEFINE_LOG_CATEGORY_STATIC(LogTest)

TEST(LogTest, CategoryNameShouldBeValid)
{
    const LogCategory category1("TestCategory");
    EXPECT_EQ(category1.name(), "TestCategory");

    const LogCategory category2("AnotherCategory");
    EXPECT_EQ(category2.name(), "AnotherCategory");
}

TEST(LogTest, StaticCategoryNameShouldMatchVariableName)
{
    EXPECT_EQ(LogTest.name(), "LogTest");
}

TEST(LogTest, MessageShouldContainCategory)
{
    const auto location = std::source_location::current();

    EXPECT_EQ(formatLogMessage(LogTest, "Hello", false, location), "[LogTest] Hello");
    EXPECT_EQ(formatLogMessage(LogTest, "", false, location), "[LogTest] ");
}

TEST(LogTest, MessageShouldContainLocation)
{
    const auto location = std::source_location::current();
    const std::string expected = std::format("[LogTest] [{}:{}] Hello", location.function_name(), location.line());

    EXPECT_EQ(formatLogMessage(LogTest, "Hello", true, location), expected);
}

TEST(LogTest, AllVerbositiesExceptFatalShouldNotStopProgram)
{
    NX_LOG(LogTest, NoLogging, "NoLogging message");
    NX_LOG(LogTest, Display, "Display message");
    NX_LOG(LogTest, Warning, "Warning message");
    NX_LOG(LogTest, Error, "Error message");
    NX_LOG(LogTest, Display, "Answer is {}, name is {}, pi is {:.2f}", 42, "Nexora", 3.14159);
    NX_LOG_DEBUG(LogTest, Display, "Message with location");

    SUCCEED();
}

TEST(LogDeathTest, FatalShouldAbortProgram)
{
    EXPECT_DEATH(NX_LOG(LogTest, Fatal, "Fatal message"), "");
}
