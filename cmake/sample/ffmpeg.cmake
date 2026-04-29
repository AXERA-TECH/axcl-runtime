# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#


# set ffmpeg sample paths
get_filename_component (AXCL_FFMPEG_VENC_SAMPLE_PATH "${AXCL_ROOT_DIR}/sample/ffmpeg/venc" ABSOLUTE)
get_filename_component (AXCL_FFMPEG_VDEC_SAMPLE_PATH "${AXCL_ROOT_DIR}/sample/ffmpeg/vdec" ABSOLUTE)
get_filename_component (AXCL_DEMUX_SAMPLE_PATH "${AXCL_ROOT_DIR}/sample/demux" ABSOLUTE)

# collect source files for venc
aux_source_directory (${AXCL_FFMPEG_VENC_SAMPLE_PATH} AXCL_FFMPEG_VENC_SAMPLE_SRCS)

# collect source files for vdec
aux_source_directory (${AXCL_FFMPEG_VDEC_SAMPLE_PATH} AXCL_FFMPEG_VDEC_SAMPLE_SRCS)

# collect demux source files
aux_source_directory (${AXCL_DEMUX_SAMPLE_PATH} AXCL_DEMUX_SAMPLE_SRCS)

# add ffmpeg venc sample
add_sample_app (ffmpeg/venc
    NAME
        axcl_ffmpeg_venc
    SOURCES
        ${AXCL_FFMPEG_VENC_SAMPLE_SRCS}
        ${AXCL_DEMUX_SAMPLE_SRCS}
        ${AXCL_ROOT_DIR}/toolkit/axcl_fifo.c
        ${AXCL_ROOT_DIR}/toolkit/threadx.cpp
        ${AXCL_ROOT_DIR}/toolkit/event.cpp
        ${AXCL_ROOT_DIR}/toolkit/elapser.cpp
    INCLUDE_DIRECTORIES
        ${AXCL_ROOT_DIR}/sample
    LINK_LIBRARIES
        axcl::runtime
        axcl::native::sys
        axcl::native::venc
        3rdparty::ffmpeg
        3rdparty::cmdline
)

# add ffmpeg vdec sample
add_sample_app (ffmpeg/vdec
    NAME
        axcl_ffmpeg_vdec
    SOURCES
        ${AXCL_FFMPEG_VDEC_SAMPLE_SRCS}
        ${AXCL_DEMUX_SAMPLE_SRCS}
        ${AXCL_ROOT_DIR}/toolkit/axcl_fifo.c
        ${AXCL_ROOT_DIR}/toolkit/threadx.cpp
        ${AXCL_ROOT_DIR}/toolkit/event.cpp
        ${AXCL_ROOT_DIR}/toolkit/elapser.cpp
    INCLUDE_DIRECTORIES
        $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/sample>
    LINK_LIBRARIES
        axcl::runtime
        axcl::native::sys
        axcl::native::vdec
        3rdparty::ffmpeg
        3rdparty::cmdline
)
