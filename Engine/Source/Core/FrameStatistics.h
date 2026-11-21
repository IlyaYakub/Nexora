#pragma once

#include "Core/Types.h"
#include "Core/Utility.h"
#include "Window/WindowId.h"

namespace Nexora
{

class IWindowManager;
class Timer;

class FrameStatistics final : public NonCopyable
{
public:
    FrameStatistics(const Timer& timer,          //
        IWindowManager&          windowManager,  //
        WindowId                 windowId);

    // core
    void update();

private:
    const Timer&    m_timer;
    IWindowManager& m_windowManager;
    WindowId        m_windowId;
    uint32          m_frameCount{0};
    float           m_startSeconds{0.0f};
};

}  // namespace Nexora
