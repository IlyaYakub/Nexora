#include "Log/Log.h"

DEFINE_LOG_CATEGORY_STATIC(LogMain);

int main()
{
    NX_LOG(LogMain, Display, "Nexora {}.{}", 0, 1);
    NX_LOG(LogMain, Log, "Written to the file only");
    NX_LOG(LogMain, Warning, "Something looks off: {}", 42);
    NX_LOG(LogMain, Error, "Something failed");
    return 0;
}
