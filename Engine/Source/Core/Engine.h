#pragma once

#include <memory>

#include "Core/Utility.h"

namespace Nexora
{

class IRHIDevice;
class IWindowManager;
class Renderer;
class Timer;
struct InputEvent;

class Engine final : public NonCopyable
{
public:
    Engine();
    ~Engine();

    void run();

private:
    void onWindowEvent(const InputEvent& event);

    std::unique_ptr<IWindowManager> m_windowManager;
    std::unique_ptr<Timer>          m_timer;
    std::unique_ptr<IRHIDevice>     m_rhiDevice;
    std::unique_ptr<Renderer>       m_renderer;
};

}  // namespace Nexora
