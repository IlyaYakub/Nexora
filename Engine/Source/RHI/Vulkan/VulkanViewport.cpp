#include "RHI/Vulkan/VulkanViewport.h"

#include "RHI/Vulkan/VulkanFrame.h"
#include "RHI/Vulkan/VulkanRHIDevice.h"
#include "RHI/Vulkan/VulkanSwapchain.h"
#include "RHI/Vulkan/VulkanUtility.h"

#include <vulkan/vulkan.h>

namespace
{

constexpr Nexora::uint32 c_framesInFlightCount = 2;

}  // namespace

namespace Nexora
{

VulkanViewport::VulkanViewport(const VulkanRHIDevice& device,              //
    void*                                             nativeWindowHandle,  //
    int32                                             width,               //
    int32                                             height)
    : m_device(device),                                                    //
      m_surface(device.instance(), nativeWindowHandle),                    //
      m_swapchain(std::make_unique<VulkanSwapchain>(device, m_surface.handle(), width, height))
{
    createRenderFinishedSemaphores();
    for (uint32 i = 0; i < c_framesInFlightCount; ++i)
    {
        m_frames.push_back(std::make_unique<VulkanFrame>(m_device));
    }
}

VulkanViewport::~VulkanViewport()
{
    destroyRenderFinishedSemaphores();
}

void VulkanViewport::resize(int32 width, int32 height)
{
    if (width == 0 || height == 0)
    {
        return;
    }

    NX_VULKAN_CHECK(vkDeviceWaitIdle(m_device.device()));
    destroyRenderFinishedSemaphores();
    m_swapchain = std::make_unique<VulkanSwapchain>(m_device, m_surface.handle(), width, height, m_swapchain->handle());
    createRenderFinishedSemaphores();
}

void VulkanViewport::createRenderFinishedSemaphores()
{
    const VkSemaphoreCreateInfo semaphoreInfo{
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
    };
    m_renderFinishedSemaphores.resize(m_swapchain->imageCount());
    for (VkSemaphore_T*& semaphore : m_renderFinishedSemaphores)
    {
        NX_VULKAN_CHECK(vkCreateSemaphore(m_device.device(), &semaphoreInfo, nullptr, &semaphore));
    }
}

void VulkanViewport::destroyRenderFinishedSemaphores()
{
    for (VkSemaphore_T* semaphore : m_renderFinishedSemaphores)
    {
        vkDestroySemaphore(m_device.device(), semaphore, nullptr);
    }
    m_renderFinishedSemaphores.clear();
}

}  // namespace Nexora
