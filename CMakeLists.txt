cmake_minimum_required(VERSION 3.31)

# dependencies come from conan: automation.py runs "conan install" and passes conan_toolchain.cmake
project(Nexora VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(${CMAKE_SOURCE_DIR}/Automation/CMAKE/CMakeHelpers.cmake)

add_subdirectory(Engine)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT NexoraEngine)

system_info()
