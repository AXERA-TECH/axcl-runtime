# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# set comm library name
set (AXCL_DEVICE_DAEMON_NAME slave_daemon)
set (AXCL_DEVICE_DAEMON_OUTPUT_NAME slave_daemon)

# set comm library path
get_filename_component (AXCL_DEVICE_DAEMON_PATH "${AXCL_DEVICE_PATH}/daemon" ABSOLUTE)

# collect all source files
aux_source_directory (${AXCL_DEVICE_DAEMON_PATH}                    AXCL_DEVICE_DAEMON_SRCS)
aux_source_directory (${AXCL_DEVICE_DAEMON_PATH}/device             AXCL_DEVICE_DAEMON_SRCS)
aux_source_directory (${AXCL_DEVICE_DAEMON_PATH}/port               AXCL_DEVICE_DAEMON_SRCS)
aux_source_directory (${AXCL_DEVICE_DAEMON_PATH}/smi                AXCL_DEVICE_DAEMON_SRCS)
aux_source_directory (${AXCL_DEVICE_DAEMON_PATH}/smi/channel        AXCL_DEVICE_DAEMON_SRCS)
aux_source_directory (${AXCL_DEVICE_DAEMON_PATH}/smi/stream         AXCL_DEVICE_DAEMON_SRCS)
aux_source_directory (${AXCL_DEVICE_DAEMON_PATH}/smi/impl           AXCL_DEVICE_DAEMON_SRCS)
aux_source_directory (${AXCL_DEVICE_DAEMON_PATH}/smi/runtime        AXCL_DEVICE_DAEMON_SRCS)
aux_source_directory (${AXCL_DEVICE_DAEMON_PATH}/smi/runtime/system AXCL_DEVICE_DAEMON_SRCS)

# add cmake help function
include (AddProjectExecutable)

# add daemon executable target
add_project_executable (${AXCL_DEVICE_DAEMON_NAME}
    SOURCES
        ${AXCL_DEVICE_DAEMON_SRCS}
        ${AXCL_ROOT_DIR}/toolkit/elapser.cpp
        ${AXCL_ROOT_DIR}/toolkit/event.cpp
        ${AXCL_ROOT_DIR}/toolkit/threadx.cpp
    INCLUDE_DIRECTORIES
        $<BUILD_INTERFACE:${AXCL_DEVICE_DAEMON_PATH}>
        $<BUILD_INTERFACE:${AXCL_DEVICE_DAEMON_PATH}/device>
        $<BUILD_INTERFACE:${AXCL_DEVICE_DAEMON_PATH}/port>
        $<BUILD_INTERFACE:${AXCL_DEVICE_DAEMON_PATH}/smi>
        $<BUILD_INTERFACE:${AXCL_DEVICE_DAEMON_PATH}/smi/base>
        $<BUILD_INTERFACE:${AXCL_DEVICE_DAEMON_PATH}/smi/channel>
        $<BUILD_INTERFACE:${AXCL_DEVICE_DAEMON_PATH}/smi/stream>
        $<BUILD_INTERFACE:${AXCL_DEVICE_DAEMON_PATH}/smi/impl>
        $<BUILD_INTERFACE:${AXCL_DEVICE_DAEMON_PATH}/smi/runtime>
        $<BUILD_INTERFACE:${AXCL_DEVICE_DAEMON_PATH}/smi/runtime/system>
        $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/toolkit>
        $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/logger>
        $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/comm/include>
        $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/protocol>
        $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/protocol/include>
        $<BUILD_INTERFACE:${AXCL_EXTERNAL_HEADER_PATH}>
        $<BUILD_INTERFACE:${AXCL_INTERNAL_HEADER_PATH}>
    LINK_LIBRARIES
        axcl::logger
        axcl::device::package
        3rdparty::bsp::header
        3rdparty::protobuf
        3rdparty::spdlog
    OUTPUT_NAME
        ${AXCL_DEVICE_DAEMON_OUTPUT_NAME}
)
