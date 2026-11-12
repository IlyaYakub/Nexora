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

void Renderer::onWindowCreated(WindowId id)
{
    const std::shared_ptr<IWindow> window = m_windowManager.getWindowById(id);
    window->windowEvent().add(this, &Renderer::onWindowEvent);
    m_viewports.emplace(id, m_rhiDevice.createViewport(window->nativeHandle(), window->width(), window->height()));
}

void Renderer::onWindowEvent(const InputEvent& event)
{
    std::visit(
        Overloaded{
            [this, &event](const WindowResizeEventData& data)                     //
            {                                                                     //
                m_viewports.at(event.windowId)->resize(data.width, data.height);  //
            },                                                                    //

            [this, &event](const WindowCloseEventData&)                           //
            {                                                                     //
                m_viewports.erase(event.windowId);                                //
            },                                                                    //

            [](const auto&) {},
        },
        event.data);
}

}  // namespace Nexora
