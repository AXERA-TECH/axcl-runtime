# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# set bsp library name
set (AXCL_RUNTIME_NAME axcl_rt)

# set bsp library path
get_filename_component (AXCL_NATIVE_PATH "${AXCL_HOST_PATH}/native" ABSOLUTE)

# add cmake help function
include (AddProjectLibrary)

function (add_native_library name)
    aux_source_directory (${AXCL_NATIVE_PATH}/${name} AXCL_NATIVE_SRCS)
    aux_source_directory (${AXCL_ROOT_DIR}/toolkit/log AXCL_NATIVE_LOG_SRCS)

    add_project_library (axcl_${name} SHARED
        SOURCES
            ${AXCL_NATIVE_SRCS}
            ${AXCL_NATIVE_LOG_SRCS}

        PRIVATE
            COMPILE_DEFINITIONS
                $<$<BOOL:${SYSTEM_IS_WINDOWS}>:AXCL_EXPORTS>
            INCLUDE_DIRECTORIES
                $<BUILD_INTERFACE:${AXCL_NATIVE_PATH}/template>
                $<BUILD_INTERFACE:${AXCL_EXTERNAL_HEADER_PATH}>
                $<BUILD_INTERFACE:${AXCL_INTERNAL_HEADER_PATH}>
                $<BUILD_INTERFACE:${HOME_EXTERNAL_HEADER_PATH}>
                $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/toolkit>
                $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/logger>
            LINK_LIBRARIES
                axcl::logger
                axcl::runtime
                axcl::host::package
                axcl::host::token
                3rdparty::bsp::header
                3rdparty::spdlog
        ALIAS
            axcl::native::${name}
    )

endfunction()

# add native libraries
add_native_library (dmadim)
add_native_library (dsp)
add_native_library (ive)
add_native_library (ivps)
add_native_library (npu)
add_native_library (sys)
add_native_library (vdec)
add_native_library (venc)
