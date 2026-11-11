#include "Event/Event.h"

#include <gtest/gtest.h>

using namespace Nexora;

TEST(EventTest, ShouldInvokeListenersInSubscriptionOrder)
{
    Event<int32> event;
    std::vector<int32> calls;
    event.add([&](int32 value) { calls.push_back(value); });
    event.add([&](int32 value) { calls.push_back(value * 10); });

    event.invoke(2);

    EXPECT_EQ(calls, (std::vector<int32>{2, 20}));
}

TEST(EventTest, ShouldNotInvokeRemovedListener)
{
    Event<> event;
    int32 calls             = 0;
    EventHandle eventHandle = event.add([&] { ++calls; });
    event.remove(eventHandle);

    event.invoke();

    EXPECT_EQ(calls, 0);
}
