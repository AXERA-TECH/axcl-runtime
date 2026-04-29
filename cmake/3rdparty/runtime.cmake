# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# set runtime library path
get_filename_component (RUNTIME_PATH "${AXCL_ROOT_DIR}/3rdparty/runtime" ABSOLUTE)

# install runtime dll files on Windows
if (WIN32)
    file(GLOB RUNTIME_INSTALL_FILES
        ${RUNTIME_PATH}/*.dll
    )

    install(
        FILES
            ${RUNTIME_INSTALL_FILES}
        DESTINATION bin
    )
endif()

