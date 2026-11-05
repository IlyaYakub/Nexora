cmake_minimum_required(VERSION 3.25)

project(NexoraEngine VERSION 0.1.0 LANGUAGES CXX)

add_executable(${PROJECT_NAME} Source/main.cpp)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /Zc:preprocessor)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Engine)
