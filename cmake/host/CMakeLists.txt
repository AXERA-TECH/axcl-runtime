# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# set host working path
get_filename_component (AXCL_HOST_PATH "${AXCL_ROOT_DIR}/host" ABSOLUTE)

# add protocol library
include (protocol.cmake)

# add comm library
include (comm.cmake)

# add pkg library
include (package.cmake)

# include host library token
include (token.cmake)

# add runtime library
include (runtime.cmake)

# add native library
include (native.cmake)

# add skel library
# include (skel.cmake)

# install headers
install(DIRECTORY ${AXCL_EXTERNAL_HEADER_PATH}/
    DESTINATION include
    FILES_MATCHING
        PATTERN "*.h"
)

# add smi app
include (smi.cmake)