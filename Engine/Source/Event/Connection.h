#pragma once

#include "Core/Types.h"

#include <memory>

namespace Nexora
{

namespace Detail
{

class IEventState
{
public:
    virtual ~IEventState() = default;

    virtual void disconnect(uint64 listenerId)        = 0;
    virtual bool isConnected(uint64 listenerId) const = 0;
};

constexpr uint64 c_invalidListenerId = 0;

}  // namespace Detail

class Connection
{
public:
    Connection() = default;

    void disconnect();
    bool isConnected() const;

private:
    template <typename... Args>
    friend class Event;

    Connection(std::weak_ptr<Detail::IEventState> state, uint64 listenerId);

    std::weak_ptr<Detail::IEventState> m_state;
    uint64 m_listenerId{Detail::c_invalidListenerId};
};

class ScopedConnection final
{
public:
    ScopedConnection() = default;
    ScopedConnection(Connection connection);
    ~ScopedConnection();

    ScopedConnection(ScopedConnection&& other) noexcept;
    ScopedConnection& operator=(ScopedConnection&& other) noexcept;

    ScopedConnection(const ScopedConnection&)            = delete;
    ScopedConnection& operator=(const ScopedConnection&) = delete;

    void reset();
    Connection release();
    bool isConnected() const;

private:
    Connection m_connection;
};

}  // namespace Nexora