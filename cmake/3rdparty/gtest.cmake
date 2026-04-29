# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# gtest needs Threads library
find_package (Threads REQUIRED)

# set gtest library name
set (GTEST_NAME gtest)

# add gtest library search path
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    if (CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64|AARCH64|arm64|ARM64)")
        get_filename_component (GTEST_PATH "${AXCL_ROOT_DIR}/3rdparty/googletest/arm64" ABSOLUTE)
    elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^(amd64|AMD64|x86_64)")
        get_filename_component (GTEST_PATH "${AXCL_ROOT_DIR}/3rdparty/googletest/x64" ABSOLUTE)
    elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^(riscv64|RISCV64)")
        get_filename_component (GTEST_PATH "${AXCL_ROOT_DIR}/3rdparty/googletest/riscv" ABSOLUTE)
    else()
        message (FATAL_ERROR "Unsupported linux architecture ${CMAKE_SYSTEM_PROCESSOR}.")
    endif()
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    if (CMAKE_SYSTEM_PROCESSOR MATCHES "^(AMD64|amd64|x86_64)")
        get_filename_component (GTEST_PATH "${AXCL_ROOT_DIR}/3rdparty/googletest/win64" ABSOLUTE)
    else()
        message (FATAL_ERROR "Unsupported windows architecture ${CMAKE_SYSTEM_PROCESSOR}.")
    endif()
else()
    message (FATAL_ERROR "Unsupported OS ${CMAKE_SYSTEM_NAME}.")
endif()

# Set gtest libraries based on build configuration
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(GTEST_LIB_PATH ${GTEST_PATH}/lib/libgtestd${CMAKE_STATIC_LIBRARY_SUFFIX})
    set(GTEST_MAIN_LIB_PATH ${GTEST_PATH}/lib/libgtest_maind${CMAKE_STATIC_LIBRARY_SUFFIX})
else()
    set(GTEST_LIB_PATH ${GTEST_PATH}/lib/libgtest${CMAKE_STATIC_LIBRARY_SUFFIX})
    set(GTEST_MAIN_LIB_PATH ${GTEST_PATH}/lib/libgtest_main${CMAKE_STATIC_LIBRARY_SUFFIX})
endif()

# add cmake help function
include (AddProjectLibrary)

# add gtest interface target
add_project_library (lib${GTEST_NAME} INTERFACE
    PUBLIC
        INCLUDE_DIRECTORIES
            $<BUILD_INTERFACE:${GTEST_PATH}/include>
        LINK_DIRECTORIES
            $<BUILD_INTERFACE:${GTEST_PATH}/lib>
        LINK_LIBRARIES
            ${GTEST_LIB_PATH}
            Threads::Threads
    ALIAS
        3rdparty::gtest
)
