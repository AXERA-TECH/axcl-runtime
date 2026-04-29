# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# add options here for now
option (AXCL_COMM_ENABLE_PCIE       "enable PCIE pipe for mpl"          ON)
option (AXCL_COMM_ENABLE_SOCKET     "enable TCP-IP pipe for mpl"        OFF)
option (AXCL_ENABLE_PROTOCOL_CHECK  "enable protocol check"             OFF)
option (AXCL_BUILD_FFMPEG           "enable build ffmpeg"               OFF)


# Disable socket on Windows since ZMQ is disabled
if (WIN32)
    set (AXCL_COMM_ENABLE_SOCKET OFF CACHE BOOL "enable TCP-IP pipe for mpl" FORCE)
    message (STATUS "Socket communication disabled on Windows (ZMQ not available)")
endif()

# check COMPILE_USER
if (NOT DEFINED CONFIG_COMPILE_USER OR "${CONFIG_COMPILE_USER}" MATCHES "^[[:space:]]*$")
    if (WIN32 AND DEFINED ENV{USERNAME})
        set (COMPILE_USER "$ENV{USERNAME}" CACHE STRING "Compile user name")
    elseif (DEFINED ENV{USER})
        set (COMPILE_USER "$ENV{USER}" CACHE STRING "Compile user name")
    else()
        message (WARNING "CONFIG_COMPILE_USER is not defined, please set it in your CMake configuration.")
        set (COMPILE_USER "DEVEL" CACHE STRING "Compile user name")
    endif()
else()
    set (COMPILE_USER ${CONFIG_COMPILE_USER} CACHE STRING "Compile user name")
endif()

# check BUILD_VERSION
if (NOT DEFINED CONFIG_BUILD_VERSION OR "${CONFIG_BUILD_VERSION}" MATCHES "^[[:space:]]*$")
    message (WARNING "CONFIG_BUILD_VERSION is not defined, please set it in your CMake configuration.")
    set (BUILD_VERSION "0.0.0" CACHE STRING "Build version")
else()
    set (BUILD_VERSION ${CONFIG_BUILD_VERSION} CACHE STRING "Build version")
endif()

message (STATUS "COMPILE_USER : ${COMPILE_USER}")
message (STATUS "BUILD_VERSION: ${BUILD_VERSION}")
message (STATUS "BUILD_FFMPEG : ${AXCL_BUILD_FFMPEG}")

# check system, add global compile definitions WINDOWS, LINUX, MACOS
function (check_win_msys2)
    if (DEFINED WIN_MSYS2_PATH AND WIN_MSYS2_PATH AND IS_DIRECTORY "${WIN_MSYS2_PATH}")
        set(WIN_MSYS2_PATH "${WIN_MSYS2_PATH}" CACHE PATH "MSYS2 installation directory" FORCE)
    else()
        set (POSSIBLE_PATHS
            "C:/msys64"
            "D:/msys64"
            "$ENV{USERPROFILE}/msys64"
            "$ENV{ProgramFiles}/msys64"
        )

        foreach (path IN LISTS POSSIBLE_PATHS)
            if (IS_DIRECTORY "${path}")
                message(STATUS "Auto detected MSYS2: ${path}")
                set(WIN_MSYS2_PATH "${path}" CACHE PATH "MSYS2 installation directory" FORCE)
                break()
            endif()
        endforeach()
    endif()

    if (NOT DEFINED WIN_MSYS2_PATH OR NOT WIN_MSYS2_PATH OR NOT IS_DIRECTORY "${WIN_MSYS2_PATH}")
        message(FATAL_ERROR "MSYS2 not installed, please install MSYS2 at first, then cmake -DWIN_MSYS2_PATH=<path> eg: D:/msys64")
    endif()

    set (BASH_PATH "${WIN_MSYS2_PATH}/usr/bin/bash.exe")
    if (NOT EXISTS "${BASH_PATH}")
        message(FATAL_ERROR "${BASH_PATH} not found!")
    endif()

    set (WIN_MSYS2_BASH "${BASH_PATH}" CACHE INTERNAL "MSYS2 bash path")
endfunction()

if (WIN32)
    if (AXCL_BUILD_FFMPEG)
        check_win_msys2()
    endif()

    set (SYSTEM_IS_WINDOWS TRUE CACHE BOOL "Is Windows System")
    mark_as_advanced (SYSTEM_IS_WINDOWS)
else ()
    set (SYSTEM_IS_WINDOWS FALSE CACHE BOOL "Is Windows System")
    mark_as_advanced (SYSTEM_IS_WINDOWS)
endif ()
if (LINUX)
    set (SYSTEM_IS_LINUX TRUE CACHE BOOL "Is Linux System")
    mark_as_advanced (SYSTEM_IS_LINUX)
else ()
    set (SYSTEM_IS_LINUX FALSE CACHE BOOL "Is Linux System")
    mark_as_advanced (SYSTEM_IS_LINUX)
endif ()
if (APPLE)
    set (SYSTEM_IS_MACOS TRUE CACHE BOOL "Is MacOS System")
    mark_as_advanced (SYSTEM_IS_MACOS)
else ()
    set (SYSTEM_IS_MACOS FALSE CACHE BOOL "Is MacOS System")
    mark_as_advanced (SYSTEM_IS_MACOS)
endif ()
