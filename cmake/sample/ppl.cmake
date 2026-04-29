# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#


set (AXCL_AXCLPPL_NAME axcl_ppl)

# set ppl sample paths
get_filename_component (AXCL_PPL_SAMPLE_PATH "${AXCL_ROOT_DIR}/sample/ppl" ABSOLUTE)
get_filename_component (AXCL_PPL_PPL_PATH "${AXCL_ROOT_DIR}/sample/ppl/ppl" ABSOLUTE)
get_filename_component (AXCL_PPL_TRANSCODE_PATH "${AXCL_ROOT_DIR}/sample/ppl/ppl/transcode" ABSOLUTE)
get_filename_component (AXCL_PPL_HVCP_PATH "${AXCL_ROOT_DIR}/sample/ppl/ppl/hvcp" ABSOLUTE)
get_filename_component (AXCL_TOOLKIT_LOG_PATH "${AXCL_ROOT_DIR}/toolkit/log" ABSOLUTE)
get_filename_component (AXCL_TOOLKIT_PATH "${AXCL_ROOT_DIR}/toolkit" ABSOLUTE)
get_filename_component (AXCL_DEMUX_PATH "${AXCL_ROOT_DIR}/sample/demux" ABSOLUTE)

# collect all source files for ppl library
aux_source_directory (${AXCL_PPL_PPL_PATH} AXCL_PPL_PPL_SRCS)
aux_source_directory (${AXCL_PPL_TRANSCODE_PATH} AXCL_PPL_TRANSCODE_SRCS)
aux_source_directory (${AXCL_PPL_HVCP_PATH} AXCL_PPL_HVCP_SRCS)
aux_source_directory (${AXCL_TOOLKIT_LOG_PATH} AXCL_PPL_LOG_SRCS)

# add cmake help function
include (AddProjectLibrary)

# add ppl sample - main ppl library
add_project_library (${AXCL_AXCLPPL_NAME} SHARED
    SOURCES
        ${AXCL_PPL_PPL_SRCS}
        ${AXCL_PPL_TRANSCODE_SRCS}
        ${AXCL_PPL_HVCP_SRCS}
        ${AXCL_PPL_LOG_SRCS}
        ${AXCL_ROOT_DIR}/toolkit/threadx.cpp
        ${AXCL_ROOT_DIR}/toolkit/event.cpp
    PRIVATE
        COMPILE_DEFINITIONS
            $<$<BOOL:${SYSTEM_IS_WINDOWS}>:AXCL_EXPORTS>
        INCLUDE_DIRECTORIES
            $<BUILD_INTERFACE:${AXCL_INSTALL_INCLUDE_PATH}>
            $<BUILD_INTERFACE:${AXCL_SAMPLE_PATH}/axclite/include>
            $<BUILD_INTERFACE:${AXCL_SAMPLE_PATH}/axclite/msys>
            $<BUILD_INTERFACE:${AXCL_SAMPLE_PATH}/axclite/ivps>
            $<BUILD_INTERFACE:${AXCL_SAMPLE_PATH}/axclite/vdec>
            $<BUILD_INTERFACE:${AXCL_SAMPLE_PATH}/axclite/venc>
            $<BUILD_INTERFACE:${AXCL_SAMPLE_PATH}/ppl/include>
            $<BUILD_INTERFACE:${AXCL_SAMPLE_PATH}/ppl/ppl>
            $<BUILD_INTERFACE:${AXCL_SAMPLE_PATH}/ppl/ppl/hvcp>
            $<BUILD_INTERFACE:${AXCL_SAMPLE_PATH}/ppl/ppl/transcode>
            $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/toolkit>
            $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/logger>
        LINK_LIBRARIES
            axcl::logger
            axcl::runtime
            axcl::native::sys
            axcl::axclite
            3rdparty::spdlog
    ALIAS
        axcl::ppl
)

# add ppl transcode demo
add_sample_app (ppl/demo/transcode
    NAME
        axcl_sample_transcode
    SOURCES
        ${AXCL_SAMPLE_PATH}/ppl/demo/transcode/main.cpp
        ${AXCL_ROOT_DIR}/toolkit/axcl_fifo.c
        ${AXCL_ROOT_DIR}/toolkit/threadx.cpp
        ${AXCL_ROOT_DIR}/toolkit/event.cpp
        ${AXCL_ROOT_DIR}/toolkit/elapser.cpp
        ${AXCL_DEMUX_PATH}/ffmpeg.cpp
    INCLUDE_DIRECTORIES
        $<BUILD_INTERFACE:${AXCL_INSTALL_INCLUDE_PATH}>
        $<BUILD_INTERFACE:${AXCL_SAMPLE_PATH}/ppl/include>
        $<BUILD_INTERFACE:${AXCL_SAMPLE_PATH}/axclite/include>
        $<BUILD_INTERFACE:${AXCL_SAMPLE_PATH}/axclite/msys>
        $<BUILD_INTERFACE:${AXCL_SAMPLE_PATH}/axclite/ivps>
        $<BUILD_INTERFACE:${AXCL_SAMPLE_PATH}/axclite/vdec>
        $<BUILD_INTERFACE:${AXCL_SAMPLE_PATH}/axclite/venc>
        $<BUILD_INTERFACE:${AXCL_SAMPLE_PATH}>
        $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/toolkit>
        $<BUILD_INTERFACE:${AXCL_DEMUX_PATH}>
    LINK_LIBRARIES
        axcl::runtime
        axcl::native::sys
        axcl::ppl
        3rdparty::ffmpeg
        3rdparty::cmdline
)

# add ppl hvcp demo
add_sample_app (ppl/demo/hvcp
    NAME
        axcl_sample_hvcp
    INCLUDE_DIRECTORIES
        $<BUILD_INTERFACE:${AXCL_INSTALL_INCLUDE_PATH}>
        $<BUILD_INTERFACE:${AXCL_SAMPLE_PATH}/ppl/include>
        $<BUILD_INTERFACE:${AXCL_SAMPLE_PATH}/axclite/include>
        $<BUILD_INTERFACE:${AXCL_SAMPLE_PATH}/axclite/msys>
        $<BUILD_INTERFACE:${AXCL_SAMPLE_PATH}/axclite/ivps>
        $<BUILD_INTERFACE:${AXCL_SAMPLE_PATH}/axclite/vdec>
        $<BUILD_INTERFACE:${AXCL_SAMPLE_PATH}/axclite/venc>
        $<BUILD_INTERFACE:${AXCL_SAMPLE_PATH}>
        $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/toolkit>
        $<BUILD_INTERFACE:${AXCL_DEMUX_PATH}>
    SOURCES
        ${AXCL_SAMPLE_PATH}/ppl/demo/hvcp/main.cpp
        ${AXCL_ROOT_DIR}/toolkit/axcl_fifo.c
        ${AXCL_ROOT_DIR}/toolkit/threadx.cpp
        ${AXCL_ROOT_DIR}/toolkit/event.cpp
        ${AXCL_ROOT_DIR}/toolkit/elapser.cpp
        ${AXCL_DEMUX_PATH}/ffmpeg.cpp
    LINK_LIBRARIES
        axcl::runtime
        axcl::native::sys
        axcl::ppl
        3rdparty::ffmpeg
        3rdparty::cmdline
)
