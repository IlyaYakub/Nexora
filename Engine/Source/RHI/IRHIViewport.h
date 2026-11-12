#pragma once

#include "Core/Types.h"
#include "Core/Utility.h"

namespace Nexora
{

class IRHIViewport : public NonCopyable
{
public:
    virtual ~IRHIViewport() = default;

    virtual void resize(int32 width, int32 height) = 0;
};

}  // namespace Nexora
