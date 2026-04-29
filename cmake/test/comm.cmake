# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# add PCIE communication test
add_test_app (comm/pcie/rc
    NAME
        axcl_ut_pcie_rc
    SOURCES
        ${AXCL_ROOT_DIR}/test/comm/pcie/rc/rc.cpp
        ${AXCL_ROOT_DIR}/toolkit/elapser.cpp
    LINK_LIBRARIES
        axcl::host::comm
        axcl::runtime
)

# add PCIE endpoint test
if (NOT SYSTEM_IS_WINDOWS)
add_test_app (comm/pcie/ep
    NAME
        axcl_ut_pcie_ep
    SOURCES
        ${AXCL_ROOT_DIR}/test/comm/pcie/ep/ep.cpp
    LINK_LIBRARIES
        axcl::host::comm
        axcl::runtime
)
endif ()