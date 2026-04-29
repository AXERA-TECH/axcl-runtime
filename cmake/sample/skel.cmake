# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#


# set skel sample path
get_filename_component (AXCL_SKEL_SAMPLE_PATH "${AXCL_ROOT_DIR}/sample/skel" ABSOLUTE)

# collect all source files
aux_source_directory (${AXCL_SKEL_SAMPLE_PATH} AXCL_SKEL_SAMPLE_SRCS)

# add skel sample
add_sample_app (skel
    NAME
        axcl_sample_skel
    SOURCES
        ${AXCL_SKEL_SAMPLE_SRCS}
    COMPILE_DEFINITIONS
        $<BUILD_INTERFACE:AXCL_SAMPLE_NAME="axcl_sample_skel">
    LINK_LIBRARIES
        axcl::runtime
        axcl::native::sys
        axcl::native::ivps
        axcl::native::npu
        axcl::native::skel
)
