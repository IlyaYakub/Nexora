#include "Renderer/Renderer.h"

#include "RHI/IRHIDevice.h"
#include "RHI/IRHIViewport.h"
#include "Window/IWindowManager.h"

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
    for (const auto& [windowId, viewport] : m_viewports)
    {
        const std::shared_ptr<IWindow> window = m_windowManager.getWindowById(windowId);
        if (window->hasZeroSize())
        {
            continue;
        }

        viewport->beginFrame();
        viewport->endFrame();
    }
}

void Renderer::onWindowCreated(WindowId id)
{
    const std::shared_ptr<IWindow> window = m_windowManager.getWindowById(id);
    window->windowEvent().add(this, &Renderer::onWindowEvent);
    m_viewports.emplace(id, m_rhiDevice.createViewport(window->nativeHandle(), window->width(), window->height()));
}

void Renderer::onWindowResized(WindowId id, int32 width, int32 height)
{
    const std::shared_ptr<IWindow> window = m_windowManager.getWindowById(id);
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
