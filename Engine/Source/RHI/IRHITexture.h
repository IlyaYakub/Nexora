#pragma once

#include "Core/Types.h"
#include "Core/Utility.h"

namespace Nexora
{

enum class TextureFormat : uint8
{
    Unknown,
    B8G8R8A8Srgb
};

class IRHITexture : public NonCopyable
{
public:
    virtual ~IRHITexture() = default;

    // getters
    virtual TextureFormat format() const = 0;
};

}  // namespace Nexora
