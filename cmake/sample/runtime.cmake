# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#


# set runtime sample path
get_filename_component (AXCL_RUNTIME_SAMPLE_PATH "${AXCL_ROOT_DIR}/sample/runtime" ABSOLUTE)

# collect all source files
aux_source_directory (${AXCL_RUNTIME_SAMPLE_PATH} AXCL_RUNTIME_SAMPLE_SRCS)

# add runtime sample
add_sample_app (runtime
    NAME
        axcl_sample_runtime
    SOURCES
        ${AXCL_RUNTIME_SAMPLE_SRCS}
    LINK_LIBRARIES
        axcl::runtime
        axcl::host::comm
        3rdparty::spdlog
)
