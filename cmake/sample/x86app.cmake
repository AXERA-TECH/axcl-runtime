# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#


# set x86app sample path
get_filename_component (AXCL_X86APP_SAMPLE_PATH "${AXCL_ROOT_DIR}/sample/x86app" ABSOLUTE)

# collect all source files
aux_source_directory (${AXCL_X86APP_SAMPLE_PATH} AXCL_X86APP_SAMPLE_SRCS)

# add x86app sample
add_sample_app (x86app
    NAME
        axcl_demo
    SOURCES
        ${AXCL_X86APP_SAMPLE_SRCS}
    LINK_LIBRARIES
        axcl::runtime
        axcl::host::comm
        3rdparty::spdlog
)
