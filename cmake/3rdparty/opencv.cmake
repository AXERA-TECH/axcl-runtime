# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# opencv needs pthread library
find_package (Threads REQUIRED)

# set opencv library name
set (OPENCV_NAME opencv)

# set opencv library linking path
get_filename_component (OPENCV_PATH "${AXCL_ROOT_DIR}/3rdparty/${OPENCV_NAME}" ABSOLUTE)
if (CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64.*|AARCH64.*|arm64.*|ARM64.*)")
    get_filename_component (OPENCV_LIB_PATH "${OPENCV_PATH}/lib/arm64" ABSOLUTE)
endif()
if (CMAKE_SYSTEM_PROCESSOR MATCHES "amd64.*|x86_64.*|AMD64.*")
    get_filename_component (OPENCV_LIB_PATH "${OPENCV_PATH}/lib/x64" ABSOLUTE)
endif()

# add cmake help function
include (AddProjectLibrary)

# add opencv interface target
add_project_library (lib${OPENCV_NAME} INTERFACE
    PUBLIC
        INCLUDE_DIRECTORIES
            $<BUILD_INTERFACE:${OPENCV_PATH}/include>
            $<BUILD_INTERFACE:${OPENCV_PATH}/include/opencv4>
        LINK_DIRECTORIES
            $<BUILD_INTERFACE:${OPENCV_LIB_PATH}>
            $<BUILD_INTERFACE:${OPENCV_LIB_PATH}/opencv4/3rdparty>
        LINK_LIBRARIES
            opencv_imgcodecs
            opencv_imgproc
            opencv_core
            dl m rt
            libjpeg-turbo
            libwebp
            libpng
            libtiff
            libopenjp2
            zlib
            Threads::Threads
    ALIAS
        3rdparty::opencv
)
