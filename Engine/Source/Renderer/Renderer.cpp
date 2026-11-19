#include "Renderer/Renderer.h"

#include "Core/EngineConfig.h"
#include "Log/Log.h"
#include "RHI/IRHICommandList.h"
#include "RHI/IRHIDevice.h"
#include "RHI/IRHIGraphicsPipeline.h"
#include "RHI/IRHIShader.h"
#include "RHI/IRHIViewport.h"
#include "RHI/IShaderCompiler.h"
#include "Window/IWindowManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogRenderer);

namespace
{

Nexora::ShaderBytecode compileShader(Nexora::IShaderCompiler& shaderCompiler,
    const std::filesystem::path&                              path,
    const std::string&                                        entryPoint)
{
    auto bytecode = shaderCompiler.compile(path, entryPoint);
    if (!bytecode)
    {
        NX_LOG(LogRenderer, Fatal, "Failed to compile {} in {}", entryPoint, path.string());
    }
    return std::move(*bytecode);
}

}  // namespace

namespace Nexora
{

Renderer::Renderer(IWindowManager& windowManager,  //
    IRHIDevice&                    rhiDevice,      //
    IShaderCompiler&               shaderCompiler)
    : m_windowManager(windowManager),              //
      m_rhiDevice(rhiDevice)
{
    m_windowManager.windowCreatedEvent().add(this, &Renderer::onWindowCreated);

    // create pipeline ->
    const auto shaderPath     = std::filesystem::path(c_shaderDirectory) / "Triangle.slang";
    const auto vertexShader   = m_rhiDevice.createShader(compileShader(shaderCompiler, shaderPath, "vertexMain"));
    const auto fragmentShader = m_rhiDevice.createShader(compileShader(shaderCompiler, shaderPath, "fragmentMain"));

    const GraphicsPipelineSettings settings{
        .vertexShader   = vertexShader.get(),
        .fragmentShader = fragmentShader.get(),
        .colorFormat    = TextureFormat::B8G8R8A8Srgb,
    };
    m_pipeline = m_rhiDevice.createGraphicsPipeline(settings);
}

Renderer::~Renderer() = default;

void Renderer::render()
{
    for (const auto& [windowId, viewport] : m_viewports)
    {
        const auto window = m_windowManager.getWindowById(windowId);
        if (window->hasZeroSize())
        {
            continue;
        }

        viewport->beginFrame();

        auto& commandList = viewport->commandList();
        commandList.beginRendering(viewport->backBuffer(), std::nullopt);
        commandList.setPipeline(*m_pipeline);
        commandList.draw(3);
        commandList.endRendering();

        viewport->endFrame();
    }
}

void Renderer::onWindowCreated(WindowId id)
{
    const auto window = m_windowManager.getWindowById(id);
    window->windowEvent().add(this, &Renderer::onWindowEvent);
    m_viewports.emplace(id, m_rhiDevice.createViewport(window->nativeHandle(), window->width(), window->height()));
}

void Renderer::onWindowResized(WindowId id, int32 width, int32 height)
{
    const auto window = m_windowManager.getWindowById(id);
    if (window->hasZeroSize())
    {
        return;
    }
    m_viewports.at(id)->resize(width, height);
}

void Renderer::onWindowEvent(const InputEvent& event)
{
    std::visit(
        Overloaded{
            [this, &event](const WindowResizeEventData& data)              //
            {                                                              //
                onWindowResized(event.windowId, data.width, data.height);  //
            },                                                             //

            [this, &event](const WindowCloseEventData&)                    //
            {                                                              //
                m_viewports.erase(event.windowId);                         //
            },                                                             //

            [](const auto&) {},
        },
        event.data);
}

}  // namespace Nexora
