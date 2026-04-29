# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# set library name
set (AXCL_LOGGER_NAME ax_logger)

# set library path
get_filename_component (AXCL_LOGGER_PATH "${AXCL_ROOT_DIR}/logger" ABSOLUTE)

# collect all source files
aux_source_directory (${AXCL_LOGGER_PATH} AXCL_LOGGER_SRCS)

# add cmake help function
include (AddProjectLibrary)

# add interface target
add_project_library (${AXCL_LOGGER_NAME} SHARED
    SOURCES
        ${AXCL_LOGGER_SRCS}
    PRIVATE
        COMPILE_DEFINITIONS
            $<$<BOOL:${SYSTEM_IS_WINDOWS}>:AXCL_EXPORTS>
        INCLUDE_DIRECTORIES
            $<BUILD_INTERFACE:${AXCL_LOGGER_PATH}>
            $<BUILD_INTERFACE:${AXCL_EXTERNAL_HEADER_PATH}>
            $<BUILD_INTERFACE:${AXCL_INTERNAL_HEADER_PATH}>
            $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/toolkit>
        LINK_LIBRARIES
            3rdparty::spdlog
    ALIAS
        axcl::logger
)
