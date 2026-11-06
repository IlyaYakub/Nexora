# Sets up a precompiled header without CMake's generated cmake_pch.cxx, so the IDE does not show
# extra "Precompile Header File" / "Source Files" folders.
#   TARGET       - target to configure
#   PCH_SOURCE   - source file that creates the PCH (contains only #include of the header)
#   PCH_HEADER   - the precompiled header
#   SOURCE_FILES - all sources of the target (list)
function(setup_precompiled_headers TARGET PCH_SOURCE PCH_HEADER SOURCE_FILES)
    if(MSVC)
        get_filename_component(PCH_HEADER_NAME ${PCH_HEADER} NAME)
        target_sources(${TARGET} PRIVATE ${PCH_HEADER} ${PCH_SOURCE})
        set_source_files_properties(${PCH_SOURCE} PROPERTIES COMPILE_FLAGS "/Yc${PCH_HEADER_NAME}")

        foreach(SOURCE_FILE ${SOURCE_FILES})
            if(SOURCE_FILE MATCHES "\\.cpp$" AND NOT SOURCE_FILE STREQUAL PCH_SOURCE)
                set_source_files_properties(${SOURCE_FILE} PROPERTIES COMPILE_FLAGS "/Yu${PCH_HEADER_NAME}")
            endif()
        endforeach()
        target_compile_options(${TARGET} PRIVATE "/FI${PCH_HEADER_NAME}")
    else()
        target_precompile_headers(${TARGET} PRIVATE ${PCH_HEADER})
    endif()
endfunction()
