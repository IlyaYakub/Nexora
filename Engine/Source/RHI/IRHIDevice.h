#pragma once

#include "Core/Utility.h"

namespace Nexora
{

class IRHIDevice : public NonCopyable
{
public:
    virtual ~IRHIDevice() = default;

    virtual void beginFrame() = 0;
    virtual void endFrame()   = 0;
};

}  // namespace Nexora
