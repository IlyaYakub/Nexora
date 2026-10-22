#include <gtest/gtest.h>

#include "Core/Engine.h"
#include "EngineConfig.h"

using namespace Nexora;

namespace
{
// hidden window, so tests do not flash on the screen
WindowSettings makeTestSettings()
{
    return {.title = "Engine Test", .width = 640, .height = 480, .visible = false};
}
}  // namespace

TEST(EngineTest, VersionShouldMatchConfig)
{
    static_assert(Engine::version() == ENGINE_VERSION_STRING);
    EXPECT_EQ(Engine::version(), "1.0.0");
}

TEST(EngineTest, EngineShouldBeInitializedWithWindow)
{
    const Engine engine(makeTestSettings());

    ASSERT_TRUE(engine.isInitialized());
    EXPECT_TRUE(engine.window().isValid());
    EXPECT_EQ(engine.window().title(), "Engine Test");
    EXPECT_EQ(engine.window().width(), 640);
    EXPECT_EQ(engine.window().height(), 480);
}

TEST(EngineTest, RunShouldReturnAfterStop)
{
    Engine engine(makeTestSettings());
    ASSERT_TRUE(engine.isInitialized());

    engine.stop();
    engine.run();

    EXPECT_TRUE(engine.window().shouldClose());
}

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

TEST(EngineTest, RunShouldDeliverQueuedEvents)
{
    Engine engine(makeTestSettings());
    ASSERT_TRUE(engine.isInitialized());

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
