#include <entt/signal/dispatcher.hpp>
#include <gtest/gtest.h>

#include "Graphics/Vulkan/VulkanFence.h"
#include "Graphics/Vulkan/VulkanGraphicsContext.h"
#include "Graphics/Vulkan/VulkanImage.h"
#include "Graphics/Vulkan/VulkanRenderer.h"
#include "Window/GLFW/GLFWWindow.h"

using namespace Nexora;

TEST(VulkanContextTest, VulkanContextCreation)
{
    entt::dispatcher events;
    GLFWWindow window{{.title = "Vulkan Test", .width = 640, .height = 480, .visible = false}, events};
    ASSERT_TRUE(window.isValid());

    std::shared_ptr<VulkanGraphicsContext> context = std::make_shared<VulkanGraphicsContext>(window, true);

    EXPECT_NE(context->instance()->handle(), VK_NULL_HANDLE);
    EXPECT_NE(context->surface()->handle(), VK_NULL_HANDLE);
    EXPECT_NE(context->physicalDevice()->handle(), VK_NULL_HANDLE);
    EXPECT_NE(context->device()->handle(), VK_NULL_HANDLE);
    EXPECT_NE(context->device()->graphicsQueue(), VK_NULL_HANDLE);
    EXPECT_NE(context->device()->presentQueue(), VK_NULL_HANDLE);
    EXPECT_NE(context->device()->allocator(), nullptr);
    EXPECT_NE(context->swapchain()->handle(), VK_NULL_HANDLE);
    ASSERT_GT(context->swapchain()->imageCount(), 0u);
    EXPECT_FALSE(context->swapchain()->image(0)->ownsImage());
    EXPECT_NE(context->swapchain()->image(0)->view(), VK_NULL_HANDLE);
}

TEST(VulkanContextTest, OwnedImageAndFence)
{
    entt::dispatcher events;
    GLFWWindow window{{.title = "Vulkan Test", .width = 640, .height = 480, .visible = false}, events};
    ASSERT_TRUE(window.isValid());

    std::shared_ptr<VulkanGraphicsContext> context = std::make_shared<VulkanGraphicsContext>(window, true);

    // owned image
    const VulkanImageSettings settings{
        .extent = {256, 256},
        .format = VK_FORMAT_R8G8B8A8_UNORM,
        .usage  = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
    };
    std::shared_ptr<VulkanImage> image = std::make_shared<VulkanImage>(context->device(), settings);
    EXPECT_TRUE(image->ownsImage());
    EXPECT_NE(image->handle(), VK_NULL_HANDLE);
    EXPECT_NE(image->view(), VK_NULL_HANDLE);
    EXPECT_EQ(image->layout(), VK_IMAGE_LAYOUT_UNDEFINED);

    // signaled fence
    std::shared_ptr<VulkanFence> fence = std::make_shared<VulkanFence>(context->device(), true);
    fence->wait();
    fence->reset();
}

TEST(VulkanRendererTest, DrawFrames)
{
    entt::dispatcher events;
    GLFWWindow window{{.title = "Vulkan Test", .width = 640, .height = 480, .visible = false}, events};
    ASSERT_TRUE(window.isValid());

    std::shared_ptr<VulkanGraphicsContext> context = std::make_shared<VulkanGraphicsContext>(window, true);
    std::shared_ptr<VulkanRenderer> renderer       = std::make_shared<VulkanRenderer>(context->device(), context->swapchain());

    // reuse every frame
    for (uint32_t i = 0; i < VulkanRenderer::FramesInFlight * 3; ++i)
    {
        renderer->drawFrame();
    }

    EXPECT_EQ(context->swapchain()->image(0)->layout(), VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
}
