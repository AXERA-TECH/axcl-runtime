# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# set comm library name
set (AXCL_DEVICE_WORKER_NAME slave_worker)

# set comm library path
get_filename_component (AXCL_DEVICE_WORKER_PATH "${AXCL_DEVICE_PATH}/worker" ABSOLUTE)

# collect all source files
aux_source_directory (${AXCL_DEVICE_WORKER_PATH}                    AXCL_DEVICE_WORKER_SRCS)
aux_source_directory (${AXCL_DEVICE_WORKER_PATH}/device             AXCL_DEVICE_WORKER_SRCS)
aux_source_directory (${AXCL_DEVICE_WORKER_PATH}/port               AXCL_DEVICE_WORKER_SRCS)
aux_source_directory (${AXCL_DEVICE_WORKER_PATH}/channel            AXCL_DEVICE_WORKER_SRCS)
aux_source_directory (${AXCL_DEVICE_WORKER_PATH}/dispatch           AXCL_DEVICE_WORKER_SRCS)
aux_source_directory (${AXCL_DEVICE_WORKER_PATH}/context            AXCL_DEVICE_WORKER_SRCS)
aux_source_directory (${AXCL_DEVICE_WORKER_PATH}/stream             AXCL_DEVICE_WORKER_SRCS)
aux_source_directory (${AXCL_DEVICE_WORKER_PATH}/event              AXCL_DEVICE_WORKER_SRCS)
aux_source_directory (${AXCL_DEVICE_WORKER_PATH}/task               AXCL_DEVICE_WORKER_SRCS)
aux_source_directory (${AXCL_DEVICE_WORKER_PATH}/impl               AXCL_DEVICE_WORKER_SRCS)
aux_source_directory (${AXCL_DEVICE_WORKER_PATH}/runtime            AXCL_DEVICE_WORKER_SRCS)
aux_source_directory (${AXCL_DEVICE_WORKER_PATH}/runtime/system     AXCL_DEVICE_WORKER_SRCS)
aux_source_directory (${AXCL_DEVICE_WORKER_PATH}/runtime/memory     AXCL_DEVICE_WORKER_SRCS)
aux_source_directory (${AXCL_DEVICE_WORKER_PATH}/runtime/engine     AXCL_DEVICE_WORKER_SRCS)
aux_source_directory (${AXCL_DEVICE_WORKER_PATH}/runtime/p2p        AXCL_DEVICE_WORKER_SRCS)
aux_source_directory (${AXCL_DEVICE_WORKER_PATH}/runtime/usrwork    AXCL_DEVICE_WORKER_SRCS)
aux_source_directory (${AXCL_DEVICE_WORKER_PATH}/native             AXCL_DEVICE_WORKER_SRCS)
aux_source_directory (${AXCL_DEVICE_WORKER_PATH}/native/dmadim      AXCL_DEVICE_WORKER_SRCS)
aux_source_directory (${AXCL_DEVICE_WORKER_PATH}/native/ive         AXCL_DEVICE_WORKER_SRCS)
aux_source_directory (${AXCL_DEVICE_WORKER_PATH}/native/ivps        AXCL_DEVICE_WORKER_SRCS)
aux_source_directory (${AXCL_DEVICE_WORKER_PATH}/native/npu         AXCL_DEVICE_WORKER_SRCS)
aux_source_directory (${AXCL_DEVICE_WORKER_PATH}/native/sys         AXCL_DEVICE_WORKER_SRCS)
aux_source_directory (${AXCL_DEVICE_WORKER_PATH}/native/vdec        AXCL_DEVICE_WORKER_SRCS)
aux_source_directory (${AXCL_DEVICE_WORKER_PATH}/native/venc        AXCL_DEVICE_WORKER_SRCS)
aux_source_directory (${AXCL_DEVICE_WORKER_PATH}/native/dsp         AXCL_DEVICE_WORKER_SRCS)
aux_source_directory (${AXCL_DEVICE_WORKER_PATH}/native/dsp         AXCL_DEVICE_WORKER_SRCS)
aux_source_directory (${AXCL_DEVICE_PATH}/usrworker/src             AXCL_DEVICE_WORKER_SRCS)

# add cmake help function
include (AddProjectExecutable)
include (ConfigProjectVersion)

# add worker executable target
add_project_executable (${AXCL_DEVICE_WORKER_NAME}
    SOURCES
        ${AXCL_DEVICE_WORKER_SRCS}
        ${AXCL_ROOT_DIR}/toolkit/elapser.cpp
        ${AXCL_ROOT_DIR}/toolkit/event.cpp
        ${AXCL_ROOT_DIR}/toolkit/threadx.cpp
    INCLUDE_DIRECTORIES
        $<BUILD_INTERFACE:${AXCL_DEVICE_WORKER_PATH}>
        $<BUILD_INTERFACE:${AXCL_DEVICE_WORKER_PATH}/device>
        $<BUILD_INTERFACE:${AXCL_DEVICE_WORKER_PATH}/port>
        $<BUILD_INTERFACE:${AXCL_DEVICE_WORKER_PATH}/channel>
        $<BUILD_INTERFACE:${AXCL_DEVICE_WORKER_PATH}/dispatch>
        $<BUILD_INTERFACE:${AXCL_DEVICE_WORKER_PATH}/context>
        $<BUILD_INTERFACE:${AXCL_DEVICE_WORKER_PATH}/stream>
        $<BUILD_INTERFACE:${AXCL_DEVICE_WORKER_PATH}/event>
        $<BUILD_INTERFACE:${AXCL_DEVICE_WORKER_PATH}/task>
        $<BUILD_INTERFACE:${AXCL_DEVICE_WORKER_PATH}/impl>
        $<BUILD_INTERFACE:${AXCL_DEVICE_WORKER_PATH}/base>
        $<BUILD_INTERFACE:${AXCL_DEVICE_WORKER_PATH}/runtime>
        $<BUILD_INTERFACE:${AXCL_DEVICE_WORKER_PATH}/runtime/system>
        $<BUILD_INTERFACE:${AXCL_DEVICE_WORKER_PATH}/runtime/memory>
        $<BUILD_INTERFACE:${AXCL_DEVICE_WORKER_PATH}/runtime/engine>
        $<BUILD_INTERFACE:${AXCL_DEVICE_WORKER_PATH}/runtime/p2p>
        $<BUILD_INTERFACE:${AXCL_DEVICE_WORKER_PATH}/runtime/usrwork>
        $<BUILD_INTERFACE:${AXCL_DEVICE_WORKER_PATH}/native>
        $<BUILD_INTERFACE:${AXCL_DEVICE_WORKER_PATH}/native/dmadim>
        $<BUILD_INTERFACE:${AXCL_DEVICE_WORKER_PATH}/native/ive>
        $<BUILD_INTERFACE:${AXCL_DEVICE_WORKER_PATH}/native/ivps>
        $<BUILD_INTERFACE:${AXCL_DEVICE_WORKER_PATH}/native/npu>
        $<BUILD_INTERFACE:${AXCL_DEVICE_WORKER_PATH}/native/sys>
        $<BUILD_INTERFACE:${AXCL_DEVICE_WORKER_PATH}/native/vdec>
        $<BUILD_INTERFACE:${AXCL_DEVICE_WORKER_PATH}/native/venc>
        $<BUILD_INTERFACE:${AXCL_DEVICE_WORKER_PATH}/native/dsp>
        $<BUILD_INTERFACE:${AXCL_DEVICE_PATH}/usrworker/src>
        $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/toolkit>
        $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/logger>
        $<BUILD_INTERFACE:${AXCL_EXTERNAL_HEADER_PATH}>
        $<BUILD_INTERFACE:${AXCL_INTERNAL_HEADER_PATH}>
    LINK_LIBRARIES
        axcl::logger
        3rdparty::bsp
        3rdparty::spdlog
        3rdparty::protobuf
        axcl::device::package
        axcl::device::comm
)

ConfigProjectVersion (${AXCL_DEVICE_WORKER_NAME})
