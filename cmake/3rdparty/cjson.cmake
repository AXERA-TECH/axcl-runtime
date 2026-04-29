# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# set cjson library name
set (CJSON_NAME cJSON)

# set cjson library linking path
get_filename_component (CJSON_PATH "${AXCL_ROOT_DIR}/3rdparty/${CJSON_NAME}" ABSOLUTE)

# add cmake help function
include (AddProjectLibrary)

# add cjson interface target
add_project_library (${CJSON_NAME} STATIC
    SOURCES
        ${CJSON_PATH}/cJSON.c
        ${CJSON_PATH}/cJSON_Utils.c
    PUBLIC INCLUDE_DIRECTORIES
        $<BUILD_INTERFACE:${CJSON_PATH}>
    PRIVATE
        COMPILE_OPTIONS
            $<$<C_COMPILER_ID:MSVC>:/sdl>
    ALIAS
        3rdparty::cjson
)
