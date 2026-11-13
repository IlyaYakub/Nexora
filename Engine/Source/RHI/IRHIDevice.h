#pragma once

#include <memory>

#include "Core/Types.h"
#include "Core/Utility.h"

namespace Nexora
{

class IRHIViewport;

class IRHIDevice : public NonCopyable
{
public:
    virtual ~IRHIDevice() = default;

    // core
    virtual std::unique_ptr<IRHIViewport> createViewport(void* nativeWindowHandle,  //
        int32                                                  width,               //
        int32                                                  height) = 0;
};

}  // namespace Nexora
