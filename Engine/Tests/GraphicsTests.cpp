#include <gtest/gtest.h>

#include "Graphics/Vulkan/VulkanInstance.h"

using namespace Nexora;

TEST(VulkanInstanceTest, InstanceCreationTest)
{
    const VulkanInstance instance({.enableValidation = true});
    EXPECT_NE(instance.handle(), VK_NULL_HANDLE);
    EXPECT_TRUE(instance.isValidationEnabled());
}
