#include <cstdlib>

#include "Core/Engine.h"

int main()
{
    Nexora::Engine engine;
    if (!engine.isInitialized())
    {
        return EXIT_FAILURE;
    }

    engine.run();
    return EXIT_SUCCESS;
}
