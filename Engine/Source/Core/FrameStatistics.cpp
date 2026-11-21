#include "Core/FrameStatistics.h"

#include "Core/EngineConfig.h"
#include "Core/Timer.h"
#include "Window/IWindowManager.h"

namespace
{

constexpr float c_updateIntervalSeconds = 1.0f;

}  // namespace

namespace Nexora
{

FrameStatistics::FrameStatistics(const Timer& timer,          //
    IWindowManager&                           windowManager,  //
    WindowId                                  windowId)
    : m_timer(timer),                                         //
      m_windowManager(windowManager),                         //
      m_windowId(windowId)
{
}

void FrameStatistics::update()
{
    const auto totalSeconds   = m_timer.totalSeconds();
    const auto elapsedSeconds = totalSeconds - m_startSeconds;
    const auto window         = m_windowManager.getWindowById(m_windowId);

    ++m_frameCount;
    if (elapsedSeconds < c_updateIntervalSeconds)
    {
        return;
    }

    // show in title ->
    const auto framesPerSecond   = static_cast<float>(m_frameCount) / elapsedSeconds;
    const auto frameMilliseconds = 1000.0f / framesPerSecond;
    window->setTitle(std::format("Nexora {} | {:.0f} FPS | {:.2f} ms", c_engineVersion, framesPerSecond, frameMilliseconds));

    // restart ->
    m_frameCount   = 0;
    m_startSeconds = totalSeconds;
}

}  // namespace Nexora
