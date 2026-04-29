# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# check CMake version
cmake_minimum_required (VERSION 3.14 FATAL_ERROR)

# disable inplace builds to prevent source tree corruption.
if (" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
    message (FATAL_ERROR "FATAL: Building inplace are not allowed. You should create a separate directory for Building.")
endif ()

# set cmake_install_prefix path
if (NOT DEFINED CMAKE_INSTALL_PREFIX)
    set (CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Installation Directory")
endif()

# check toolchain about
if (CMAKE_TOOLCHAIN_FILE)
    set (LIBRARY_OUTPUT_PATH_ROOT ${CMAKE_BINARY_DIR} CACHE PATH "root for library output, set this to change where android libs are compiled to")
    # get absolute path, but get_filename_component ABSOLUTE only refer with source dir, so find_file here :(
    get_filename_component (CMAKE_TOOLCHAIN_FILE_NAME ${CMAKE_TOOLCHAIN_FILE} NAME)
    find_file (CMAKE_TOOLCHAIN_FILE ${CMAKE_TOOLCHAIN_FILE_NAME} PATHS ${CMAKE_SOURCE_DIR} NO_DEFAULT_PATH)
endif()

# check if building type is not set
if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE release CACHE STRING "Choose the type of build" FORCE)
endif()

# add cmake modules path
list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/modules")

# one should have a name finally
project (axcl C CXX)

# set MSVC runtime library to use MD/MDd appropriately
# Debug builds use MDd, Release builds use MD
if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")

# set debug postfix for libraries and executables
# Debug builds will have 'd' suffix (e.g., libaxcl_rt.dll -> libaxcl_rtd.dll)
set(CMAKE_DEBUG_POSTFIX "d")

# set project root directory
get_filename_component (AXCL_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)

# set common header path
get_filename_component (AXCL_EXTERNAL_HEADER_PATH "${AXCL_ROOT_DIR}/include/external" ABSOLUTE)
get_filename_component (AXCL_INTERNAL_HEADER_PATH "${AXCL_ROOT_DIR}/include/internal" ABSOLUTE)
get_filename_component (HOME_PATH "${AXCL_ROOT_DIR}/.." ABSOLUTE)
get_filename_component (HOME_EXTERNAL_HEADER_PATH "${HOME_PATH}/header/external" ABSOLUTE)
get_filename_component (AXCL_INSTALL_INCLUDE_PATH "${CMAKE_INSTALL_PREFIX}/include" ABSOLUTE)
get_filename_component (AXCL_INSTALL_LIBRARY_PATH "${CMAKE_INSTALL_PREFIX}/lib" ABSOLUTE)


if (MSVC)
    # set debug environment for visual studio IDE
    set(SPDLOG_LIB_DIR "${AXCL_ROOT_DIR}/3rdparty/spdlog/win64/lib")
    set(FFMPEG_LIB_DIR "${AXCL_ROOT_DIR}/3rdparty/ffmpeg/win64/lib;${AXCL_ROOT_DIR}/3rdparty/ffmpeg/win64/mingw64")

    set(VSDEBUG_ENV
        "PATH=${SPDLOG_LIB_DIR};${FFMPEG_LIB_DIR}"
        CACHE INTERNAL "Visual Studio debugger environment"
    )
endif()

if (NOT IS_DIRECTORY "${AXCL_INSTALL_INCLUDE_PATH}")
    file (MAKE_DIRECTORY "${AXCL_INSTALL_INCLUDE_PATH}")
endif()
if (NOT IS_DIRECTORY "${AXCL_INSTALL_LIBRARY_PATH}")
    file (MAKE_DIRECTORY "${AXCL_INSTALL_LIBRARY_PATH}")
endif()

# install axcl header files
if (IS_DIRECTORY "${AXCL_EXTERNAL_HEADER_PATH}")
    file (COPY "${AXCL_EXTERNAL_HEADER_PATH}/" DESTINATION "${AXCL_INSTALL_INCLUDE_PATH}/")
endif()

# install native header files
if (IS_DIRECTORY "${HOME_EXTERNAL_HEADER_PATH}")
    find_package(Python3 REQUIRED)
    set(PYTHON_SCRIPT "${AXCL_ROOT_DIR}/scripts/remove_func.py")
    set(HEADER_FILES
        "ax_base_type.h"
        "ax_global_type.h"
        "ax_buffer_tool.h"
        "ax_pool_type.h"
        "ax_codec_comm.h"
        "ax_dmadim_api.h"
        "ax_dsp_api.h"
        "ax_engine_type.h"
        "ax_ive_type.h"
        "ax_ivps_type.h"
        "ax_ivps_api.h"
        "ax_sys_api.h"
        "ax_vdec_type.h"
        "ax_venc_comm.h"
        "ax_venc_exif.h"
        "ax_venc_rc.h"
    )

    foreach(header_file ${HEADER_FILES})
        set(src "${HOME_EXTERNAL_HEADER_PATH}/${header_file}")
        set(dst "${AXCL_INSTALL_INCLUDE_PATH}/${header_file}")
        if (EXISTS "${src}")
            execute_process(
                COMMAND ${Python3_EXECUTABLE} "${PYTHON_SCRIPT}" "${src}" "${dst}"
                OUTPUT_QUIET
            )
        endif()
    endforeach()
endif()

# add options
add_subdirectory (options)

# add 3rdparty
add_subdirectory (3rdparty)

# add logger
add_subdirectory (logger)

# add driver
add_subdirectory (driver)

# add protocol
add_subdirectory (protocol)

# add host about libraries
add_subdirectory (host)

# add device about libraries
if (SYSTEM_IS_LINUX)
    add_subdirectory (device)
endif ()

# add sample
add_subdirectory (sample)

# add test
add_subdirectory (test)
