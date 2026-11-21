#include "Renderer/Renderer.h"

#include "RHI/IRHICommandList.h"
#include "RHI/IRHIDevice.h"
#include "RHI/IRHIViewport.h"
#include "Window/IWindowManager.h"

namespace
{

constexpr Nexora::TextureFormat c_backBufferFormat = Nexora::TextureFormat::B8G8R8A8Srgb;

}  // namespace

namespace Nexora
{

Renderer::Renderer(IWindowManager& windowManager, IRHIDevice& rhiDevice)
    : m_windowManager(windowManager),  //
      m_rhiDevice(rhiDevice)
{
    m_windowManager.windowCreatedEvent().add(this, &Renderer::onWindowCreated);
}

Renderer::~Renderer() = default;

void Renderer::render()
{
    for (const auto& viewport : m_viewports | std::views::values)
    {

        viewport->beginFrame();

        const RenderPassInfo renderPassInfo{
            .colorTarget = &viewport->backBuffer(),
            .loadAction  = RenderTargetLoadAction::Clear,
        };
        auto& commandList = viewport->commandList();
        commandList.beginRenderPass(renderPassInfo);
        commandList.endRenderPass();

        viewport->endFrame();
    }
}

void Renderer::onWindowCreated(WindowId id)
{
    const auto window = m_windowManager.getWindowById(id);
    window->windowEvent().add(this, &Renderer::onWindowEvent);
    const ViewportSettings settings{
        .nativeWindowHandle = window->nativeHandle(),
        .width              = window->width(),
        .height             = window->height(),
        .format             = c_backBufferFormat,
    };
    m_viewports.emplace(id, m_rhiDevice.createViewport(settings));
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
