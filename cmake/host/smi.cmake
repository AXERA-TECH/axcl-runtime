# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# set smi name
set (AXCL_SMI_NAME axcl-smi)

# get smi home path
get_filename_component (AXCL_SMI_HOME_PATH "${AXCL_HOST_PATH}/smi"       ABSOLUTE)
get_filename_component (AXCL_SMI_CMD_PATH  "${AXCL_SMI_HOME_PATH}/cmd"   ABSOLUTE)
get_filename_component (AXCL_SMI_UTIL_PATH "${AXCL_SMI_HOME_PATH}/utils" ABSOLUTE)

# collect all source files
aux_source_directory (${AXCL_SMI_HOME_PATH}  AXCL_SMI_CPPS)
aux_source_directory (${AXCL_SMI_CMD_PATH}   AXCL_SMI_CPPS)
aux_source_directory (${AXCL_SMI_UTIL_PATH}  AXCL_SMI_CPPS)

# add cmake help function
include (AddProjectExecutable)

# add target
add_project_executable (${AXCL_SMI_NAME}
    SOURCES
        ${AXCL_SMI_CPPS}
    INCLUDE_DIRECTORIES
        ${AXCL_SMI_HOME_PATH}
        ${AXCL_SMI_CMD_PATH}
        ${AXCL_SMI_UTIL_PATH}
        $<BUILD_INTERFACE:${AXCL_INSTALL_INCLUDE_PATH}>
        $<BUILD_INTERFACE:${AXCL_INTERNAL_HEADER_PATH}>
        $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/toolkit>
    COMPILE_DEFINITIONS
        $<BUILD_INTERFACE:AXCL_SDK_VERSION="${BUILD_VERSION}">
        $<BUILD_INTERFACE:AXCL_SMI_VERSION="${BUILD_VERSION}">
    LINK_LIBRARIES
        3rdparty::argsxx
        axcl::runtime
        axcl::native::npu
)