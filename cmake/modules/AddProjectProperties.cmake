# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# save policy
cmake_policy(PUSH)

function (add_project_properties _name _visibility)
    # verify if target exist
    if(NOT TARGET ${_name})
        message(FATAL_ERROR "Target '${_name}' does not exist")
    endif()

    # check param ${_visibility}
    if(NOT ${_visibility} MATCHES "^(PUBLIC|PRIVATE|INTERFACE)$")
        message(FATAL_ERROR "Invalid visibility '${_visibility}'. Must be one of: PUBLIC, PRIVATE, INTERFACE")
    endif()

    cmake_parse_arguments (
        "ARG" "" ""
        "INCLUDE_DIRECTORIES;COMPILE_DEFINITIONS;COMPILE_OPTIONS;LINK_OPTIONS;LINK_DIRECTORIES;LINK_LIBRARIES"
        ${ARGN}
    )

    # get target type
    get_target_property (_target_type ${_name} TYPE)
    if(NOT _target_type)
        message(FATAL_ERROR "Failed to get target type for '${_name}'")
    endif()

    # target type 'MODULE_LIBRARY', 'EXECUTABLE' means properties will not propagate to its dependents
    if (_target_type STREQUAL "MODULE_LIBRARY" OR _target_type STREQUAL "EXECUTABLE")
        # 'INTERFACE' is not allowed in 'MODULE_LIBRARY' and 'EXECUTABLE'
        if (${_visibility} STREQUAL "INTERFACE")
            message (FATAL_ERROR "Properties with type 'INTERFACE' is not allowed in target type '${_target_type}'")
        endif ()
        if (${_visibility} STREQUAL "PUBLIC")
            message (WARNING "Properties with type 'PUBLIC' is not allowed in target type '${_target_type}', will be treated as 'PRIVATE'")
            set (_visibility PRIVATE)
        endif ()
    endif ()

    # add each property
    foreach (dir ${ARG_INCLUDE_DIRECTORIES})
        target_include_directories (${_name} ${_visibility} $<BUILD_INTERFACE:${dir}>)
    endforeach()
    foreach (var ${ARG_COMPILE_DEFINITIONS})
        target_compile_definitions (${_name} ${_visibility} $<BUILD_INTERFACE:${var}>)
    endforeach()
    foreach (opt ${ARG_COMPILE_OPTIONS})
        target_compile_options (${_name} ${_visibility} $<BUILD_INTERFACE:${opt}>)
    endforeach()
    foreach (opt ${ARG_LINK_OPTIONS})
        target_link_options (${_name} ${_visibility} $<BUILD_INTERFACE:${opt}>)
    endforeach()
    foreach (dir ${ARG_LINK_DIRECTORIES})
        target_link_directories (${_name} ${_visibility} $<BUILD_INTERFACE:${dir}>)
    endforeach()
    foreach (lib ${ARG_LINK_LIBRARIES})
        target_link_libraries (${_name} ${_visibility} $<BUILD_INTERFACE:${lib}>)
    endforeach()
endfunction()

# restore policy
cmake_policy(POP)
