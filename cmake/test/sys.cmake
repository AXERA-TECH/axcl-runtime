# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# add system common memory management test
add_test_app (sys/cmm
    NAME
        axcl_ut_cmm
    SOURCES
        ${AXCL_ROOT_DIR}/test/sys/cmm/axcl_ut_cmm.cpp
    LINK_LIBRARIES
        axcl::native::sys
        axcl::native::dmadim
        axcl::runtime
)

# add system other test
add_test_app (sys/other
    NAME
        axcl_ut_msys
    SOURCES
        ${AXCL_ROOT_DIR}/test/sys/other/axcl_ut_msys.cpp
    LINK_LIBRARIES
        axcl::native::sys
        axcl::native::dmadim
        axcl::runtime
)

# add system pool test
add_test_app (sys/pool
    NAME
        axcl_ut_pool
    SOURCES
        ${AXCL_ROOT_DIR}/test/sys/pool/axcl_ut_pool.cpp
    LINK_LIBRARIES
        axcl::native::sys
        axcl::native::dmadim
        axcl::runtime
)
