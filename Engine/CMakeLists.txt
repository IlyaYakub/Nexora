project(NexoraEngine VERSION 0.1.0)

set(ENGINE_SOURCES
    Source/pch.h
    Source/pch.cpp
    Source/Core/Types.h
    Source/Core/Utility.h
    Source/Log/Log.h
    Source/Log/Log.cpp
    Source/Event/Event.h
)

add_library(${PROJECT_NAME}Lib ${ENGINE_SOURCES})
target_include_directories(${PROJECT_NAME}Lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Source)

if(MSVC)
    target_compile_options(${PROJECT_NAME}Lib PUBLIC /W4 /permissive- /Zc:preprocessor /utf-8)
endif()

find_package(spdlog REQUIRED)
target_link_libraries(${PROJECT_NAME}Lib PRIVATE spdlog::spdlog)

add_executable(${PROJECT_NAME} Source/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}Lib)

find_package(GTest REQUIRED)
add_executable(${PROJECT_NAME}TestRunner
    Tests/LogTests.cpp
    Tests/EventTests.cpp
)
target_link_libraries(${PROJECT_NAME}TestRunner PRIVATE ${PROJECT_NAME}Lib GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}TestRunner WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

set_target_properties(${PROJECT_NAME} ${PROJECT_NAME}Lib ${PROJECT_NAME}TestRunner PROPERTIES FOLDER Engine)
create_ide_folders(ENGINE_SOURCES)
setup_precompiled_headers(${PROJECT_NAME}Lib Source/pch.cpp Source/pch.h "${ENGINE_SOURCES}")
