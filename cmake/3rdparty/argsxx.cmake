# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# set module name
set (ARGSXX argsxx)

# set module path
get_filename_component (ARGSXX_PATH "${AXCL_ROOT_DIR}/3rdparty/argsxx" ABSOLUTE)

# add cmake help function
include (AddProjectLibrary)

# add module interface target
add_project_library (${ARGSXX} INTERFACE
    PUBLIC INCLUDE_DIRECTORIES
        $<BUILD_INTERFACE:${ARGSXX_PATH}>
    ALIAS
        3rdparty::argsxx
)
