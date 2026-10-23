#include <gtest/gtest.h>

#include "Core/Engine.h"

using namespace Nexora;

TEST(EngineTest, EngineShouldStartAndStop)
{
    Engine engine({.title = "Engine Test", .width = 640, .height = 480, .visible = false});
    ASSERT_TRUE(engine.isInitialized());

    engine.stop();
    engine.run();

    EXPECT_TRUE(engine.window().shouldClose());
}
