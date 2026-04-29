# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# module version configuration function
function (ConfigProjectVersion _target)
    # get target name from argument
    if (NOT _target)
        message (FATAL_ERROR "ConfigProjectVersion: Target name is not specified.")
    endif()

    set (CONFIG_PROTO_FILE "${CMAKE_SOURCE_DIR}/config/ax_module_version.h.in")
    set (VERSION_PATH "${CMAKE_CURRENT_BINARY_DIR}/generated")

    # prepare generated dependencies parameters
    set (MODULE_NAME "${_target}")
    set (VERSION_FILE "${VERSION_PATH}/ax_module_version.h")

    # generate version file
    configure_file ("${CONFIG_PROTO_FILE}" "${VERSION_FILE}" @ONLY)
    message (STATUS "Configured version file: ${VERSION_FILE}")

    # add generated version file to target
    target_include_directories (${_target} PRIVATE "${VERSION_PATH}")
endfunction()
