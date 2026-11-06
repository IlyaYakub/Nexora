#include "Log/Log.h"

DEFINE_LOG_CATEGORY_STATIC(LogMain)

int main()
{
    NX_LOG(LogMain, Display, "Nexora engine started");    return 0;
}
