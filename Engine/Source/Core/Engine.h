#pragma once

#include <memory>

#include "Core/Utility.h"

namespace Nexora
{

class FrameStatistics;
class IRHIDevice;
class IWindowManager;
class Renderer;
class Timer;

class Engine final : public NonCopyable
{
public:
    Engine();
    ~Engine();

    // core
    void run();

private:
    std::unique_ptr<IWindowManager>  m_windowManager;
    std::unique_ptr<Timer>           m_timer;
    std::unique_ptr<IRHIDevice>      m_rhiDevice;
    std::unique_ptr<Renderer>        m_renderer;
    std::unique_ptr<FrameStatistics> m_frameStatistics;
};

}  // namespace Nexora
