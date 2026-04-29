# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# set bsp library name
set (BSP_NAME ax_bsp)

# set bsp library path
if (EXISTS ${AXCL_ROOT_DIR}/../msp)
    get_filename_component (BSP_PATH "${AXCL_ROOT_DIR}/../msp/out" ABSOLUTE)
    get_filename_component (BSP_3RD_PATH "${AXCL_ROOT_DIR}/../third-party" ABSOLUTE)
else()
    get_filename_component (BSP_PATH "${AXCL_ROOT_DIR}/3rdparty/bsp" ABSOLUTE)
endif()

message (STATUS "BSP PATH: ${BSP_PATH}")

# add cmake help function
include (AddProjectLibrary)

# add dma interface target
add_project_library (${BSP_NAME}_header INTERFACE
    PUBLIC INCLUDE_DIRECTORIES
        $<BUILD_INTERFACE:${BSP_PATH}/include>
    ALIAS
        3rdparty::bsp::header
)

# add dma interface target
add_project_library (${BSP_NAME}_dma INTERFACE
    PUBLIC
        INCLUDE_DIRECTORIES
            $<BUILD_INTERFACE:${BSP_PATH}/include>
        LINK_LIBRARIES
            ax_dmadim
        LINK_DIRECTORIES
            $<BUILD_INTERFACE:${BSP_PATH}/lib>
    ALIAS
        3rdparty::bsp::dma
)

# add dma interface target
add_project_library (${BSP_NAME}_dsp INTERFACE
    PUBLIC
        INCLUDE_DIRECTORIES
            $<BUILD_INTERFACE:${BSP_PATH}/include>
        LINK_LIBRARIES
            ax_dsp
        LINK_DIRECTORIES
            $<BUILD_INTERFACE:${BSP_PATH}/lib>
    ALIAS
        3rdparty::bsp::dsp
)

# add sys interface target
add_project_library (${BSP_NAME}_sys INTERFACE
    PUBLIC
        INCLUDE_DIRECTORIES
            $<BUILD_INTERFACE:${BSP_PATH}/include>
        LINK_LIBRARIES
            ax_sys
            3rdparty::bsp::dma
        LINK_DIRECTORIES
            $<BUILD_INTERFACE:${BSP_PATH}/lib>
    ALIAS
        3rdparty::bsp::sys
)

# add engine interface target
add_project_library (${BSP_NAME}_engine INTERFACE
    PUBLIC
        INCLUDE_DIRECTORIES
            $<BUILD_INTERFACE:${BSP_PATH}/include>
        LINK_LIBRARIES
            ax_engine
            ax_interpreter
        LINK_DIRECTORIES
            $<BUILD_INTERFACE:${BSP_PATH}/lib>
    ALIAS
        3rdparty::bsp::engine
)

# add ive interface target
add_project_library (${BSP_NAME}_ive INTERFACE
    PUBLIC
        INCLUDE_DIRECTORIES
            $<BUILD_INTERFACE:${BSP_PATH}/include>
        LINK_LIBRARIES
            ax_ive
        LINK_DIRECTORIES
            $<BUILD_INTERFACE:${BSP_PATH}/lib>
    ALIAS
        3rdparty::bsp::ive
)

# add ivps interface target
add_project_library (${BSP_NAME}_ivps INTERFACE
    PUBLIC
        INCLUDE_DIRECTORIES
            $<BUILD_INTERFACE:${BSP_PATH}/include>
        LINK_LIBRARIES
            ax_ivps
        LINK_DIRECTORIES
            $<BUILD_INTERFACE:${BSP_PATH}/lib>
    ALIAS
        3rdparty::bsp::ivps
)

# add vdec interface target
add_project_library (${BSP_NAME}_vdec INTERFACE
    PUBLIC
        INCLUDE_DIRECTORIES
            $<BUILD_INTERFACE:${BSP_PATH}/include>
        LINK_LIBRARIES
            ax_vdec
        LINK_DIRECTORIES
            $<BUILD_INTERFACE:${BSP_PATH}/lib>
    ALIAS
        3rdparty::bsp::vdec
)

# add venc interface target
add_project_library (${BSP_NAME}_venc INTERFACE
    PUBLIC
        INCLUDE_DIRECTORIES
            $<BUILD_INTERFACE:${BSP_PATH}/include>
        LINK_LIBRARIES
            ax_venc
            exif
        LINK_DIRECTORIES
            $<BUILD_INTERFACE:${BSP_PATH}/lib>
            $<BUILD_INTERFACE:${BSP_3RD_PATH}/libexif/lib>
    ALIAS
        3rdparty::bsp::venc
)

# add engine interface target
add_project_library (${BSP_NAME}_pcie INTERFACE
    PUBLIC
        INCLUDE_DIRECTORIES
            $<BUILD_INTERFACE:${BSP_PATH}/include>
        LINK_LIBRARIES
            ax_pcie_msg
            ax_pcie_dma
        LINK_DIRECTORIES
            $<BUILD_INTERFACE:${BSP_PATH}/lib>
    ALIAS
        3rdparty::bsp::pcie
)

# add engine interface target
add_project_library (${BSP_NAME}_axcl_pcie INTERFACE
    PUBLIC
        INCLUDE_DIRECTORIES
            $<BUILD_INTERFACE:${BSP_PATH}/include>
        LINK_LIBRARIES
            axcl_pcie_msg
            axcl_pcie_dma
        LINK_DIRECTORIES
            $<BUILD_INTERFACE:${BSP_PATH}/lib>
    ALIAS
        3rdparty::bsp::axcl_pcie
)

# add 3rdparty interface target
add_project_library (${BSP_NAME} INTERFACE
    PUBLIC
        LINK_LIBRARIES
            3rdparty::bsp::header
            3rdparty::bsp::sys
            3rdparty::bsp::engine
            3rdparty::bsp::ive
            3rdparty::bsp::ivps
            3rdparty::bsp::vdec
            3rdparty::bsp::venc
            3rdparty::bsp::pcie
            3rdparty::bsp::dsp
    ALIAS
        3rdparty::bsp
)
