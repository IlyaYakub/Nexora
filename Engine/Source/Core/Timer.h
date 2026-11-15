#pragma once

#include <chrono>

namespace Nexora
{

class Timer final
{
public:
    Timer();

    // core
    void tick();

    // getters
    float deltaSeconds() const;
    float totalSeconds() const;

private:
    std::chrono::steady_clock::time_point m_startTime;
    std::chrono::steady_clock::time_point m_lastTickTime;
    float                                 m_deltaSeconds{0.0f};
    float                                 m_totalSeconds{0.0f};
};

}  // namespace Nexora
