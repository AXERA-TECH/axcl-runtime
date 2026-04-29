# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#


# set subid sample path
get_filename_component (AXCL_SUBID_SAMPLE_PATH "${AXCL_ROOT_DIR}/sample/subid" ABSOLUTE)

# collect all source files
aux_source_directory (${AXCL_SUBID_SAMPLE_PATH} AXCL_SUBID_SAMPLE_SRCS)

# add subid sample
add_sample_app (subid
    NAME
        axcl_change_subid
    SOURCES
        ${AXCL_SUBID_SAMPLE_SRCS}
    LINK_LIBRARIES
        axcl::runtime
)