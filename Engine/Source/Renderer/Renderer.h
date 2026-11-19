#pragma once

#include <memory>
#include <unordered_map>

#include "Core/Types.h"
#include "Core/Utility.h"
#include "Window/WindowId.h"

namespace Nexora
{

class IRHIDevice;
class IRHIGraphicsPipeline;
class IRHIViewport;
class IShaderCompiler;
class IWindowManager;
struct InputEvent;

class Renderer final : public NonCopyable
{
public:
    Renderer(IWindowManager& windowManager,  //
        IRHIDevice&          rhiDevice,      //
        IShaderCompiler&     shaderCompiler);
    ~Renderer();

    // core
    void render();

private:
    void onWindowCreated(WindowId id);
    void onWindowResized(WindowId id, int32 width, int32 height);
    void onWindowEvent(const InputEvent& event);

    IWindowManager&                                             m_windowManager;
    IRHIDevice&                                                 m_rhiDevice;
    std::unique_ptr<IRHIGraphicsPipeline>                       m_pipeline;
    std::unordered_map<WindowId, std::unique_ptr<IRHIViewport>> m_viewports;
};

}  // namespace Nexora
