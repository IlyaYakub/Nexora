#include <entt/signal/dispatcher.hpp>
#include <gtest/gtest.h>

#include "Event/InputEvents.h"

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
}  // namespace

TEST(EventTest, QueuedEventsShouldBeDeliveredOnUpdate)
{
    entt::dispatcher events;
    Weapon weapon;
    events.sink<KeyPressedEvent>().connect<&Weapon::onKeyPressed>(weapon);

    events.enqueue(KeyPressedEvent{Key::W});
    events.enqueue(KeyPressedEvent{Key::F});
    events.enqueue(KeyPressedEvent{Key::F, true});  // held key does not shoot again
    EXPECT_EQ(weapon.shots, 0);

    events.update();
    EXPECT_EQ(weapon.shots, 1);
}
