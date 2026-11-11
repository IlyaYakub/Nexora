#pragma once

#include <memory>

#include "Core/Utility.h"

namespace Nexora
{

class IWindowManager;
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
    std::unique_ptr<Timer> m_timer;
};

}  // namespace Nexora
