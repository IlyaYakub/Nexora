#pragma once

#include <memory>
#include <unordered_map>

#include "Core/Utility.h"
#include "Window/WindowId.h"

namespace Nexora
{

class IRHIDevice;
class IRHIViewport;
class IWindowManager;
struct InputEvent;

class Renderer final : public NonCopyable
{
public:
    Renderer(IWindowManager& windowManager, IRHIDevice& rhiDevice);
    ~Renderer();

private:
    void onWindowCreated(WindowId id);
    void onWindowEvent(const InputEvent& event);

    IWindowManager&                                             m_windowManager;
    IRHIDevice&                                                 m_rhiDevice;
    std::unordered_map<WindowId, std::unique_ptr<IRHIViewport>> m_viewports;
};

}  // namespace Nexora
