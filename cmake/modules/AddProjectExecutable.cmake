# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#

# Require CMake 3.13 or higher for target_link_options
cmake_minimum_required(VERSION 3.13)

# save policy
cmake_policy(PUSH)

include (AddProjectProperties)
include (ConfigWindowsVersion)

# add_project_executable func
function (add_project_executable _name)
    cmake_parse_arguments (ARG
        ""
        "ALIAS;OUTPUT_NAME;ENABLE_WALL;ENABLE_ASAN;C_STANDARD;CXX_STANDARD;INSTALL_BINDIR;FILE_DESCRIPTION;PRODUCT_NAME"
        "SOURCES;OBJECTS;DEPENDS;INCLUDE_DIRECTORIES;COMPILE_DEFINITIONS;COMPILE_OPTIONS;LINK_OPTIONS;LINK_DIRECTORIES;LINK_LIBRARIES"
        ${ARGN}
    )

    # validate sources
    if(NOT ARG_SOURCES)
        message(FATAL_ERROR "No sources specified for executable '${_name}'")
    endif()

    # validate dependencies
    if(ARG_DEPENDS)
        foreach(dep ${ARG_DEPENDS})
            if(NOT TARGET ${dep})
                message(FATAL_ERROR "Dependency target '${dep}' does not exist")
            endif()
        endforeach()
    endif()

    add_executable (${_name} ${ARG_SOURCES})

    # add objects
    if (ARG_OBJECTS)
        target_sources (${_name} PRIVATE $<TARGET_OBJECTS:${ARG_OBJECTS}>)
    endif()

    # add dependencies
    if (ARG_DEPENDS)
        add_dependencies (${_name} ${ARG_DEPENDS})
    endif()

    # PRIVATE PUBLIC INTERFACE
    add_project_properties (${_name} PRIVATE
        INCLUDE_DIRECTORIES
            ${ARG_INCLUDE_DIRECTORIES}
        COMPILE_DEFINITIONS
            ${ARG_COMPILE_DEFINITIONS}
        COMPILE_OPTIONS
            ${ARG_COMPILE_OPTIONS}
        LINK_OPTIONS
            ${ARG_LINK_OPTIONS}
        LINK_DIRECTORIES
            ${ARG_LINK_DIRECTORIES}
        LINK_LIBRARIES
            ${ARG_LINK_LIBRARIES}
    )

    # alias _name may not allowed in some target types, but not known yet
    if (ARG_ALIAS)
        add_library (${ARG_ALIAS} ALIAS ${_name})
    endif()

    if (ARG_OUTPUT_NAME)
        set_target_properties (${_name}
            PROPERTIES
                OUTPUT_NAME ${ARG_OUTPUT_NAME}
        )
    endif()

    # Set debug postfix for executable
    set_target_properties(${_name} PROPERTIES
        DEBUG_POSTFIX "d"
    )

    # set C/C++ standards
    if (ARG_C_STANDARD)
        set_target_properties(${_name} PROPERTIES
            C_STANDARD ${ARG_C_STANDARD}
            C_STANDARD_REQUIRED ON
            C_EXTENSIONS OFF
        )
    else()
        set_target_properties(${_name} PROPERTIES
            C_STANDARD 11
            C_STANDARD_REQUIRED ON
            C_EXTENSIONS OFF
        )
    endif()
    if (ARG_CXX_STANDARD)
        set_target_properties(${_name} PROPERTIES
            CXX_STANDARD ${ARG_CXX_STANDARD}
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
        )
    else()
        set_target_properties(${_name} PROPERTIES
            CXX_STANDARD 17
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
        )
    endif()

    if (MSVC)
        set_target_properties(${_name} PROPERTIES
            VS_DEBUGGER_ENVIRONMENT
                "$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:${VSDEBUG_ENV}>"
        )
    endif()

    # get compiler & language compatibility
    set (_IS_GNU_LANG $<AND:$<OR:$<C_COMPILER_ID:GNU,Clang>,$<CXX_COMPILER_ID:GNU,Clang>>,$<COMPILE_LANGUAGE:C,CXX>>)

    # add compile definitions
    target_compile_definitions (${_name}
        PRIVATE
            $<$<BOOL:${SYSTEM_IS_WINDOWS}>:WINDOWS>
            $<$<BOOL:${SYSTEM_IS_LINUX}>:LINUX>
            $<$<BOOL:${SYSTEM_IS_MACOS}>:MACOS>
    )

    target_compile_definitions (${_name}
        PRIVATE
            $<BUILD_INTERFACE:AXCL_BUILD_VERSION="${BUILD_VERSION}">
            $<BUILD_INTERFACE:COMPILER_USERNAME="${COMPILE_USER}">
        )

    # add compile options
    target_compile_options (${_name}
        PRIVATE
            $<$<BOOL:${_IS_GNU_LANG}>:-fdata-sections>
            $<$<BOOL:${_IS_GNU_LANG}>:-ffunction-sections>
            $<$<BOOL:${_IS_GNU_LANG}>:-Wl,--gc-sections>
            $<$<AND:$<BOOL:${_IS_GNU_LANG}>,$<BOOL:${ARG_ENABLE_ASAN}>>:-fsanitize=address>
            $<$<AND:$<BOOL:${_IS_GNU_LANG}>,$<BOOL:${ARG_ENABLE_ASAN}>>:-fno-omit-frame-pointer>
            $<$<AND:$<BOOL:${_IS_GNU_LANG}>,$<BOOL:${ARG_ENABLE_WALL}>>:-Wall>
            $<$<AND:$<BOOL:${_IS_GNU_LANG}>,$<BOOL:${ARG_ENABLE_WALL}>>:-Wextra>
            $<$<CXX_COMPILER_ID:MSVC>:/wd4996>
    )

    # add link options
    target_link_options (${_name}
        PRIVATE
            $<$<AND:$<BOOL:${_IS_GNU_LANG}>,$<BOOL:${ARG_ENABLE_ASAN}>>:-fsanitize=address>
            $<$<AND:$<BOOL:${_IS_GNU_LANG}>,$<BOOL:${ARG_ENABLE_WALL}>>:-Wall>
            $<$<AND:$<BOOL:${_IS_GNU_LANG}>,$<BOOL:${ARG_ENABLE_WALL}>>:-Wextra>
    )

    if(NOT ARG_INSTALL_BINDIR)
        set(ARG_INSTALL_BINDIR "bin")
    endif()

    install(TARGETS ${_name}
        RUNTIME DESTINATION ${ARG_INSTALL_BINDIR}
    )

    # Configure Windows version information
    if (SYSTEM_IS_WINDOWS)
        if (NOT ARG_FILE_DESCRIPTION)
            set(ARG_FILE_DESCRIPTION "AXCL Sample Application")
        endif()
        if (NOT ARG_PRODUCT_NAME)
            set(ARG_PRODUCT_NAME "AXCL SDK")
        endif()
        ConfigWindowsVersion(${_name} "${ARG_FILE_DESCRIPTION}" "${ARG_PRODUCT_NAME}")
    endif()
endfunction()

# restore policy
cmake_policy(POP)
