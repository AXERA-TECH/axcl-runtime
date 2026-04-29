# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#


# set aicard sample path
get_filename_component (AXCL_AICARD_SAMPLE_PATH "${AXCL_ROOT_DIR}/sample/aicard" ABSOLUTE)
get_filename_component (AXCL_AICARD_CAPTURE_PATH "${AXCL_ROOT_DIR}/sample/aicard/component/capture" ABSOLUTE)
get_filename_component (AXCL_AICARD_CONFIG_INI_PATH "${AXCL_ROOT_DIR}/sample/aicard/component/config/ini" ABSOLUTE)
get_filename_component (AXCL_AICARD_DETECT_PATH "${AXCL_ROOT_DIR}/sample/aicard/component/detect" ABSOLUTE)
get_filename_component (AXCL_AICARD_DISPATCH_PATH "${AXCL_ROOT_DIR}/sample/aicard/component/dispatch" ABSOLUTE)
get_filename_component (AXCL_AICARD_IVPS_PATH "${AXCL_ROOT_DIR}/sample/aicard/component/ivps" ABSOLUTE)
get_filename_component (AXCL_AICARD_LOG_PATH "${AXCL_ROOT_DIR}/sample/aicard/component/log" ABSOLUTE)
get_filename_component (AXCL_AICARD_POOL_PATH "${AXCL_ROOT_DIR}/sample/aicard/component/pool" ABSOLUTE)
get_filename_component (AXCL_AICARD_TRANSFER_PATH "${AXCL_ROOT_DIR}/sample/aicard/component/transfer" ABSOLUTE)
get_filename_component (AXCL_AICARD_UTILS_PATH "${AXCL_ROOT_DIR}/sample/aicard/component/utils" ABSOLUTE)
get_filename_component (AXCL_AICARD_VDEC_PATH "${AXCL_ROOT_DIR}/sample/aicard/component/vdec" ABSOLUTE)
get_filename_component (AXCL_AICARD_VDEC_STREAM_PATH "${AXCL_ROOT_DIR}/sample/aicard/component/vdec/stream" ABSOLUTE)
get_filename_component (AXCL_AICARD_VO_PATH "${AXCL_ROOT_DIR}/sample/aicard/component/vo" ABSOLUTE)
get_filename_component (AXCL_AICARD_YUV_PATH "${AXCL_ROOT_DIR}/sample/aicard/component/yuv" ABSOLUTE)

# collect all source files
aux_source_directory (${AXCL_AICARD_SAMPLE_PATH} AXCL_AICARD_SAMPLE_SRCS)
aux_source_directory (${AXCL_AICARD_CAPTURE_PATH} AXCL_AICARD_CAPTURE_SRCS)
aux_source_directory (${AXCL_AICARD_CONFIG_INI_PATH} AXCL_AICARD_CONFIG_INI_SRCS)
aux_source_directory (${AXCL_AICARD_DETECT_PATH} AXCL_AICARD_DETECT_SRCS)
aux_source_directory (${AXCL_AICARD_DISPATCH_PATH} AXCL_AICARD_DISPATCH_SRCS)
aux_source_directory (${AXCL_AICARD_IVPS_PATH} AXCL_AICARD_IVPS_SRCS)
aux_source_directory (${AXCL_AICARD_LOG_PATH} AXCL_AICARD_LOG_SRCS)
aux_source_directory (${AXCL_AICARD_POOL_PATH} AXCL_AICARD_POOL_SRCS)
aux_source_directory (${AXCL_AICARD_TRANSFER_PATH} AXCL_AICARD_TRANSFER_SRCS)
aux_source_directory (${AXCL_AICARD_UTILS_PATH} AXCL_AICARD_UTILS_SRCS)
aux_source_directory (${AXCL_AICARD_VDEC_PATH} AXCL_AICARD_VDEC_SRCS)
aux_source_directory (${AXCL_AICARD_VDEC_STREAM_PATH} AXCL_AICARD_VDEC_STREAM_SRCS)
aux_source_directory (${AXCL_AICARD_VO_PATH} AXCL_AICARD_VO_SRCS)
aux_source_directory (${AXCL_AICARD_YUV_PATH} AXCL_AICARD_YUV_SRCS)

# add aicard sample (Linux only)
if (NOT SYSTEM_IS_WINDOWS)
    add_sample_app (aicard
        NAME
            axcl_demo
        SOURCES
            ${AXCL_AICARD_SAMPLE_SRCS}
            ${AXCL_AICARD_CAPTURE_SRCS}
            ${AXCL_AICARD_CONFIG_INI_SRCS}
            ${AXCL_AICARD_DETECT_SRCS}
            ${AXCL_AICARD_DISPATCH_SRCS}
            ${AXCL_AICARD_IVPS_SRCS}
            ${AXCL_AICARD_LOG_SRCS}
            ${AXCL_AICARD_POOL_SRCS}
            ${AXCL_AICARD_TRANSFER_SRCS}
            ${AXCL_AICARD_UTILS_SRCS}
            ${AXCL_AICARD_VDEC_SRCS}
            ${AXCL_AICARD_VDEC_STREAM_SRCS}
            ${AXCL_AICARD_VO_SRCS}
            ${AXCL_AICARD_YUV_SRCS}
            ${AXCL_ROOT_DIR}/toolkit/elapser.cpp
            ${AXCL_ROOT_DIR}/toolkit/axcl_fifo.c
        COMPILE_DEFINITIONS
            __RTSP_UNSUPPORT__
        LINK_LIBRARIES
            axcl::runtime
            axcl::native::sys
            axcl::native::pkg
            axcl::native::vdec
            axcl::native::venc
            axcl::native::ivps
            axcl::native::npu
            axcl::native::skel
            3rdparty::ffmpeg
    )
endif ()
