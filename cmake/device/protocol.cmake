# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# set slave protocol static library name
set (AXCL_DEVICE_PROTOCOL_NAME ax_device_proto)

# set bsp library path
get_filename_component (AXCL_PROTO_GEN_PATH "${CMAKE_BINARY_DIR}/proto" ABSOLUTE)

# find all protobuf generated files
file (GLOB_RECURSE PROTOCOL_GENERATED_FILES ${AXCL_PROTO_GEN_PATH}/*.cc)

# add cmake help function
include (AddProjectLibrary)

# add protocol library
add_project_library (${AXCL_DEVICE_PROTOCOL_NAME} STATIC
    SOURCES
        ${PROTOCOL_GENERATED_FILES}
    PUBLIC
        INCLUDE_DIRECTORIES
            $<BUILD_INTERFACE:${AXCL_PROTO_GEN_PATH}>
        LINK_LIBRARIES
            3rdparty::protobuf
    ALIAS
        axcl::device::protocol
)
