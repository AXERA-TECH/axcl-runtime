# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# set npu test path
get_filename_component (AXCL_NPU_TEST_PATH "${AXCL_ROOT_DIR}/test/npu" ABSOLUTE)

# collect all source files
aux_source_directory (${AXCL_NPU_TEST_PATH} AXCL_NPU_TEST_SRCS)

# add npu test
add_test_app (npu
    NAME
        axcl_ut_npu
    SOURCES
        ${AXCL_NPU_TEST_SRCS}
    LINK_LIBRARIES
        axcl::native::npu
        axcl::runtime
        3rdparty::gtest
)
