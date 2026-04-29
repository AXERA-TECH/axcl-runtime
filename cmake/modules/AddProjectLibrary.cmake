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

# function: add_project_library
function (add_project_library _name)
    cmake_parse_arguments (ARG
        "MODULE;SHARED;STATIC;OBJECT;INTERFACE"
        "ALIAS;OUTPUT_NAME;HIDE_SYMBOL;ENABLE_WALL;ENABLE_ASAN;C_STANDARD;CXX_STANDARD;FILE_DESCRIPTION;PRODUCT_NAME"
        "SOURCES;OBJECTS;DEPENDS;PRIVATE;PUBLIC"
        ${ARGN}
    )

    # validate library type
    if(NOT (ARG_MODULE OR ARG_SHARED OR ARG_STATIC OR ARG_OBJECT OR ARG_INTERFACE))
        message(FATAL_ERROR "Library type must be specified as: MODULE|SHARED|STATIC|OBJECT|INTERFACE")
    endif()

    # validate interface library
    if(ARG_INTERFACE AND ARG_SOURCES)
        message(FATAL_ERROR "INTERFACE library should not have sources")
    endif()

    # validate dependencies
    if(ARG_DEPENDS)
        foreach(dep ${ARG_DEPENDS})
            if(NOT TARGET ${dep})
                message(FATAL_ERROR "Dependency target '${dep}' does not exist")
            endif()
        endforeach()
    endif()

    # validate output directory
    if(ARG_OUTPUT_NAME AND NOT (ARG_STATIC OR ARG_SHARED OR ARG_MODULE))
        message(FATAL_ERROR "OUTPUT_NAME can only be set for STATIC, SHARED, or MODULE libraries")
    endif()

    # add library with specified type & sources (if sources was given)
    if (ARG_MODULE)
        add_library (${_name} MODULE ${ARG_SOURCES})
    endif ()
    if (ARG_SHARED)
        add_library (${_name} SHARED ${ARG_SOURCES})
    endif ()
    if (ARG_STATIC)
        add_library (${_name} STATIC ${ARG_SOURCES})
    endif ()
    if (ARG_OBJECT)
        add_library (${_name} OBJECT ${ARG_SOURCES})
    endif ()
    if (ARG_INTERFACE)
        add_library (${_name} INTERFACE)
        if (ARG_SOURCES)
            message (WARNING "INTERFACE library should not have sources.")
        endif ()
    endif ()

    # add objects
    if (ARG_OBJECTS)
        target_sources (${_name} ${OBJECT_SCOPE_TYPE} $<TARGET_OBJECTS:${ARG_OBJECTS}>)
    endif()

    # add dependencies
    if (ARG_DEPENDS)
        add_dependencies (${_name} ${ARG_DEPENDS})
    endif()

    # PRIVATE PUBLIC INTERFACE
    if (ARG_INTERFACE)
        add_project_properties (${_name} INTERFACE ${ARG_PRIVATE})
        add_project_properties (${_name} INTERFACE ${ARG_PUBLIC})
    else ()
        if (ARG_PRIVATE)
            add_project_properties (${_name} PRIVATE ${ARG_PRIVATE})
        endif ()
        if (ARG_PUBLIC)
            add_project_properties (${_name} PUBLIC ${ARG_PUBLIC})
        endif ()
    endif ()

    # alias _name may not allowed in some target types, but not known yet
    if (ARG_ALIAS)
        add_library (${ARG_ALIAS} ALIAS ${_name})
    endif()

    if (ARG_STATIC OR ARG_SHARED OR ARG_MODULE)
        if (NOT ARG_OUTPUT_NAME AND WIN32)
            set(ARG_OUTPUT_NAME "lib${_name}")
        endif()

        if (ARG_OUTPUT_NAME)
            set_target_properties (${_name}
            PROPERTIES
                OUTPUT_NAME ${ARG_OUTPUT_NAME}
            )
        endif()
    endif()

    # it seems the standard properties should be inferred to be depended targets
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

    # get compiler & language compatibility
    set (_IS_GNU_LANG $<AND:$<OR:$<C_COMPILER_ID:GNU,Clang>,$<CXX_COMPILER_ID:GNU,Clang>>,$<COMPILE_LANGUAGE:C,CXX>>)

    if (NOT ARG_INTERFACE)
        set (INTERFACE_SCOPE_TYPE PRIVATE)

        target_compile_definitions (${_name}
            ${INTERFACE_SCOPE_TYPE}
                $<$<BOOL:${SYSTEM_IS_WINDOWS}>:WINDOWS>
                $<$<BOOL:${SYSTEM_IS_LINUX}>:LINUX>
                $<$<BOOL:${SYSTEM_IS_MACOS}>:MACOS>
        )

        target_compile_definitions (${_name}
            ${INTERFACE_SCOPE_TYPE}
            $<BUILD_INTERFACE:AXCL_BUILD_VERSION="${BUILD_VERSION}">
            $<BUILD_INTERFACE:AXCL_MODULE_NAME="${_name}">
            $<BUILD_INTERFACE:COMPILER_USERNAME="${COMPILE_USER}">
        )

        target_compile_options (${_name}
            ${INTERFACE_SCOPE_TYPE}
                $<$<BOOL:${_IS_GNU_LANG}>:-fdata-sections>
                $<$<BOOL:${_IS_GNU_LANG}>:-ffunction-sections>
                $<$<BOOL:${_IS_GNU_LANG}>:-Wl,--gc-sections>
                $<$<AND:$<BOOL:${_IS_GNU_LANG}>,$<BOOL:${ARG_ENABLE_ASAN}>>:-fsanitize=address>
                $<$<AND:$<BOOL:${_IS_GNU_LANG}>,$<BOOL:${ARG_ENABLE_ASAN}>>:-fno-omit-frame-pointer>
                $<$<CXX_COMPILER_ID:MSVC>:/wd4267>
                $<$<CXX_COMPILER_ID:MSVC>:/wd4819>
                $<$<CXX_COMPILER_ID:MSVC>:/wd4996>
                $<$<CXX_COMPILER_ID:MSVC>:/wd4251>
        )

        # 'target_link_options' added in 3.13
        target_link_options (${_name}
            ${INTERFACE_SCOPE_TYPE}
                $<$<AND:$<BOOL:${_IS_GNU_LANG}>,$<BOOL:${ARG_ENABLE_ASAN}>>:-fsanitize=address>
                $<$<AND:$<BOOL:${_IS_GNU_LANG}>,$<BOOL:${ARG_ENABLE_WALL}>>:-Wall>
                $<$<AND:$<BOOL:${_IS_GNU_LANG}>,$<BOOL:${ARG_ENABLE_WALL}>>:-Wextra>
        )
    endif ()

    # set visibility preset for C/C++ targets, and enable position independent code
    if (ARG_HIDE_SYMBOL)
        set_target_properties (${_name}
            PROPERTIES
                C_VISIBILITY_PRESET hidden
                CXX_VISIBILITY_PRESET hidden
        )
    endif ()

    # enable position independent code
    set_target_properties (${_name}
        PROPERTIES
            POSITION_INDEPENDENT_CODE ON
    )

    if (NOT ARG_INTERFACE)
        if (ARG_SHARED OR ARG_MODULE)
            install(TARGETS ${_name}
                RUNTIME DESTINATION bin
                LIBRARY DESTINATION bin
                ARCHIVE DESTINATION lib
            )
        elseif (ARG_STATIC)
            install(TARGETS ${_name}
                ARCHIVE DESTINATION lib
            )
        endif()

        # On Windows, remove .exp files after installation
        if (MSVC)
            install(CODE "
                file(GLOB EXP_FILES \"\${CMAKE_INSTALL_PREFIX}/lib/*.exp\")
                if(EXP_FILES)
                    file(REMOVE \${EXP_FILES})
                    message(STATUS \"Removed .exp files: \${EXP_FILES}\")
                endif()
            ")
        endif()
    endif()

    # Configure Windows version information for shared libraries and modules
    if (SYSTEM_IS_WINDOWS AND (ARG_SHARED OR ARG_MODULE))
        if (NOT ARG_FILE_DESCRIPTION)
            set(ARG_FILE_DESCRIPTION "AXCL Library")
        endif()
        if (NOT ARG_PRODUCT_NAME)
            set(ARG_PRODUCT_NAME "AXCL SDK")
        endif()
        ConfigWindowsVersion(${_name} "${ARG_FILE_DESCRIPTION}" "${ARG_PRODUCT_NAME}")
    endif()

endfunction()

# restore policy
cmake_policy(POP)
