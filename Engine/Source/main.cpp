#include "Log/Log.h"
#include "Window/GLFW/GLFWWindowManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogMain)

int main()
{
    NX_LOG(LogMain, Display, "Nexora engine started");

    auto managerResult = Nexora::GLFWWindowManager::create();
    if (!managerResult)
    {
        NX_LOG(LogMain, Error, "Failed to create the window manager");
        return 1;
    }
    Nexora::IWindowManager& windowManager = **managerResult;

    if (!windowManager.createWindow({}))
    {
        NX_LOG(LogMain, Error, "Failed to create the main window");
        return 1;
    }

    while (!windowManager.areAllWindowsClosed())
    {
        windowManager.update();
    }

    return 0;
}
