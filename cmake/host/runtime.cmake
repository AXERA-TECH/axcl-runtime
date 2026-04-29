# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# set bsp library name
set (AXCL_RUNTIME_NAME axcl_rt)

# set bsp library path
get_filename_component (AXCL_RUNTIME_PATH "${AXCL_HOST_PATH}/runtime" ABSOLUTE)

# collect all source files
aux_source_directory (${AXCL_RUNTIME_PATH}            AXCL_RUNTIME_SRCS)
aux_source_directory (${AXCL_RUNTIME_PATH}/channel    AXCL_RUNTIME_SRCS)
aux_source_directory (${AXCL_RUNTIME_PATH}/config     AXCL_RUNTIME_SRCS)
aux_source_directory (${AXCL_RUNTIME_PATH}/context    AXCL_RUNTIME_SRCS)
aux_source_directory (${AXCL_RUNTIME_PATH}/device     AXCL_RUNTIME_SRCS)
aux_source_directory (${AXCL_RUNTIME_PATH}/engine     AXCL_RUNTIME_SRCS)
aux_source_directory (${AXCL_RUNTIME_PATH}/event      AXCL_RUNTIME_SRCS)
aux_source_directory (${AXCL_RUNTIME_PATH}/memory     AXCL_RUNTIME_SRCS)
aux_source_directory (${AXCL_RUNTIME_PATH}/p2p        AXCL_RUNTIME_SRCS)
aux_source_directory (${AXCL_RUNTIME_PATH}/stream     AXCL_RUNTIME_SRCS)
aux_source_directory (${AXCL_RUNTIME_PATH}/system     AXCL_RUNTIME_SRCS)
aux_source_directory (${AXCL_RUNTIME_PATH}/task       AXCL_RUNTIME_SRCS)
aux_source_directory (${AXCL_RUNTIME_PATH}/usrwork    AXCL_RUNTIME_SRCS)
aux_source_directory (${AXCL_ROOT_DIR}/toolkit/log    AXCL_RUNTIME_LOG_SRCS)

# add cmake help function
include (AddProjectLibrary)

# add sys interface target
add_project_library (${AXCL_RUNTIME_NAME} SHARED
    SOURCES
        ${AXCL_RUNTIME_SRCS}
        ${AXCL_RUNTIME_LOG_SRCS}
        ${AXCL_ROOT_DIR}/toolkit/elapser.cpp
        ${AXCL_ROOT_DIR}/toolkit/event.cpp
        ${AXCL_ROOT_DIR}/toolkit/threadx.cpp
    PRIVATE
        COMPILE_DEFINITIONS
            $<$<BOOL:${SYSTEM_IS_WINDOWS}>:AXCL_EXPORTS>
        INCLUDE_DIRECTORIES
            $<BUILD_INTERFACE:${AXCL_RUNTIME_PATH}>
            $<BUILD_INTERFACE:${HOME_EXTERNAL_HEADER_PATH}>
            $<BUILD_INTERFACE:${AXCL_EXTERNAL_HEADER_PATH}>
            $<BUILD_INTERFACE:${AXCL_INTERNAL_HEADER_PATH}>
            $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/toolkit>
            $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/logger>
        LINK_LIBRARIES
            axcl::logger
            axcl::host::comm
            axcl::host::package
            axcl::host::token
            3rdparty::bsp::header
            3rdparty::spdlog
            $<$<NOT:$<BOOL:${WIN32}>>:3rdparty::zmq>
            3rdparty::cjson
    ALIAS
        axcl::runtime
)
