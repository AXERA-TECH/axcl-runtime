# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# set device working path
get_filename_component (AXCL_DEVICE_PATH "${AXCL_ROOT_DIR}/device" ABSOLUTE)

# add logger library
include (logger.cmake)

# add protocol library
include (protocol.cmake)

# add protocol library
include (package.cmake)

# add comm library
include (comm.cmake)

# add daemon
include (daemon.cmake)

# add worker
include (worker.cmake)
