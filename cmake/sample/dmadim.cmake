# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#


# set dmadim sample path
get_filename_component (AXCL_DMADIM_SAMPLE_PATH "${AXCL_ROOT_DIR}/sample/dmadim" ABSOLUTE)

# collect all source files
aux_source_directory (${AXCL_DMADIM_SAMPLE_PATH} AXCL_DMADIM_SAMPLE_SRCS)

# add dmadim sample
add_sample_app (dmadim
    NAME
        axcl_sample_dmadim
    SOURCES
        ${AXCL_DMADIM_SAMPLE_SRCS}
    LINK_LIBRARIES
        axcl::runtime
        axcl::native::sys
        axcl::native::dmadim
)
