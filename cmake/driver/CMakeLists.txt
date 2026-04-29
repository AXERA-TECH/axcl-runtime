# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# set driver path
get_filename_component (AXCL_DEVICE_PATH "${AXCL_ROOT_DIR}/drv" ABSOLUTE)

# add Windows PCIe driver libraries for host build
if (WIN32)
    include (pcie.cmake)
endif()