project(NexoraEngine VERSION 0.1.0)

set(ENGINE_SOURCES
    Source/pch.h
    Source/pch.cpp
    Source/Core/Types.h
    Source/Core/Utility.h
    Source/Core/EngineConfig.h.in
    Source/Core/Engine.h
    Source/Core/Engine.cpp
    Source/Core/Timer.h
    Source/Core/Timer.cpp
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
    Source/RHI/Vulkan/VulkanRHIDevice.h
    Source/RHI/Vulkan/VulkanRHIDevice.cpp
)

configure_file(Source/Core/EngineConfig.h.in ${CMAKE_CURRENT_BINARY_DIR}/Generated/Core/EngineConfig.h)

add_library(${PROJECT_NAME}Lib ${ENGINE_SOURCES})
target_include_directories(${PROJECT_NAME}Lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Source ${CMAKE_CURRENT_BINARY_DIR}/Generated)

if(MSVC)
    target_compile_options(${PROJECT_NAME}Lib PUBLIC /W4 /permissive- /Zc:preprocessor /utf-8)
endif()

find_package(spdlog REQUIRED)
find_package(glfw3 REQUIRED)
find_package(VulkanLoader REQUIRED)
find_package(vk-bootstrap REQUIRED)
target_link_libraries(${PROJECT_NAME}Lib PRIVATE spdlog::spdlog glfw Vulkan::Loader vk-bootstrap::vk-bootstrap)

add_executable(${PROJECT_NAME} Source/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}Lib)

find_package(GTest REQUIRED)
add_executable(${PROJECT_NAME}TestRunner
    Tests/LogTests.cpp
    Tests/EventTests.cpp
    Tests/WindowTests.cpp
    Tests/TimerTests.cpp
)
target_link_libraries(${PROJECT_NAME}TestRunner PRIVATE ${PROJECT_NAME}Lib GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}TestRunner WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

set_target_properties(${PROJECT_NAME} ${PROJECT_NAME}Lib ${PROJECT_NAME}TestRunner PROPERTIES FOLDER Engine)
create_ide_folders(ENGINE_SOURCES)
setup_precompiled_headers(${PROJECT_NAME}Lib Source/pch.cpp Source/pch.h "${ENGINE_SOURCES}")
