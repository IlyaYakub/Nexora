#pragma once

#include <memory>

#include "Core/Utility.h"
#include "Window/IWindowManager.h"

namespace Nexora
{

// Owns the engine managers and the main loop.
class Engine final : public NonCopyable
{
public:
    // Creates the managers. An unrecoverable failure (for example GLFW cannot start) is logged as Fatal.
    Engine();

    IWindowManager& windowManager() { return *m_windowManager; }

    // Runs the main loop until all windows are closed.
    void run();

private:
    std::unique_ptr<IWindowManager> m_windowManager;
};

}  // namespace Nexora
