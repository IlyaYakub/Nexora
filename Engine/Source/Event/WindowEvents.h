#pragma once

namespace Nexora
{

struct WindowClosedEvent
{
};

struct WindowResizedEvent
{
    int width{0};
    int height{0};
};

}
