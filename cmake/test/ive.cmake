# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# set ive test path
get_filename_component (AXCL_IVE_TEST_PATH "${AXCL_ROOT_DIR}/test/ive" ABSOLUTE)
get_filename_component (AXCL_IVE_UT_CASE_PATH "${AXCL_ROOT_DIR}/test/ive/ut_case" ABSOLUTE)

# collect all source files
aux_source_directory (${AXCL_IVE_TEST_PATH} AXCL_IVE_TEST_SRCS)
aux_source_directory (${AXCL_IVE_UT_CASE_PATH} AXCL_IVE_UT_CASE_SRCS)

# add ive test
add_test_app (ive
    NAME
        axcl_ut_ive
    SOURCES
        ${AXCL_IVE_TEST_SRCS}
        ${AXCL_IVE_UT_CASE_SRCS}
    LINK_LIBRARIES
        axcl::native::ive
        axcl::native::sys
        axcl::native::dmadim
        axcl::native::npu
        axcl::runtime
)
