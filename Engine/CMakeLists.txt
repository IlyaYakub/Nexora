project(NexoraEngine VERSION 1.0.0 LANGUAGES CXX)

find_package(spdlog REQUIRED)
find_package(GTest REQUIRED)
find_package(glfw3 REQUIRED)
find_package(EnTT REQUIRED)

# version header is generated into the build folder
configure_file(EngineConfig.h.template ${CMAKE_CURRENT_BINARY_DIR}/Generated/EngineConfig.h)

set(ENGINE_SOURCES
    .clang-format
    Source/pch.h
    Source/Core/Engine.h
    Source/Core/Engine.cpp
    Source/Core/Utility.h
    Source/Event/InputEvents.h
    Source/Event/WindowEvents.h
    Source/Input/KeyCodes.h
    Source/Log/Log.h
    Source/Log/Log.cpp
    Source/Window/GLFW/GLFWWindow.h
    Source/Window/GLFW/GLFWWindow.cpp
)

set(TEST_SOURCES
    Tests/EngineTests.cpp
    Tests/EventTests.cpp
    Tests/LogTests.cpp
    Tests/WindowTests.cpp
)

# engine library
add_library(${PROJECT_NAME}Lib STATIC ${ENGINE_SOURCES})
target_include_directories(${PROJECT_NAME}Lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Source
    ${CMAKE_CURRENT_BINARY_DIR}/Generated
)
# EnTT is public: engine users subscribe to events through entt::dispatcher
target_link_libraries(${PROJECT_NAME}Lib PUBLIC EnTT::EnTT PRIVATE spdlog::spdlog glfw)
target_precompile_headers(${PROJECT_NAME}Lib PRIVATE Source/pch.h)

if(MSVC)
    # /Zc:preprocessor is required for __VA_OPT__ in log macros
    target_compile_options(${PROJECT_NAME}Lib PUBLIC /Zc:preprocessor /utf-8)
    target_compile_options(${PROJECT_NAME}Lib PRIVATE /W4)
endif()

# engine executable
add_executable(${PROJECT_NAME} Source/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}Lib)

# tests
add_executable(${PROJECT_NAME}TestRunner ${TEST_SOURCES})
target_link_libraries(${PROJECT_NAME}TestRunner PRIVATE ${PROJECT_NAME}Lib GTest::gtest_main)

# IDE folders
set_target_properties(${PROJECT_NAME} ${PROJECT_NAME}Lib ${PROJECT_NAME}TestRunner PROPERTIES FOLDER Engine)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ENGINE_SOURCES} ${TEST_SOURCES})
