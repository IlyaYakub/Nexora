#pragma once

#include "Core/Types.h"
#include "Core/Utility.h"

namespace Nexora
{

class IRHICommandList;
class IRHITexture;

class IRHIViewport : public NonCopyable
{
public:
    virtual ~IRHIViewport() = default;

    // core
    virtual void resize(int32 width, int32 height) = 0;
    virtual void beginFrame()                      = 0;
    virtual void endFrame()                        = 0;

    // getters
    virtual IRHICommandList& commandList() = 0;
    virtual IRHITexture&     backBuffer()  = 0;
};

}  // namespace Nexora
