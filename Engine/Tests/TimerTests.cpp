#include "Core/Timer.h"

#include <chrono>
#include <thread>

#include <gtest/gtest.h>

using namespace Nexora;

TEST(TimerTest, ShouldMeasureElapsedTimeBetweenTicks)
{
    Timer timer;
    timer.tick();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    timer.tick();

    EXPECT_GT(timer.deltaSeconds(), 0.0f);
}
