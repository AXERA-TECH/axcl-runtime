# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#


# set axclite library name
set (AXCL_AXCLITE_NAME axcl_lite)

# set axclite sample paths
get_filename_component (AXCL_AXCLITE_HOME_PATH "${AXCL_ROOT_DIR}/sample/axclite" ABSOLUTE)
get_filename_component (AXCL_AXCLITE_MSYS_PATH "${AXCL_ROOT_DIR}/sample/axclite/msys" ABSOLUTE)
get_filename_component (AXCL_AXCLITE_VDEC_PATH "${AXCL_ROOT_DIR}/sample/axclite/vdec" ABSOLUTE)
get_filename_component (AXCL_AXCLITE_IVPS_PATH "${AXCL_ROOT_DIR}/sample/axclite/ivps" ABSOLUTE)
get_filename_component (AXCL_AXCLITE_VENC_PATH "${AXCL_ROOT_DIR}/sample/axclite/venc" ABSOLUTE)
get_filename_component (AXCL_TOOLKIT_LOG_PATH "${AXCL_ROOT_DIR}/toolkit/log" ABSOLUTE)

# collect all source files
aux_source_directory (${AXCL_AXCLITE_MSYS_PATH} AXCL_AXCLITE_MSYS_SRCS)
aux_source_directory (${AXCL_AXCLITE_VDEC_PATH} AXCL_AXCLITE_VDEC_SRCS)
aux_source_directory (${AXCL_AXCLITE_IVPS_PATH} AXCL_AXCLITE_IVPS_SRCS)
aux_source_directory (${AXCL_AXCLITE_VENC_PATH} AXCL_AXCLITE_VENC_SRCS)
aux_source_directory (${AXCL_TOOLKIT_LOG_PATH} AXCL_AXCLITE_LOG_SRCS)

# add cmake help function
include (AddProjectLibrary)

# add axclite library target
add_project_library (${AXCL_AXCLITE_NAME} SHARED
    SOURCES
        ${AXCL_ROOT_DIR}/sample/axclite/version.cpp
        ${AXCL_AXCLITE_MSYS_SRCS}
        ${AXCL_AXCLITE_VDEC_SRCS}
        ${AXCL_AXCLITE_IVPS_SRCS}
        ${AXCL_AXCLITE_VENC_SRCS}
        ${AXCL_AXCLITE_LOG_SRCS}
        ${AXCL_ROOT_DIR}/toolkit/threadx.cpp
        ${AXCL_ROOT_DIR}/toolkit/event.cpp
    PRIVATE
        COMPILE_DEFINITIONS
            $<$<BOOL:${SYSTEM_IS_WINDOWS}>:AXCL_EXPORTS>
        INCLUDE_DIRECTORIES
            $<BUILD_INTERFACE:${AXCL_INSTALL_INCLUDE_PATH}>
            $<BUILD_INTERFACE:${AXCL_AXCLITE_HOME_PATH}/include>
            $<BUILD_INTERFACE:${AXCL_AXCLITE_MSYS_PATH}>
            $<BUILD_INTERFACE:${AXCL_AXCLITE_VDEC_PATH}>
            $<BUILD_INTERFACE:${AXCL_AXCLITE_IVPS_PATH}>
            $<BUILD_INTERFACE:${AXCL_AXCLITE_VENC_PATH}>
            $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/toolkit>
            $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/logger>
        LINK_LIBRARIES
            axcl::logger
            axcl::runtime
            axcl::native::sys
            axcl::native::ivps
            axcl::native::vdec
            axcl::native::venc
            3rdparty::spdlog
    ALIAS
        axcl::axclite
)
