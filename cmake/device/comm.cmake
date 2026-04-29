# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# set comm library name
set (AXCL_DEVICE_COMM_NAME axcl_device_comm)
set (AXCL_DEVICE_COMM_OUTPUT_NAME ax_comm)

# set comm library path
get_filename_component (AXCL_DEVICE_COMM_PATH "${AXCL_ROOT_DIR}/comm" ABSOLUTE)

# set dma buffer size
if (PCIE_DMA_BUFFER_SIZE)
    set (COMM_PCIE_DMA_BUFFER_SIZE ${PCIE_DMA_BUFFER_SIZE})
else()
    set (COMM_PCIE_DMA_BUFFER_SIZE 0x400000)
endif()

# collect all source files
aux_source_directory (${AXCL_DEVICE_COMM_PATH}/impl              AXCL_DEVICE_COMM_SRCS)
aux_source_directory (${AXCL_DEVICE_COMM_PATH}/impl/pcie         AXCL_DEVICE_COMM_PCIE_SRCS)
aux_source_directory (${AXCL_DEVICE_COMM_PATH}/impl/pcie/handler AXCL_DEVICE_COMM_PCIE_SRCS)

# Only collect socket sources if socket is enabled and not on Windows
if (AXCL_COMM_ENABLE_SOCKET AND NOT WIN32)
    aux_source_directory (${AXCL_DEVICE_COMM_PATH}/impl/socket       AXCL_DEVICE_COMM_SOCKET_SRCS)
endif()

# add cmake help function
include (AddProjectLibrary)
include (ConfigProjectVersion)

# add comm shared target
add_project_library (${AXCL_DEVICE_COMM_NAME} SHARED
    SOURCES
        ${AXCL_DEVICE_COMM_SRCS}
        $<$<BOOL:${AXCL_COMM_ENABLE_PCIE}>:${AXCL_DEVICE_COMM_PCIE_SRCS}>
        $<$<BOOL:${AXCL_COMM_ENABLE_SOCKET}>:${AXCL_DEVICE_COMM_SOCKET_SRCS}>
        ${AXCL_ROOT_DIR}/toolkit/dma_buffer.cpp
    PUBLIC
        INCLUDE_DIRECTORIES
            $<BUILD_INTERFACE:${AXCL_DEVICE_COMM_PATH}/include>
    PRIVATE
        COMPILE_DEFINITIONS
            $<BUILD_INTERFACE:PCIE_DMA_BUFFER_SIZE=${COMM_PCIE_DMA_BUFFER_SIZE}>
        INCLUDE_DIRECTORIES
            $<BUILD_INTERFACE:${AXCL_DEVICE_COMM_PATH}/include>
            $<BUILD_INTERFACE:${AXCL_DEVICE_COMM_PATH}/impl>
            $<BUILD_INTERFACE:${AXCL_DEVICE_COMM_PATH}/impl/pcie>
            $<BUILD_INTERFACE:${AXCL_DEVICE_COMM_PATH}/impl/pcie/handler>
            $<$<BOOL:${AXCL_COMM_ENABLE_SOCKET}>:$<BUILD_INTERFACE:${AXCL_DEVICE_COMM_PATH}/impl/socket>>
            $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/toolkit>
            $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/logger>
            $<BUILD_INTERFACE:${AXCL_EXTERNAL_HEADER_PATH}>
            $<BUILD_INTERFACE:${AXCL_INTERNAL_HEADER_PATH}>
        LINK_LIBRARIES
            axcl::logger
            3rdparty::spdlog
            $<$<BOOL:${AXCL_COMM_ENABLE_PCIE}>:3rdparty::bsp::pcie>
            $<$<BOOL:${AXCL_COMM_ENABLE_SOCKET}>:3rdparty::zmq>
    OUTPUT_NAME
        ${AXCL_DEVICE_COMM_OUTPUT_NAME}
    ALIAS
        axcl::device::comm
)

ConfigProjectVersion (${AXCL_DEVICE_COMM_NAME})
