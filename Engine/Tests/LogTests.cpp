#include <gtest/gtest.h>

#include "Log/Log.h"

using namespace Nexora;

DEFINE_LOG_CATEGORY_STATIC(LogTest)

TEST(LogTest, AllVerbosities)
{
    NX_LOG(LogTest, NoLogging, "NoLogging message");
    NX_LOG(LogTest, Display, "Display message");
    NX_LOG(LogTest, Warning, "Warning message");
    NX_LOG(LogTest, Error, "Error message");
    EXPECT_DEATH(NX_LOG(LogTest, Fatal, "Fatal message"), "");
}
