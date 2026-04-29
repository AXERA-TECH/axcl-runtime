# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# set package test path
get_filename_component (AXCL_PKG_TEST_PATH "${AXCL_ROOT_DIR}/test/pkg" ABSOLUTE)
get_filename_component (AXCL_PKG_NATIVE_TEST_PATH "${AXCL_ROOT_DIR}/test/pkg/native" ABSOLUTE)
get_filename_component (AXCL_PKG_RUNTIME_TEST_PATH "${AXCL_ROOT_DIR}/test/pkg/runtime" ABSOLUTE)

# collect all source files
aux_source_directory (${AXCL_PKG_TEST_PATH} AXCL_PKG_TEST_SRCS)
aux_source_directory (${AXCL_PKG_NATIVE_TEST_PATH} AXCL_PKG_NATIVE_TEST_SRCS)
aux_source_directory (${AXCL_PKG_RUNTIME_TEST_PATH} AXCL_PKG_RUNTIME_TEST_SRCS)

# add package test
add_test_app (pkg
    NAME
        axcl_ut_package
    SOURCES
        ${AXCL_PKG_TEST_SRCS}
        ${AXCL_PKG_NATIVE_TEST_SRCS}
        ${AXCL_PKG_RUNTIME_TEST_SRCS}
        ${AXCL_ROOT_DIR}/toolkit/elapser.cpp
    LINK_LIBRARIES
        axcl::host::package
        3rdparty::protobuf
        3rdparty::gtest
)
