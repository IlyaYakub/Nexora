project(NexoraEngine VERSION 0.1.0)

set(ENGINE_SOURCES
    Source/pch.h
    Source/pch.cpp
    Source/Core/Types.h
    Source/Core/Utility.h
    Source/Core/EngineConfig.h.in
    Source/Core/Engine.h
    Source/Core/Engine.cpp
    Source/Core/File.h
    Source/Core/File.cpp
    Source/Core/Timer.h
    Source/Core/Timer.cpp
    Source/Core/FrameStatistics.h
    Source/Core/FrameStatistics.cpp
    Source/Log/Log.h
    Source/Log/Log.cpp
    Source/Event/Event.h
    Source/Event/InputEvent.h
    Source/Window/IWindow.h
    Source/Window/IWindowManager.h
    Source/Window/WindowId.h
    Source/Window/GLFW/GLFWWindow.h
    Source/Window/GLFW/GLFWWindow.cpp
    Source/Window/GLFW/GLFWKeyMapper.h
    Source/Window/GLFW/GLFWKeyMapper.cpp
    Source/Window/GLFW/GLFWWindowManager.h
    Source/Window/GLFW/GLFWWindowManager.cpp
    Source/RHI/IRHIDevice.h
    Source/RHI/IRHIViewport.h
    Source/RHI/IRHICommandList.h
    Source/RHI/IRHITexture.h
    Source/RHI/IRHIShader.h
    Source/RHI/ShaderCompiler/IShaderCompiler.h
    Source/RHI/IRHIPipeline.h
    Source/RHI/Vulkan/Public/VulkanRHIDevice.h
    Source/RHI/Vulkan/Private/VulkanRHIDevice.cpp
    Source/RHI/Vulkan/Public/VulkanUtility.h
    Source/RHI/Vulkan/Public/VulkanMapper.h
    Source/RHI/Vulkan/Private/VulkanMapper.cpp
    Source/RHI/Vulkan/Public/VulkanInstance.h
    Source/RHI/Vulkan/Private/VulkanInstance.cpp
    Source/RHI/Vulkan/Public/VulkanDevice.h
    Source/RHI/Vulkan/Private/VulkanDevice.cpp
    Source/RHI/Vulkan/Public/VulkanQueue.h
    Source/RHI/Vulkan/Private/VulkanQueue.cpp
    Source/RHI/Vulkan/Public/VulkanCommandPool.h
    Source/RHI/Vulkan/Private/VulkanCommandPool.cpp
    Source/RHI/Vulkan/Public/VulkanCommandBuffer.h
    Source/RHI/Vulkan/Private/VulkanCommandBuffer.cpp
    Source/RHI/Vulkan/Public/VulkanCommandList.h
    Source/RHI/Vulkan/Private/VulkanCommandList.cpp
    Source/RHI/Vulkan/Public/VulkanSurface.h
    Source/RHI/Vulkan/Private/VulkanSurface.cpp
    Source/RHI/Vulkan/Public/VulkanSwapchain.h
    Source/RHI/Vulkan/Private/VulkanSwapchain.cpp
    Source/RHI/Vulkan/Public/VulkanViewport.h
    Source/RHI/Vulkan/Private/VulkanViewport.cpp
    Source/RHI/Vulkan/Public/VulkanFrame.h
    Source/RHI/Vulkan/Private/VulkanFrame.cpp
    Source/RHI/Vulkan/Public/VulkanPresentationState.h
    Source/RHI/Vulkan/Private/VulkanPresentationState.cpp
    Source/RHI/Vulkan/Public/VulkanImage.h
    Source/RHI/Vulkan/Private/VulkanImage.cpp
    Source/RHI/Vulkan/Public/VulkanShader.h
    Source/RHI/Vulkan/Private/VulkanShader.cpp
    Source/RHI/Vulkan/Public/VulkanPipeline.h
    Source/RHI/Vulkan/Private/VulkanPipeline.cpp
    Source/RHI/ShaderCompiler/SlangShaderCompiler.h
    Source/RHI/ShaderCompiler/SlangShaderCompiler.cpp
    Source/RHI/Vulkan/Public/VulkanResourceState.h
    Source/RHI/Vulkan/Public/VulkanSync.h
    Source/RHI/Vulkan/Private/VulkanSync.cpp
    Source/RHI/Vulkan/Public/VulkanFence.h
    Source/RHI/Vulkan/Private/VulkanFence.cpp
    Source/RHI/Vulkan/Public/VulkanSemaphore.h
    Source/RHI/Vulkan/Private/VulkanSemaphore.cpp
    Source/Renderer/Renderer.h
    Source/Renderer/Renderer.cpp
)

set(SHADER_SOURCES
    Tests/Shaders/TestShader.slang
)

configure_file(Source/Core/EngineConfig.h.in ${CMAKE_CURRENT_BINARY_DIR}/Generated/Core/EngineConfig.h)

add_library(${PROJECT_NAME}Lib ${ENGINE_SOURCES})
target_include_directories(${PROJECT_NAME}Lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Source ${CMAKE_CURRENT_BINARY_DIR}/Generated)

if(MSVC)
    target_compile_options(${PROJECT_NAME}Lib PUBLIC /W4 /WX /external:anglebrackets /external:W0 /permissive- /Zc:preprocessor /utf-8)
endif()

find_package(spdlog REQUIRED)
find_package(glfw3 REQUIRED)
find_package(VulkanLoader REQUIRED)
find_package(glm REQUIRED)
target_link_libraries(${PROJECT_NAME}Lib PRIVATE spdlog::spdlog glfw Vulkan::Loader)
target_link_libraries(${PROJECT_NAME}Lib PUBLIC glm::glm)

cmake_path(CONVERT "$ENV{VULKAN_SDK}" TO_CMAKE_PATH_LIST VULKAN_SDK_DIR)
find_path(SLANG_INCLUDE_DIR slang.h PATHS ${VULKAN_SDK_DIR}/Include/slang REQUIRED NO_DEFAULT_PATH)
find_library(SLANG_LIBRARY slang PATHS ${VULKAN_SDK_DIR}/Lib REQUIRED NO_DEFAULT_PATH)
target_include_directories(${PROJECT_NAME}Lib PRIVATE ${SLANG_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME}Lib PRIVATE ${SLANG_LIBRARY})

add_executable(${PROJECT_NAME} Source/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}Lib)

find_package(GTest REQUIRED)
add_executable(${PROJECT_NAME}TestRunner
    Tests/LogTests.cpp
    Tests/EventTests.cpp
    Tests/WindowTests.cpp
    Tests/TimerTests.cpp
    Tests/FileTests.cpp
    Tests/ShaderCompilerTests.cpp
)
target_link_libraries(${PROJECT_NAME}TestRunner PRIVATE ${PROJECT_NAME}Lib GTest::gtest_main)
target_compile_definitions(${PROJECT_NAME}TestRunner PRIVATE NX_TEST_SHADER_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}/Tests/Shaders")
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}TestRunner WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

add_custom_target(${PROJECT_NAME}Shaders SOURCES ${SHADER_SOURCES})

set_target_properties(${PROJECT_NAME} ${PROJECT_NAME}Lib ${PROJECT_NAME}TestRunner ${PROJECT_NAME}Shaders PROPERTIES FOLDER Engine)
create_ide_folders(ENGINE_SOURCES)
create_ide_folders(SHADER_SOURCES)
setup_precompiled_headers(${PROJECT_NAME}Lib Source/pch.cpp Source/pch.h "${ENGINE_SOURCES}")
