# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#


# set vdec sample path
get_filename_component (AXCL_VDEC_SAMPLE_PATH "${AXCL_ROOT_DIR}/sample/vdec" ABSOLUTE)
get_filename_component (AXCL_SYS_SAMPLE_PATH "${AXCL_ROOT_DIR}/sample/sys" ABSOLUTE)
get_filename_component (AXCL_DEMUX_SAMPLE_PATH "${AXCL_ROOT_DIR}/sample/demux" ABSOLUTE)
get_filename_component (AXCL_TOOLKIT_PATH "${AXCL_ROOT_DIR}/toolkit" ABSOLUTE)
get_filename_component (AXCL_LOGGER_PATH "${AXCL_ROOT_DIR}/logger" ABSOLUTE)

# collect all source files
aux_source_directory (${AXCL_VDEC_SAMPLE_PATH} AXCL_VDEC_SAMPLE_SRCS)
aux_source_directory (${AXCL_DEMUX_SAMPLE_PATH} AXCL_DEMUX_SAMPLE_SRCS)
aux_source_directory (${AXCL_TOOLKIT_LOG_PATH} AXCL_TOOLKIT_LOG_SRCS)

add_sample_app (vdec
    NAME
        axcl_sample_vdec
    SOURCES
        ${AXCL_VDEC_SAMPLE_SRCS}
        ${AXCL_ROOT_DIR}/toolkit/axcl_fifo.c
        ${AXCL_SYS_SAMPLE_PATH}/sample_sys.cpp
        ${AXCL_DEMUX_SAMPLE_SRCS}
        ${AXCL_ROOT_DIR}/toolkit/threadx.cpp
        ${AXCL_ROOT_DIR}/toolkit/event.cpp
        ${AXCL_ROOT_DIR}/toolkit/elapser.cpp
        ${AXCL_ROOT_DIR}/toolkit/dma_buffer.cpp
        ${AXCL_TOOLKIT_LOG_SRCS}
    INCLUDE_DIRECTORIES
        $<BUILD_INTERFACE:${AXCL_INSTALL_INCLUDE_PATH}>
        $<BUILD_INTERFACE:${AXCL_VDEC_SAMPLE_PATH}>
        $<BUILD_INTERFACE:${AXCL_TOOLKIT_PATH}>
        $<BUILD_INTERFACE:${AXCL_LOGGER_PATH}>
        $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/sample>
    LINK_LIBRARIES
        axcl::logger
        axcl::runtime
        axcl::native::sys
        axcl::native::vdec
        3rdparty::ffmpeg
        3rdparty::spdlog
)
