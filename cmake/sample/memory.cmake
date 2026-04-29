# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axcl Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axcl Semiconductor Co., Ltd.
#


# set memory sample path
get_filename_component (AXCL_MEMORY_SAMPLE_PATH "${AXCL_ROOT_DIR}/sample/memory" ABSOLUTE)

# collect all source files
aux_source_directory (${AXCL_MEMORY_SAMPLE_PATH} AXCL_MEMORY_SAMPLE_SRCS)

# add memory sample
add_sample_app (memory
    NAME
        axcl_sample_memory
    SOURCES
        ${AXCL_MEMORY_SAMPLE_SRCS}
    LINK_LIBRARIES
        axcl::runtime
)
