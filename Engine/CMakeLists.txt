cmake_minimum_required(VERSION 3.25)

project(NexoraEngine VERSION 0.1.0 LANGUAGES CXX)

find_package(spdlog REQUIRED)
find_package(GTest REQUIRED)

set(ENGINE_SOURCES
    Source/pch.h
    Source/pch.cpp
    Source/Core/Types.h
    Source/Core/Utility.h
    Source/Log/Log.h
    Source/Log/Log.cpp
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/Source FILES ${ENGINE_SOURCES})

# engine library
add_library(${PROJECT_NAME}Lib STATIC ${ENGINE_SOURCES})
target_include_directories(${PROJECT_NAME}Lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Source)
target_link_libraries(${PROJECT_NAME}Lib PUBLIC spdlog::spdlog)
include(${CMAKE_SOURCE_DIR}/Automation/CMAKE/CMakeHelpers.cmake)
setup_precompiled_headers(${PROJECT_NAME}Lib Source/pch.cpp Source/pch.h "${ENGINE_SOURCES}")

# executable
add_executable(${PROJECT_NAME} Source/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}Lib)

# tests
add_executable(${PROJECT_NAME}TestRunner
    Tests/LogTests.cpp
)
target_link_libraries(${PROJECT_NAME}TestRunner PRIVATE ${PROJECT_NAME}Lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}TestRunner)

if(MSVC)
    target_compile_options(${PROJECT_NAME}Lib PUBLIC /Zc:preprocessor)
    target_compile_options(${PROJECT_NAME}Lib PRIVATE /W4)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
    target_compile_options(${PROJECT_NAME}TestRunner PRIVATE /W4)
endif()

set_target_properties(${PROJECT_NAME}Lib ${PROJECT_NAME} ${PROJECT_NAME}TestRunner PROPERTIES FOLDER Engine)
