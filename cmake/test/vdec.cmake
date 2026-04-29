# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# set vdec test path
get_filename_component (AXCL_VDEC_TEST_PATH "${AXCL_ROOT_DIR}/test/vdec" ABSOLUTE)
get_filename_component (AXCL_VDEC_COMMON_TEST_PATH "${AXCL_ROOT_DIR}/test/vdec/common" ABSOLUTE)
get_filename_component (AXCL_SYS_SAMPLE_PATH "${AXCL_ROOT_DIR}/sample/sys" ABSOLUTE)

# collect all source files
aux_source_directory (${AXCL_VDEC_TEST_PATH} AXCL_VDEC_TEST_SRCS)
aux_source_directory (${AXCL_VDEC_COMMON_TEST_PATH} AXCL_VDEC_COMMON_TEST_SRCS)
aux_source_directory (${AXCL_SYS_SAMPLE_PATH} AXCL_SYS_SAMPLE_SRCS)

# add vdec test
add_test_app (vdec
    NAME
        axcl_ut_vdec
    SOURCES
        ${AXCL_VDEC_TEST_SRCS}
        ${AXCL_VDEC_COMMON_TEST_SRCS}
        ${AXCL_SYS_SAMPLE_SRCS}
        ${AXCL_ROOT_DIR}/toolkit/dma_buffer.cpp
        ${AXCL_ROOT_DIR}/toolkit/elapser.cpp
    INCLUDE_DIRECTORIES
        common
    LINK_LIBRARIES
        axcl::native::vdec
        axcl::native::sys
        axcl::logger
        axcl::runtime
        3rdparty::ffmpeg
        3rdparty::spdlog
)

# install test script
install(FILES ${AXCL_VDEC_TEST_PATH}/vdec_stdStrm_test.sh
    DESTINATION bin/ut
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                GROUP_READ GROUP_EXECUTE
                WORLD_READ WORLD_EXECUTE
)
