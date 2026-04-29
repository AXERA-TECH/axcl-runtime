# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# set cmdline library name
set (CMDLINE_NAME cmdline)

# set cmdline library path
get_filename_component (CMDLINE_PATH "${AXCL_ROOT_DIR}/3rdparty/cmdline" ABSOLUTE)

# add cmake help function
include (AddProjectLibrary)

# add cmdline interface target
add_project_library (${CMDLINE_NAME} INTERFACE
    PUBLIC INCLUDE_DIRECTORIES
        $<BUILD_INTERFACE:${CMDLINE_PATH}/include>
    ALIAS
        3rdparty::cmdline
)
