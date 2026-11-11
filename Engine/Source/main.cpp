#include <memory>

#include "Window/GLFW/GLFWWindowManager.h"

using namespace Nexora;

int main()
{
    // create window ->
    const std::unique_ptr<IWindowManager> windowManager = std::make_unique<GLFWWindowManager>();
    windowManager->createWindow(WindowSettings{});

    // main loop ->
    while (!windowManager->areAllWindowsClosed())
    {
        windowManager->update();
    }
    return 0;
}
