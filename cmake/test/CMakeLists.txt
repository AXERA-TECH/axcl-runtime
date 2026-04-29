# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# set test working path
get_filename_component (AXCL_TEST_PATH "${AXCL_ROOT_DIR}/test" ABSOLUTE)

# add cmake help function
include (AddProjectExecutable)

# base sample add function
function (add_test_app path)
    cmake_parse_arguments (ARG
        ""
        "NAME"
        "PATH;SOURCES;INCLUDE_DIRECTORIES;COMPILE_DEFINITIONS;COMPILE_OPTIONS;LINK_OPTIONS;LINK_DIRECTORIES;LINK_LIBRARIES;"
        ${ARGN}
    )

    if (NOT ARG_NAME)
        get_filename_component(path_name ${path} NAME_WE)
        set (name axcl_ut_${path_name})
    else()
        set (name ${ARG_NAME})
    endif ()

    aux_source_directory (${AXCL_TEST_PATH}/${path} AXCL_SAMPLE_SRCS)
    foreach (dir ${ARG_PATH})
        aux_source_directory (${AXCL_TEST_PATH}/${dir} AXCL_SAMPLE_SRCS)
    endforeach()

    add_project_executable (${name}
        INSTALL_BINDIR
            bin/test
        SOURCES
            ${ARG_SOURCES}
            ${AXCL_SAMPLE_SRCS}
        INCLUDE_DIRECTORIES
            $<BUILD_INTERFACE:${AXCL_TEST_PATH}/${ARG_INCLUDE_DIRECTORIES}>
            $<BUILD_INTERFACE:${AXCL_TEST_PATH}/${path}>
            $<BUILD_INTERFACE:${AXCL_TEST_PATH}/utils>
            $<BUILD_INTERFACE:${AXCL_EXTERNAL_HEADER_PATH}>
            $<BUILD_INTERFACE:${AXCL_INTERNAL_HEADER_PATH}>
            $<BUILD_INTERFACE:${HOME_EXTERNAL_HEADER_PATH}>
            $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/toolkit>
            $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/logger>
        COMPILE_DEFINITIONS
            ${ARG_COMPILE_DEFINITIONS}
        COMPILE_OPTIONS
            ${ARG_COMPILE_OPTIONS}
        LINK_OPTIONS
            ${ARG_LINK_OPTIONS}
        LINK_DIRECTORIES
            ${AXCL_TEST_PATH}/${ARG_LINK_DIRECTORIES}
        LINK_LIBRARIES
            ${ARG_LINK_LIBRARIES}
            axcl::host::comm
            axcl::runtime
            3rdparty::cmdline
            3rdparty::gtest
    )
endfunction()

# add package test working path
include (pkg.cmake)

# add npu test working path
include (npu.cmake)

# add runtime test working path
include (runtime.cmake)

# add system test working path
include (sys.cmake)

if (NOT SYSTEM_IS_WINDOWS)
    # add vdec test working path
    include (vdec.cmake)

    # add ive test working path
    include (ive.cmake)

    # add venc test working path
    include (venc.cmake)
endif()