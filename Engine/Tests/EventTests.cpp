#include "Event/Event.h"

#include <gtest/gtest.h>

#include <memory>
#include <vector>

using namespace Nexora;

TEST(EventTest, InvokeShouldCallConnectedListenersOnly)
{
    Event<int> event;
    std::vector<int> first;
    std::vector<int> second;

    Connection firstConnection = event.add([&](int value) { first.push_back(value); });
    event.add([&](int value) { second.push_back(value); });

    event.invoke(1);
    firstConnection.disconnect();
    event.invoke(2);

    EXPECT_EQ(first, std::vector<int>({1}));
    EXPECT_EQ(second, std::vector<int>({1, 2}));
    EXPECT_FALSE(firstConnection.isConnected());
}

TEST(EventTest, ScopedConnectionShouldDisconnectWhenDestroyed)
{
    Event<> event;
    int calls = 0;

    {
        ScopedConnection connection = event.add([&] { ++calls; });
        event.invoke();
    }
    event.invoke();

    EXPECT_EQ(calls, 1);
}

TEST(EventTest, ListenerShouldBeAbleToDisconnectItselfDuringInvoke)
{
    Event<> event;
    int selfCalls  = 0;
    int otherCalls = 0;

    ScopedConnection self;
    self = event.add(
        [&]
        {
            ++selfCalls;
            self.reset();
        });
    event.add([&] { ++otherCalls; });

    event.invoke();
    event.invoke();

    EXPECT_EQ(selfCalls, 1);
    EXPECT_EQ(otherCalls, 2);
}

TEST(EventTest, ListenerShouldBeAbleToDestroyEventDuringInvoke)
{
    auto event = std::make_unique<Event<>>();
    int calls  = 0;

    event->add(
        [&]
        {
            ++calls;
            event.reset();
        });
    event->add([&] { ++calls; });

    event->invoke();  // must not touch freed memory; the remaining listeners still run

    EXPECT_EQ(event, nullptr);
    EXPECT_EQ(calls, 2);
}

TEST(EventTest, ConnectionShouldBeSafeAfterEventIsDestroyed)
{
    auto event                  = std::make_unique<Event<>>();
    ScopedConnection connection = event->add([] {});

    event.reset();

    EXPECT_FALSE(connection.isConnected());
    connection.reset();  // must not crash
}
