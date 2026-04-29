# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# set skel library name
set (AXCL_SKEL_NAME axcl_skel)

# set skel library path
get_filename_component (AXCL_SKEL_PATH "${AXCL_HOST_PATH}/skel" ABSOLUTE)

# collect all source files
aux_source_directory (${AXCL_SKEL_PATH}/src       AXCL_SKEL_SRCS)
aux_source_directory (${AXCL_SKEL_PATH}/src/api   AXCL_SKEL_SRCS)
aux_source_directory (${AXCL_SKEL_PATH}/src/mgr   AXCL_SKEL_SRCS)
aux_source_directory (${AXCL_SKEL_PATH}/src/impl  AXCL_SKEL_SRCS)
aux_source_directory (${AXCL_SKEL_PATH}/src/hvcp  AXCL_SKEL_SRCS)
aux_source_directory (${AXCL_SKEL_PATH}/src/face  AXCL_SKEL_SRCS)
aux_source_directory (${AXCL_SKEL_PATH}/src/track AXCL_SKEL_SRCS)
aux_source_directory (${AXCL_SKEL_PATH}/src/utils AXCL_SKEL_SRCS)

# add cmake help function
include (AddProjectLibrary)

# add skel shared target
add_project_library (${AXCL_SKEL_NAME} SHARED
    SOURCES
        ${AXCL_SKEL_SRCS}
        ${AXCL_ROOT_DIR}/toolkit/dma_buffer.cpp
    PRIVATE
        INCLUDE_DIRECTORIES
            $<BUILD_INTERFACE:${AXCL_SKEL_PATH}/inc>
            $<BUILD_INTERFACE:${AXCL_SKEL_PATH}/src>
            $<BUILD_INTERFACE:${AXCL_SKEL_PATH}/src/api>
            $<BUILD_INTERFACE:${AXCL_SKEL_PATH}/src/mgr>
            $<BUILD_INTERFACE:${AXCL_SKEL_PATH}/src/impl>
            $<BUILD_INTERFACE:${AXCL_SKEL_PATH}/src/hvcp>
            $<BUILD_INTERFACE:${AXCL_SKEL_PATH}/src/face>
            $<BUILD_INTERFACE:${AXCL_SKEL_PATH}/src/track>
            $<BUILD_INTERFACE:${AXCL_SKEL_PATH}/src/utils>
            $<BUILD_INTERFACE:${AXCL_SKEL_PATH}/src/base>
            $<BUILD_INTERFACE:${AXCL_SKEL_PATH}/src/cv>
            $<BUILD_INTERFACE:${AXCL_SKEL_PATH}/src/middleware>
            $<BUILD_INTERFACE:${AXCL_SKEL_PATH}/src/utilities>
            $<BUILD_INTERFACE:${AXCL_SKEL_PATH}/src/3rdparty/eigen3>
            $<BUILD_INTERFACE:${AXCL_EXTERNAL_HEADER_PATH}>
            $<BUILD_INTERFACE:${AXCL_INTERNAL_HEADER_PATH}>
            $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/toolkit>
            $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/logger>
        LINK_LIBRARIES
            axcl::logger
            axcl::runtime
            axcl::native::sys
            axcl::native::ivps
            3rdparty::bsp::header
            3rdparty::spdlog
    ALIAS
        axcl::skel
)
