# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# set PCIe Windows driver library names
set (AXCL_PCIE_MSG_NAME axcl_pcie_msg)
set (AXCL_PCIE_DMA_NAME axcl_pcie_dma)

# set PCIe Windows driver library path
get_filename_component (AXCL_PCIE_WIN64_PATH "${AXCL_ROOT_DIR}/drv/win64" ABSOLUTE)

# add cmake help function
include (AddProjectLibrary)

#
# Windows PCIe Message Library
#
add_project_library (${AXCL_PCIE_MSG_NAME} SHARED
    SOURCES
        ${AXCL_PCIE_WIN64_PATH}/msg/ax_pcie_msg.cpp
        ${AXCL_PCIE_WIN64_PATH}/msg/version.c
    PUBLIC
        INCLUDE_DIRECTORIES
            $<BUILD_INTERFACE:${AXCL_EXTERNAL_HEADER_PATH}>
            $<BUILD_INTERFACE:${AXCL_INTERNAL_HEADER_PATH}>
            $<BUILD_INTERFACE:${HOME_EXTERNAL_HEADER_PATH}>
    PRIVATE
        COMPILE_DEFINITIONS
            $<$<BOOL:${SYSTEM_IS_WINDOWS}>:AXCL_EXPORTS>
        INCLUDE_DIRECTORIES
            $<BUILD_INTERFACE:${AXCL_EXTERNAL_HEADER_PATH}>
            $<BUILD_INTERFACE:${AXCL_INTERNAL_HEADER_PATH}>
            $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/toolkit>
            $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/logger>
        LINK_LIBRARIES
            axcl::logger
            3rdparty::spdlog
    ALIAS
        axcl::driver::pcie_msg
    HIDE_SYMBOL
        ON
)

#
# Windows PCIe DMA Library
#
add_project_library (${AXCL_PCIE_DMA_NAME} SHARED
    SOURCES
        ${AXCL_PCIE_WIN64_PATH}/dma/ax_pcie_dma.cpp
        ${AXCL_PCIE_WIN64_PATH}/dma/version.c
    PUBLIC
        INCLUDE_DIRECTORIES
            $<BUILD_INTERFACE:${AXCL_EXTERNAL_HEADER_PATH}>
            $<BUILD_INTERFACE:${AXCL_INTERNAL_HEADER_PATH}>
            $<BUILD_INTERFACE:${HOME_EXTERNAL_HEADER_PATH}>
    PRIVATE
        COMPILE_DEFINITIONS
            $<$<BOOL:${SYSTEM_IS_WINDOWS}>:AXCL_EXPORTS>
        INCLUDE_DIRECTORIES
            $<BUILD_INTERFACE:${AXCL_EXTERNAL_HEADER_PATH}>
            $<BUILD_INTERFACE:${AXCL_INTERNAL_HEADER_PATH}>
            $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/toolkit>
            $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/logger>
        LINK_LIBRARIES
            axcl::logger
            3rdparty::spdlog
            ${AXCL_PCIE_MSG_NAME}
    ALIAS
        axcl::driver::pcie_dma
    HIDE_SYMBOL
        ON
)

#
# Create interface targets for BSP compatibility
#
add_project_library (axcl_pcie_win64 INTERFACE
    PUBLIC
        INCLUDE_DIRECTORIES
            $<BUILD_INTERFACE:${AXCL_EXTERNAL_HEADER_PATH}>
            $<BUILD_INTERFACE:${HOME_EXTERNAL_HEADER_PATH}>
        LINK_LIBRARIES
            ${AXCL_PCIE_MSG_NAME}
            ${AXCL_PCIE_DMA_NAME}
    ALIAS
        3rdparty::bsp::axcl_pcie_win64
)

# install win64 pcie driver files
file(GLOB DRV_FILES "${AXCL_PCIE_WIN64_PATH}/drv/bin/*")
install(
    FILES ${DRV_FILES}
    DESTINATION drv
)
