#pragma once

#include <memory>

#include "Core/Utility.h"

namespace Nexora
{

class IWindowManager;
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
};

}  // namespace Nexora
