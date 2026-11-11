project(NexoraEngine VERSION 0.1.0)

set(ENGINE_SOURCES
    Source/pch.h
    Source/pch.cpp
)

add_library(${PROJECT_NAME}Lib ${ENGINE_SOURCES})
target_include_directories(${PROJECT_NAME}Lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Source)

if(MSVC)
    target_compile_options(${PROJECT_NAME}Lib PUBLIC /W4 /permissive- /Zc:preprocessor /utf-8)
endif()

add_executable(${PROJECT_NAME} Source/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}Lib)

set_target_properties(${PROJECT_NAME} ${PROJECT_NAME}Lib PROPERTIES FOLDER Engine)
create_ide_folders(ENGINE_SOURCES)
setup_precompiled_headers(${PROJECT_NAME}Lib Source/pch.cpp Source/pch.h "${ENGINE_SOURCES}")
