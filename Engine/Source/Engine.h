#pragma once

#include "Core/Utility.h"
#include "Graphics/IGraphicsContext.h"
#include "Window/IWindowManager.h"

#include <memory>

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
    std::unique_ptr<IGraphicsContext> m_graphicsContext;
};

}  // namespace Nexora
