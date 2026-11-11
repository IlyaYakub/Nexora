#include "Core/Timer.h"

namespace Nexora
{

Timer::Timer() : m_startTime(std::chrono::steady_clock::now()), m_lastTickTime(m_startTime) {}

void Timer::tick()
{
    const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    m_deltaSeconds                                  = std::chrono::duration<float>(now - m_lastTickTime).count();
    m_totalSeconds                                  = std::chrono::duration<float>(now - m_startTime).count();
    m_lastTickTime                                  = now;
}

float Timer::deltaSeconds() const
{
    return m_deltaSeconds;
}

float Timer::totalSeconds() const
{
    return m_totalSeconds;
}

}  // namespace Nexora
