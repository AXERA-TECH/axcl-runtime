# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# set venc test path
get_filename_component (AXCL_VENC_TEST_PATH "${AXCL_ROOT_DIR}/test/venc" ABSOLUTE)
get_filename_component (AXCL_VENC_COMMON_TEST_PATH "${AXCL_ROOT_DIR}/test/venc/common" ABSOLUTE)
get_filename_component (AXCL_VENC_FEATURE_TEST_PATH "${AXCL_ROOT_DIR}/test/venc/featureTest" ABSOLUTE)
get_filename_component (AXCL_SYS_SAMPLE_PATH "${AXCL_ROOT_DIR}/sample/sys" ABSOLUTE)

# collect all source files
aux_source_directory (${AXCL_VENC_TEST_PATH} AXCL_VENC_TEST_SRCS)
aux_source_directory (${AXCL_VENC_COMMON_TEST_PATH} AXCL_VENC_COMMON_TEST_SRCS)
aux_source_directory (${AXCL_VENC_FEATURE_TEST_PATH} AXCL_VENC_FEATURE_TEST_SRCS)
aux_source_directory (${AXCL_SYS_SAMPLE_PATH} AXCL_SYS_SAMPLE_SRCS)

# add venc test
add_test_app (venc
    NAME
        axcl_ut_venc
    SOURCES
        ${AXCL_VENC_TEST_SRCS}
        ${AXCL_VENC_COMMON_TEST_SRCS}
        ${AXCL_VENC_FEATURE_TEST_SRCS}
        ${AXCL_SYS_SAMPLE_SRCS}
        ${AXCL_ROOT_DIR}/toolkit/threadx.cpp
        ${AXCL_ROOT_DIR}/toolkit/event.cpp
        ${AXCL_ROOT_DIR}/toolkit/elapser.cpp
    LINK_LIBRARIES
        axcl::native::venc
        axcl::native::sys
        axcl::runtime
)
