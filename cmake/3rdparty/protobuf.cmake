# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# set protobuf library name
set (PROTOBUF_NAME protobuf)

# set spdlog library path
get_filename_component (PROTOBUF_PATH "${AXCL_ROOT_DIR}/3rdparty/protobuf" ABSOLUTE)

# add protoc executable search path
if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    if (CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "^(aarch64|AARCH64|arm64|ARM64)")
        set (Protobuf_PROTOC_EXECUTABLE "${PROTOBUF_PATH}/arm64/bin/protoc" CACHE FILEPATH "Protobuf protoc executable")
    elseif (CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "^(amd64|AMD64|x86_64)")
        set (Protobuf_PROTOC_EXECUTABLE "${PROTOBUF_PATH}/x64/bin/protoc" CACHE FILEPATH "Protobuf protoc executable")
    elseif (CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "^(riscv64|RISCV64)")
        set (Protobuf_PROTOC_EXECUTABLE "${PROTOBUF_PATH}/riscv/bin/protoc" CACHE FILEPATH "Protobuf protoc executable")
    else()
        message (FATAL_ERROR "Unsupported linux architecture ${CMAKE_HOST_SYSTEM_PROCESSOR}.")
    endif()
elseif (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    if (CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "^(amd64|AMD64|x86_64)")
        set (Protobuf_PROTOC_EXECUTABLE "${PROTOBUF_PATH}/win64/bin/protoc.exe" CACHE FILEPATH "Protobuf protoc executable")
    else()
        message (FATAL_ERROR "Unsupported windows architecture ${CMAKE_HOST_SYSTEM_PROCESSOR}.")
    endif()
else()
    message (FATAL_ERROR "Unsupported OS ${CMAKE_HOST_SYSTEM_NAME}.")
endif()

# add protobuf library search path
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    if (CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64|AARCH64|arm64|ARM64)")
        set (Protobuf_ROOT "${PROTOBUF_PATH}/arm64" CACHE PATH "Protobuf root directory")
    elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^(amd64|AMD64|x86_64)")
        set (Protobuf_ROOT "${PROTOBUF_PATH}/x64" CACHE PATH "Protobuf root directory")
    elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^(riscv64|RISCV64)")
        set (Protobuf_ROOT "${PROTOBUF_PATH}/riscv" CACHE PATH "Protobuf root directory")
    else()
        message (FATAL_ERROR "Unsupported linux architecture ${CMAKE_SYSTEM_PROCESSOR}.")
    endif()
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    if (CMAKE_SYSTEM_PROCESSOR MATCHES "^(amd64|AMD64|x86_64)")
        set (Protobuf_ROOT "${PROTOBUF_PATH}/win64" CACHE PATH "Protobuf root directory")
    else()
        message (FATAL_ERROR "Unsupported windows architecture ${CMAKE_SYSTEM_PROCESSOR}.")
    endif()
else()
    message (FATAL_ERROR "Unsupported OS ${CMAKE_SYSTEM_NAME}.")
endif()

# set searching path for Protobuf
set(Protobuf_USE_STATIC_LIBS ON)

set(Protobuf_INCLUDE_DIRS ${Protobuf_ROOT}/include)
# Set protobuf libraries based on build configuration
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(Protobuf_LIBRARIES ${Protobuf_ROOT}/lib/libprotobufd${CMAKE_STATIC_LIBRARY_SUFFIX})
    set(Protobuf_LITE_LIBRARIES ${Protobuf_ROOT}/lib/libprotobuf-lited${CMAKE_STATIC_LIBRARY_SUFFIX})
else()
    set(Protobuf_LIBRARIES ${Protobuf_ROOT}/lib/libprotobuf${CMAKE_STATIC_LIBRARY_SUFFIX})
    set(Protobuf_LITE_LIBRARIES ${Protobuf_ROOT}/lib/libprotobuf-lite${CMAKE_STATIC_LIBRARY_SUFFIX})
endif()
# add cmake help function
include (AddProjectLibrary)

add_project_library (lib${PROTOBUF_NAME} INTERFACE
    PUBLIC
        COMPILE_OPTIONS
            $<$<C_COMPILER_ID:MSVC>:
                /wd4065 # switch statement contains 'default' but no 'case' labels
                /wd4244 # 'conversion' conversion from 'type1' to 'type2', possible loss of data
                /wd4251 # 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
                /wd4267 # 'var' : conversion from 'size_t' to 'type', possible loss of data
                /wd4305 # 'identifier' : truncation from 'type1' to 'type2'
                /wd4307 # 'operator' : integral constant overflow
                /wd4309 # 'conversion' : truncation of constant value
                /wd4334 # 'operator' : result of 32-bit shift implicitly converted to 64 bits (was 64-bit shift intended?)
                /wd4355 # 'this' : used in base member initializer list
                /wd4506 # no definition for inline function 'function'
                /wd4800 # 'type' : forcing value to bool 'true' or 'false' (performance warning)
                /wd4996 # The compiler encountered a deprecated declaration.
            >
        INCLUDE_DIRECTORIES
            $<BUILD_INTERFACE:${Protobuf_INCLUDE_DIRS}>
        LINK_LIBRARIES
            $<$<BOOL:${AXCL_ENABLE_PROTOCOL_CHECK}>:${Protobuf_LIBRARIES}>
            $<$<NOT:$<BOOL:${AXCL_ENABLE_PROTOCOL_CHECK}>>:${Protobuf_LITE_LIBRARIES}>
    ALIAS
        3rdparty::protobuf
)
