#include "Connection.h"

#include <utility>

using namespace Nexora;

// Connection

Connection::Connection(std::weak_ptr<Detail::IEventState> state, uint64 listenerId) : m_state(std::move(state)), m_listenerId(listenerId) {}

void Connection::disconnect()
{
    // lock() returns null when the event is already destroyed; then there is nothing to disconnect from.
    if (const auto state = m_state.lock())
    {
        state->disconnect(m_listenerId);
    }
    m_state.reset();
}

bool Connection::isConnected() const
{
    const auto state = m_state.lock();
    return state && state->isConnected(m_listenerId);
}

// ScopedConnection

ScopedConnection::ScopedConnection(Connection connection) : m_connection(std::move(connection)) {}

ScopedConnection::~ScopedConnection()
{
    m_connection.disconnect();
}

ScopedConnection::ScopedConnection(ScopedConnection&& other) noexcept : m_connection(other.release()) {}

ScopedConnection& ScopedConnection::operator=(ScopedConnection&& other) noexcept
{
    if (this != &other)
    {
        // The listener we owned so far is no longer wanted.
        m_connection.disconnect();
        m_connection = other.release();
    }
    return *this;
}

void ScopedConnection::reset()
{
    m_connection.disconnect();
}

Connection ScopedConnection::release()
{
    return std::exchange(m_connection, Connection{});
}

bool ScopedConnection::isConnected() const
{
    return m_connection.isConnected();
}
