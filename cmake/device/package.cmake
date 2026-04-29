# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# set bsp library name
set (AXCL_DEVICE_PKG_NAME axcl_device_pkg)
set (AXCL_DEVICE_PKG_OUTPUT_NAME ax_pkg)

# set bsp library path
get_filename_component (AXCL_PROTO_GEN_PATH  "${CMAKE_BINARY_DIR}/proto" ABSOLUTE)
get_filename_component (AXCL_PROTO_INC_PATH  "${AXCL_ROOT_DIR}/protocol/include" ABSOLUTE)
get_filename_component (AXCL_DEVICE_INC_PATH "${AXCL_ROOT_DIR}/protocol" ABSOLUTE)
get_filename_component (AXCL_DEVICE_PKG_PATH "${AXCL_ROOT_DIR}/protocol/package" ABSOLUTE)

# collect all source files
aux_source_directory (${AXCL_DEVICE_PKG_PATH}                               AXCL_DEVICE_PKG_SRCS)
aux_source_directory (${AXCL_DEVICE_PKG_PATH}/device                        AXCL_DEVICE_PKG_SRCS)
aux_source_directory (${AXCL_DEVICE_PKG_PATH}/device/native                 AXCL_DEVICE_PKG_SRCS)
aux_source_directory (${AXCL_DEVICE_PKG_PATH}/device/native/module          AXCL_DEVICE_PKG_SRCS)
aux_source_directory (${AXCL_DEVICE_PKG_PATH}/device/runtime                AXCL_DEVICE_PKG_SRCS)
aux_source_directory (${AXCL_DEVICE_PKG_PATH}/device/runtime/p2p            AXCL_DEVICE_PKG_SRCS)
aux_source_directory (${AXCL_DEVICE_PKG_PATH}/device/runtime/memory         AXCL_DEVICE_PKG_SRCS)
aux_source_directory (${AXCL_DEVICE_PKG_PATH}/device/runtime/engine         AXCL_DEVICE_PKG_SRCS)
aux_source_directory (${AXCL_DEVICE_PKG_PATH}/device/runtime/p2p            AXCL_DEVICE_PKG_SRCS)
aux_source_directory (${AXCL_DEVICE_PKG_PATH}/device/runtime/system         AXCL_DEVICE_PKG_SRCS)
aux_source_directory (${AXCL_DEVICE_PKG_PATH}/device/runtime/system/control AXCL_DEVICE_PKG_SRCS)
aux_source_directory (${AXCL_DEVICE_PKG_PATH}/device/runtime/system/latency AXCL_DEVICE_PKG_SRCS)
aux_source_directory (${AXCL_DEVICE_PKG_PATH}/device/runtime/system/log     AXCL_DEVICE_PKG_SRCS)
aux_source_directory (${AXCL_DEVICE_PKG_PATH}/device/runtime/usrwork        AXCL_DEVICE_PKG_SRCS)
aux_source_directory (${AXCL_ROOT_DIR}/toolkit/log                          AXCL_DEVICE_LOG_SRCS)

# add cmake help function
include (AddProjectLibrary)
include (ConfigProjectVersion)

# add sys interface target
add_project_library (${AXCL_DEVICE_PKG_NAME} SHARED
    SOURCES
        ${AXCL_DEVICE_PKG_SRCS}
        ${AXCL_DEVICE_LOG_SRCS}
    PUBLIC
        INCLUDE_DIRECTORIES
            $<BUILD_INTERFACE:${AXCL_PROTO_GEN_PATH}>
            $<BUILD_INTERFACE:${AXCL_PROTO_INC_PATH}>
            $<BUILD_INTERFACE:${AXCL_DEVICE_INC_PATH}>
            $<BUILD_INTERFACE:${AXCL_DEVICE_PKG_PATH}>
    PRIVATE
        INCLUDE_DIRECTORIES
            $<BUILD_INTERFACE:${AXCL_DEVICE_PKG_PATH}>
            $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/protocol>
            $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/protocol/include>
            $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/toolkit>
            $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/logger>
            $<BUILD_INTERFACE:${AXCL_EXTERNAL_HEADER_PATH}>
            $<BUILD_INTERFACE:${AXCL_INTERNAL_HEADER_PATH}>
        LINK_LIBRARIES
            axcl::logger
            axcl::device::protocol
            3rdparty::bsp::header
            3rdparty::spdlog
            3rdparty::protobuf
        OUTPUT_NAME
            ${AXCL_DEVICE_PKG_OUTPUT_NAME}
    ALIAS
        axcl::device::package
)

ConfigProjectVersion (${AXCL_DEVICE_PKG_NAME})
