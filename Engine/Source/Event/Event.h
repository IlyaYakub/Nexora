#pragma once

#include <compare>
#include <functional>
#include <vector>

#include "Core/Types.h"
#include "Core/Utility.h"

namespace Nexora
{

struct EventHandle
{
    uint64 value{0};

    bool isValid() const { return value != 0; }
    auto operator<=>(const EventHandle&) const = default;
};

template <typename... Args>
class Event final : public NonCopyable
{
public:
    using Listener = std::function<void(Args...)>;

    EventHandle add(Listener listener)
    {
        const EventHandle handle{m_nextHandleValue++};
        m_listeners.push_back({handle, std::move(listener)});
        return handle;
    }

    template <typename ObjType>
    EventHandle add(ObjType* object, void (ObjType::*method)(Args...))
    {
        return add([object, method](Args... args) { (object->*method)(args...); });
    }

    void remove(EventHandle handle)
    {
        std::erase_if(m_listeners, [handle](const Entry& entry) { return entry.handle == handle; });
    }

    void invoke(Args... args)
    {
        for (const auto& entry : m_listeners)
        {
            entry.listener(args...);
        }
    }

private:
    struct Entry
    {
        EventHandle handle;
        Listener    listener;
    };

    std::vector<Entry> m_listeners;
    uint64             m_nextHandleValue{1};
};

}  // namespace Nexora
