#include <entt/signal/dispatcher.hpp>
#include <gtest/gtest.h>

#include "Graphics/Vulkan/VulkanGraphicsContext.h"
#include "Graphics/Vulkan/VulkanInstance.h"
#include "Window/GLFW/GLFWWindow.h"

using namespace Nexora;

namespace
{
class VulkanContextTest : public testing::Test
{
protected:
    entt::dispatcher events;
    GLFWWindow window{{.title = "Vulkan Test", .width = 640, .height = 480, .visible = false}, events};
    VulkanGraphicsContext context{window, true};
};
}  // namespace

TEST(VulkanInstanceTest, InstanceCreationTest)
{
    const VulkanInstance instance({.enableValidation = true});
    EXPECT_NE(instance.handle(), VK_NULL_HANDLE);
    EXPECT_TRUE(instance.isValidationEnabled());
}

TEST_F(VulkanContextTest, DeviceShouldHaveQueues)
{
    const VulkanDevice& device = *context.device();
    EXPECT_NE(device.handle(), VK_NULL_HANDLE);
    EXPECT_NE(device.graphicsQueue(), VK_NULL_HANDLE);
    EXPECT_NE(device.presentQueue(), VK_NULL_HANDLE);
}

TEST_F(VulkanContextTest, SwapchainShouldMatchWindow)
{
    const VulkanSwapchain& swapchain = context.swapchain();
    EXPECT_EQ(swapchain.extent().width, 640u);
    EXPECT_EQ(swapchain.extent().height, 480u);
    EXPECT_EQ(swapchain.presentMode(), VK_PRESENT_MODE_FIFO_KHR);
    EXPECT_FALSE(swapchain.images().empty());
    EXPECT_EQ(swapchain.images().size(), swapchain.imageViews().size());
}

TEST_F(VulkanContextTest, SwapchainShouldBeRecreated)
{
    VulkanSwapchain& swapchain = context.swapchain();
    const VkSwapchainKHR oldHandle = swapchain.handle();

    window.setSize(800, 600);
    swapchain.recreate({800, 600});
    EXPECT_NE(swapchain.handle(), oldHandle);
    EXPECT_EQ(swapchain.extent().width, 800u);
    EXPECT_EQ(swapchain.extent().height, 600u);

    swapchain.setVsync(false);
    EXPECT_FALSE(swapchain.isVsync());
    EXPECT_NE(swapchain.presentMode(), VK_PRESENT_MODE_FIFO_KHR);
}
