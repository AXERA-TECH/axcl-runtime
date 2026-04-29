# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#


# set ive sample path
get_filename_component (AXCL_IVE_SAMPLE_PATH "${AXCL_ROOT_DIR}/sample/ive" ABSOLUTE)
get_filename_component (AXCL_IVE_CV_SAMPLE_PATH "${AXCL_ROOT_DIR}/sample/ive/ive_cv" ABSOLUTE)

# collect all source files
aux_source_directory (${AXCL_IVE_SAMPLE_PATH} AXCL_IVE_SAMPLE_SRCS)
aux_source_directory (${AXCL_IVE_CV_SAMPLE_PATH} AXCL_IVE_CV_SAMPLE_SRCS)

# add ive sample
add_sample_app (ive
    NAME
        axcl_sample_ive
    SOURCES
        ${AXCL_IVE_SAMPLE_SRCS}
        ${AXCL_IVE_CV_SAMPLE_SRCS}
    LINK_LIBRARIES
        axcl::runtime
        axcl::native::sys
        axcl::native::ive
        axcl::native::dmadim
        axcl::native::npu
)
