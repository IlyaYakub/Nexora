#include "Log/Log.h"

#include <gtest/gtest.h>

using namespace Nexora;

DEFINE_LOG_CATEGORY_STATIC(LogTest);

TEST(LogTest, ShouldAcceptOnlyLogCategory)
{
    EXPECT_TRUE(ValidLogCategory<decltype(LogTest)>);
    EXPECT_FALSE(ValidLogCategory<int>);
}
