#include <gtest/gtest.h>

#include "Core/Engine.h"

using namespace Nexora;

namespace
{
struct QuitRequestedEvent
{
};

struct QuitListener
{
    Engine& engine;
    int calls{0};

    void onQuitRequested(const QuitRequestedEvent&)
    {
        ++calls;
        engine.stop();
    }
};
}  // namespace

TEST(EngineTest, EngineShouldRunUntilStopped)
{
    // hidden window, so the test does not flash on the screen
    Engine engine({.title = "Engine Test", .width = 640, .height = 480, .visible = false});
    ASSERT_TRUE(engine.isInitialized());
    EXPECT_NE(engine.graphicsContext().instance().handle(), VK_NULL_HANDLE);

    QuitListener listener{engine};
    engine.events().sink<QuitRequestedEvent>().connect<&QuitListener::onQuitRequested>(listener);
    engine.events().enqueue(QuitRequestedEvent{});

    // the first frame delivers the event, the listener stops the engine
    engine.run();

    EXPECT_EQ(listener.calls, 1);
    EXPECT_TRUE(engine.window().shouldClose());
}

TEST(EngineTest, EngineShouldNotBeInitializedWithInvalidWindow)
{
    Engine engine({.title = "Invalid", .width = 0, .height = 0, .visible = false});
    EXPECT_FALSE(engine.isInitialized());

    // must return immediately and not crash
    engine.run();
    engine.stop();
}
