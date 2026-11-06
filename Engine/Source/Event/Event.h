#pragma once

#include "Core/Types.h"
#include "Core/Utility.h"
#include "Event/Connection.h"

#include <algorithm>
#include <deque>
#include <functional>
#include <memory>

namespace Nexora
{

template <typename... Args>
class Event final : public NonCopyable
{
public:
    using Listener = std::function<void(Args...)>;

    Event() : m_state(std::make_shared<State>()) {}

    // An empty listener is not stored (calling it would throw std::bad_function_call); the returned connection is empty.
    Connection add(Listener listener)
    {
        if (!listener) return Connection{};

        const uint64 id = m_state->nextListenerId++;
        m_state->slots.push_back(Slot{id, std::move(listener)});
        return Connection(m_state, id);
    }

    template <typename ObjType>
    Connection add(ObjType* object, void (ObjType::*method)(Args...))
    {
        return add([object, method](Args... args) { (object->*method)(args...); });
    }

    template <typename ObjType>
    Connection add(const ObjType* object, void (ObjType::*method)(Args...) const)
    {
        return add([object, method](Args... args) { (object->*method)(args...); });
    }

    void invoke(Args... args)
    {
        // A local owner: a listener may destroy this Event (and its owner), but State stays alive until invoke() returns.
        // Only `state` is used below, never members of `this`.
        const std::shared_ptr<State> state = m_state;
        ++state->invokeDepth;

        const size_t slotCount = state->slots.size();
        for (size_t i = 0; i < slotCount; ++i)
        {
            const Slot& slot = state->slots[i];
            if (slot.id != Detail::c_invalidListenerId)
            {
                slot.listener(args...);
            }
        }

        if (--state->invokeDepth == 0)
        {
            std::erase_if(state->slots, [](const Slot& slot) { return slot.id == Detail::c_invalidListenerId; });
        }
    }

private:
    struct Slot
    {
        uint64 id{Detail::c_invalidListenerId};
        Listener listener;
    };

    struct State final : Detail::IEventState
    {
        std::deque<Slot> slots;
        uint64 nextListenerId{1};
        uint32 invokeDepth{0};

        void disconnect(uint64 listenerId) override
        {
            const auto it = std::ranges::find(slots, listenerId, &Slot::id);
            if (it == slots.end()) return;

            if (invokeDepth > 0)
            {
                it->id = Detail::c_invalidListenerId;
            }
            else
            {
                slots.erase(it);
            }
        }

        bool isConnected(uint64 listenerId) const override
        {
            return listenerId != Detail::c_invalidListenerId && std::ranges::find(slots, listenerId, &Slot::id) != slots.end();
        }
    };

    std::shared_ptr<State> m_state;
};

}  // namespace Nexora