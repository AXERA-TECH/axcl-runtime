# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#


# set usrworker sample path
get_filename_component (AXCL_USRWORKER_SAMPLE_PATH "${AXCL_ROOT_DIR}/sample/usrworker/host" ABSOLUTE)

# collect all source files
aux_source_directory (${AXCL_USRWORKER_SAMPLE_PATH} AXCL_USRWORKER_SAMPLE_SRCS)

# add usrworker sample
add_sample_app (usrworker/host
    NAME
        axcl_run_usrworker
    SOURCES
        ${AXCL_USRWORKER_SAMPLE_SRCS}
    LINK_LIBRARIES
        axcl::runtime
        axcl::native::sys
)
