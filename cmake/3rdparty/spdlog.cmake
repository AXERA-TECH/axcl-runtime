# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# spdlog needs Threads library
find_package (Threads REQUIRED)

# set spdlog library name
set (SPDLOG_NAME spdlog)

# add spdlog library search path
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    if (CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64|AARCH64|arm64|ARM64)")
        get_filename_component (SPDLOG_PATH "${AXCL_ROOT_DIR}/3rdparty/${SPDLOG_NAME}/arm64" ABSOLUTE)
    elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^(amd64|AMD64|x86_64)")
        get_filename_component (SPDLOG_PATH "${AXCL_ROOT_DIR}/3rdparty/${SPDLOG_NAME}/x64" ABSOLUTE)
    elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^(riscv64|RISCV64)")
        get_filename_component (SPDLOG_PATH "${AXCL_ROOT_DIR}/3rdparty/${SPDLOG_NAME}/riscv" ABSOLUTE)
    else()
        message (FATAL_ERROR "Unsupported linux architecture %{CMAKE_HOST_SYSTEM_PROCESSOR}.")
    endif()
    set(SPDLOG_LIB_PATH ${SPDLOG_PATH}/lib/lib${SPDLOG_NAME}.so)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    if (CMAKE_SYSTEM_PROCESSOR MATCHES "^(AMD64|amd64|x86_64)")
        get_filename_component (SPDLOG_PATH "${AXCL_ROOT_DIR}/3rdparty/${SPDLOG_NAME}/win64" ABSOLUTE)
    else()
        message (FATAL_ERROR "Unsupported windows architecture ${CMAKE_SYSTEM_PROCESSOR}.")
    endif()
    # Set spdlog libraries based on build configuration
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(SPDLOG_LIB_PATH ${SPDLOG_PATH}/lib/lib${SPDLOG_NAME}d.lib)
    else()
        set(SPDLOG_LIB_PATH ${SPDLOG_PATH}/lib/lib${SPDLOG_NAME}.lib)
    endif()
else()
    message (FATAL_ERROR "Unsupported OS ${CMAKE_HOST_SYSTEM_NAME}.")
endif()

# Set spdlog DLL based on build configuration
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(SPDLOG_BIN_PATH ${SPDLOG_PATH}/lib/lib${SPDLOG_NAME}d${CMAKE_SHARED_LIBRARY_SUFFIX})
else()
    set(SPDLOG_BIN_PATH ${SPDLOG_PATH}/lib/lib${SPDLOG_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})
endif()

# add cmake help function
include (AddProjectLibrary)

# add sys interface target
add_project_library (lib${SPDLOG_NAME} INTERFACE
    PUBLIC
        INCLUDE_DIRECTORIES
            $<BUILD_INTERFACE:${SPDLOG_PATH}/include>
        LINK_DIRECTORIES
            $<BUILD_INTERFACE:${SPDLOG_PATH}/lib>
        LINK_LIBRARIES
            $<$<BOOL:${MINGW}>:ws2_32>
            $<$<BOOL:${WIN32}>:ws2_32>
            Threads::Threads
            ${SPDLOG_LIB_PATH}
    ALIAS
        3rdparty::spdlog
)


install(
    FILES
        ${SPDLOG_BIN_PATH}
    DESTINATION bin
)
