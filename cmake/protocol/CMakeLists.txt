# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# set spdlog library name
set (AXCL_PROTOCOL_NAME axcl_proto)

# set bsp library path
get_filename_component (AXCL_PROTO_PATH "${AXCL_ROOT_DIR}/protocol/proto" ABSOLUTE)
get_filename_component (AXCL_PROTO_GEN_PATH "${CMAKE_BINARY_DIR}/proto" ABSOLUTE)

# config protobuf protocol check
if (AXCL_ENABLE_PROTOCOL_CHECK)
    set (AXCL_PROTOCOL_OPTIMIZED_OPTION SPEED)
else()
    set (AXCL_PROTOCOL_OPTIMIZED_OPTION LITE_RUNTIME)
endif()

# configure proto files
file (GLOB_RECURSE PROTOCOL_PROTO_FILES ${AXCL_PROTO_PATH}/*.proto)
foreach (file ${PROTOCOL_PROTO_FILES})
    file(RELATIVE_PATH rel_file ${AXCL_PROTO_PATH} ${file})
    message("-- Generate proto file: ${rel_file} to ${AXCL_PROTO_GEN_PATH}/${rel_file}...")
    configure_file (${AXCL_PROTO_PATH}/${rel_file} ${AXCL_PROTO_GEN_PATH}/${rel_file})
endforeach()

# find all protobuf files
file (GLOB_RECURSE PROTOCOL_PROTO_FILES ${AXCL_PROTO_GEN_PATH}/*.proto)

# auto generate protobuf files
foreach (file ${PROTOCOL_PROTO_FILES})
    file(RELATIVE_PATH rel_file ${AXCL_PROTO_GEN_PATH} ${file})

    message("-- Compile proto file: ${rel_file}...")
    execute_process (
        COMMAND
            ${Protobuf_PROTOC_EXECUTABLE}
            --cpp_out=${AXCL_PROTO_GEN_PATH}
            -I=${AXCL_PROTO_GEN_PATH}
            ${file}
    )
endforeach()
