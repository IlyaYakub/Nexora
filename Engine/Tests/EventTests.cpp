#include <optional>

#include <entt/signal/dispatcher.hpp>
#include <entt/signal/sigh.hpp>
#include <gtest/gtest.h>

#include "Event/InputEvents.h"
#include "Event/WindowEvents.h"

using namespace Nexora;

namespace
{
// example of a game system: shoots on F, does not know anything about GLFW
struct Weapon
{
    int shots{0};

    void onKeyPressed(const KeyPressedEvent& event)
    {
        if (event.key == Key::F && !event.repeat)
        {
            ++shots;
        }
    }
};

// example of a custom game event, declared outside of the engine
struct WeaponFiredEvent
{
    int ammoLeft{0};
};

struct Listener
{
    int calls{0};
    std::optional<MouseScrolledEvent> lastScroll;
    std::optional<WeaponFiredEvent> lastFired;

    void onKeyPressed(const KeyPressedEvent&) { ++calls; }
    void onScrolled(const MouseScrolledEvent& event) { lastScroll = event; }
    void onWeaponFired(const WeaponFiredEvent& event) { lastFired = event; }
};
}  // namespace

TEST(EventTest, TriggerShouldDeliverEventImmediately)
{
    entt::dispatcher events;
    Listener listener;
    events.sink<MouseScrolledEvent>().connect<&Listener::onScrolled>(listener);

    events.trigger(MouseScrolledEvent{1.5, -2.0});

    ASSERT_TRUE(listener.lastScroll.has_value());
    EXPECT_DOUBLE_EQ(listener.lastScroll->xOffset, 1.5);
    EXPECT_DOUBLE_EQ(listener.lastScroll->yOffset, -2.0);
}

TEST(EventTest, QueuedEventShouldBeDeliveredOnlyOnUpdate)
{
    entt::dispatcher events;
    Listener listener;
    events.sink<KeyPressedEvent>().connect<&Listener::onKeyPressed>(listener);

    events.enqueue(KeyPressedEvent{Key::Space});
    events.enqueue(KeyPressedEvent{Key::Enter});
    EXPECT_EQ(listener.calls, 0);

    events.update();
    EXPECT_EQ(listener.calls, 2);

    // queue is empty after update
    events.update();
    EXPECT_EQ(listener.calls, 2);
}

TEST(EventTest, WeaponShouldReactOnlyToItsKey)
{
    entt::dispatcher events;
    Weapon weapon;
    events.sink<KeyPressedEvent>().connect<&Weapon::onKeyPressed>(weapon);

    events.enqueue(KeyPressedEvent{Key::W});
    events.enqueue(KeyPressedEvent{Key::F});
    events.enqueue(KeyPressedEvent{Key::F, true});  // held key does not shoot again
    events.update();

    EXPECT_EQ(weapon.shots, 1);
}

TEST(EventTest, CustomEventShouldWorkWithoutEngineChanges)
{
    entt::dispatcher events;
    Listener listener;
    events.sink<WeaponFiredEvent>().connect<&Listener::onWeaponFired>(listener);

    events.enqueue(WeaponFiredEvent{29});
    events.update();

    ASSERT_TRUE(listener.lastFired.has_value());
    EXPECT_EQ(listener.lastFired->ammoLeft, 29);
}

TEST(EventTest, ScopedConnectionShouldDisconnectOnDestruction)
{
    entt::dispatcher events;
    Weapon weapon;
    {
        const entt::scoped_connection connection = events.sink<KeyPressedEvent>().connect<&Weapon::onKeyPressed>(weapon);
        events.trigger(KeyPressedEvent{Key::F});
        EXPECT_EQ(weapon.shots, 1);
    }

    events.trigger(KeyPressedEvent{Key::F});
    EXPECT_EQ(weapon.shots, 1);
}

TEST(EventTest, ManyListenersShouldReceiveSameEvent)
{
    entt::dispatcher events;
    Weapon weapon1;
    Weapon weapon2;
    events.sink<KeyPressedEvent>().connect<&Weapon::onKeyPressed>(weapon1);
    events.sink<KeyPressedEvent>().connect<&Weapon::onKeyPressed>(weapon2);

    events.trigger(KeyPressedEvent{Key::F});

    EXPECT_EQ(weapon1.shots, 1);
    EXPECT_EQ(weapon2.shots, 1);
}

TEST(EventTest, EventsShouldHaveDefaultValues)
{
    EXPECT_EQ(KeyPressedEvent{}.key, Key::Unknown);
    EXPECT_FALSE(KeyPressedEvent{}.repeat);
    EXPECT_EQ(MouseButtonPressedEvent{}.button, MouseButton::Left);
    EXPECT_EQ(WindowResizedEvent{}.width, 0);
    EXPECT_EQ(WindowResizedEvent{}.height, 0);
}
