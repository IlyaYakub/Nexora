#include <cstdlib>

#include "Engine.h"
#include "Log/Log.h"

DEFINE_LOG_CATEGORY_STATIC(LogMain)

int main()
{
    NX_LOG(LogMain, Display, "Nexora engine started");

    Nexora::Engine engine;

    if (!engine.windowManager().createWindow({}))
    {
        NX_LOG(LogMain, Error, "Failed to create the main window");
        return EXIT_FAILURE;
    }

    engine.run();
    return EXIT_SUCCESS;
}
