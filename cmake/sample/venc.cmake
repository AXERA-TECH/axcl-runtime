# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#


# set venc sample path
get_filename_component (AXCL_VENC_SAMPLE_PATH "${AXCL_ROOT_DIR}/sample/venc" ABSOLUTE)
get_filename_component (AXCL_VENC_COMMON_SAMPLE_PATH "${AXCL_ROOT_DIR}/sample/venc/common" ABSOLUTE)
get_filename_component (AXCL_SYS_SAMPLE_PATH "${AXCL_ROOT_DIR}/sample/sys" ABSOLUTE)

# collect all source files
aux_source_directory (${AXCL_VENC_SAMPLE_PATH} AXCL_VENC_SAMPLE_SRCS)
aux_source_directory (${AXCL_VENC_COMMON_SAMPLE_PATH} AXCL_VENC_COMMON_SAMPLE_SRCS)
aux_source_directory (${AXCL_SYS_SAMPLE_PATH} AXCL_SYS_SAMPLE_SRCS)

# add venc sample (Linux only)
if (NOT SYSTEM_IS_WINDOWS)
    add_sample_app (venc
        NAME
            axcl_sample_venc
        SOURCES
            ${AXCL_VENC_SAMPLE_SRCS}
            ${AXCL_VENC_COMMON_SAMPLE_SRCS}
            ${AXCL_SYS_SAMPLE_SRCS}
            ${AXCL_ROOT_DIR}/toolkit/threadx.cpp
            ${AXCL_ROOT_DIR}/toolkit/event.cpp
        LINK_LIBRARIES
            axcl::runtime
            axcl::native::sys
            axcl::native::venc
    )
endif ()
