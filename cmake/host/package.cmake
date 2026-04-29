# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# set bsp library name
set (AXCL_HOST_PKG_NAME axcl_pkg)

# set bsp library path
get_filename_component (AXCL_PROTO_GEN_PATH "${CMAKE_BINARY_DIR}/proto" ABSOLUTE)
get_filename_component (AXCL_PROTO_INC_PATH "${AXCL_ROOT_DIR}/protocol/include" ABSOLUTE)
get_filename_component (AXCL_HOST_INC_PATH  "${AXCL_ROOT_DIR}/protocol" ABSOLUTE)
get_filename_component (AXCL_HOST_PKG_PATH  "${AXCL_ROOT_DIR}/protocol/package" ABSOLUTE)

# collect all source files
aux_source_directory (${AXCL_HOST_PKG_PATH}                                 AXCL_HOST_PKG_SRCS)
aux_source_directory (${AXCL_HOST_PKG_PATH}/host                            AXCL_HOST_PKG_SRCS)
aux_source_directory (${AXCL_HOST_PKG_PATH}/host/native                     AXCL_HOST_PKG_SRCS)
aux_source_directory (${AXCL_HOST_PKG_PATH}/host/native/module              AXCL_HOST_PKG_SRCS)
aux_source_directory (${AXCL_HOST_PKG_PATH}/host/runtime                    AXCL_HOST_PKG_SRCS)
aux_source_directory (${AXCL_HOST_PKG_PATH}/host/runtime/memory             AXCL_HOST_PKG_SRCS)
aux_source_directory (${AXCL_HOST_PKG_PATH}/host/runtime/engine             AXCL_HOST_PKG_SRCS)
aux_source_directory (${AXCL_HOST_PKG_PATH}/host/runtime/p2p                AXCL_HOST_PKG_SRCS)
aux_source_directory (${AXCL_HOST_PKG_PATH}/host/runtime/system             AXCL_HOST_PKG_SRCS)
aux_source_directory (${AXCL_HOST_PKG_PATH}/host/runtime/system/control     AXCL_HOST_PKG_SRCS)
aux_source_directory (${AXCL_HOST_PKG_PATH}/host/runtime/system/latency     AXCL_HOST_PKG_SRCS)
aux_source_directory (${AXCL_HOST_PKG_PATH}/host/runtime/system/log         AXCL_HOST_PKG_SRCS)
aux_source_directory (${AXCL_HOST_PKG_PATH}/host/runtime/usrwork            AXCL_HOST_PKG_SRCS)

aux_source_directory (${AXCL_HOST_PKG_PATH}/device                          AXCL_HOST_PKG_SRCS)
aux_source_directory (${AXCL_HOST_PKG_PATH}/device/native                   AXCL_HOST_PKG_SRCS)
aux_source_directory (${AXCL_HOST_PKG_PATH}/device/native/module            AXCL_HOST_PKG_SRCS)
aux_source_directory (${AXCL_HOST_PKG_PATH}/device/runtime                  AXCL_HOST_PKG_SRCS)
aux_source_directory (${AXCL_HOST_PKG_PATH}/device/runtime/memory           AXCL_HOST_PKG_SRCS)
aux_source_directory (${AXCL_HOST_PKG_PATH}/device/runtime/engine           AXCL_HOST_PKG_SRCS)
aux_source_directory (${AXCL_HOST_PKG_PATH}/device/runtime/p2p              AXCL_HOST_PKG_SRCS)
aux_source_directory (${AXCL_HOST_PKG_PATH}/device/runtime/system           AXCL_HOST_PKG_SRCS)
aux_source_directory (${AXCL_HOST_PKG_PATH}/device/runtime/system/control   AXCL_HOST_PKG_SRCS)
aux_source_directory (${AXCL_HOST_PKG_PATH}/device/runtime/system/latency   AXCL_HOST_PKG_SRCS)
aux_source_directory (${AXCL_HOST_PKG_PATH}/device/runtime/system/log       AXCL_HOST_PKG_SRCS)
aux_source_directory (${AXCL_HOST_PKG_PATH}/device/runtime/usrwork          AXCL_HOST_PKG_SRCS)

aux_source_directory (${AXCL_ROOT_DIR}/toolkit/log                          AXCL_HOST_LOG_SRCS)

# add cmake help function
include (AddProjectLibrary)

# add sys interface target
add_project_library (${AXCL_HOST_PKG_NAME} STATIC
    SOURCES
        ${AXCL_HOST_PKG_SRCS}
        ${AXCL_HOST_LOG_SRCS}
    PUBLIC
        INCLUDE_DIRECTORIES
            $<BUILD_INTERFACE:${AXCL_PROTO_GEN_PATH}>
            $<BUILD_INTERFACE:${AXCL_PROTO_INC_PATH}>
            $<BUILD_INTERFACE:${AXCL_HOST_INC_PATH}>
            $<BUILD_INTERFACE:${AXCL_HOST_PKG_PATH}>
    PRIVATE
        COMPILE_DEFINITIONS
            $<$<BOOL:${SYSTEM_IS_WINDOWS}>:AXCL_EXPORTS>
        INCLUDE_DIRECTORIES
            $<BUILD_INTERFACE:${AXCL_PROTO_INC_PATH}>
            $<BUILD_INTERFACE:${AXCL_HOST_PKG_PATH}>
            $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/protocol>
            $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/protocol/include>
            $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/toolkit>
            $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/logger>
            $<BUILD_INTERFACE:${AXCL_EXTERNAL_HEADER_PATH}>
            $<BUILD_INTERFACE:${AXCL_INTERNAL_HEADER_PATH}>
            $<BUILD_INTERFACE:${HOME_EXTERNAL_HEADER_PATH}>
        LINK_LIBRARIES
            axcl::logger
            axcl::host::protocol
            3rdparty::bsp::header
            3rdparty::protobuf
            3rdparty::spdlog
    ALIAS
        axcl::host::package
)
