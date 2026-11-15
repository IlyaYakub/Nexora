#pragma once

#include "Core/Utility.h"

namespace Nexora
{

class IRHITexture : public NonCopyable
{
public:
    virtual ~IRHITexture() = default;
};

}  // namespace Nexora
