# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# set misc test paths
get_filename_component (AXCL_MISC_THREAD_TEST_PATH "${AXCL_ROOT_DIR}/test/misc/thread" ABSOLUTE)
get_filename_component (AXCL_MISC_DRV_HOST_TEST_PATH "${AXCL_ROOT_DIR}/test/misc/drv/host" ABSOLUTE)
get_filename_component (AXCL_MISC_DRV_DEVICE_TEST_PATH "${AXCL_ROOT_DIR}/test/misc/drv/device" ABSOLUTE)
get_filename_component (AXCL_MISC_LOGGER_TEST_PATH "${AXCL_ROOT_DIR}/test/misc/logger" ABSOLUTE)
get_filename_component (AXCL_MISC_MEM_TEST_PATH "${AXCL_ROOT_DIR}/test/misc/mem" ABSOLUTE)

# collect all source files
aux_source_directory (${AXCL_MISC_THREAD_TEST_PATH} AXCL_MISC_THREAD_TEST_SRCS)
aux_source_directory (${AXCL_MISC_DRV_HOST_TEST_PATH} AXCL_MISC_DRV_HOST_TEST_SRCS)
aux_source_directory (${AXCL_MISC_DRV_DEVICE_TEST_PATH} AXCL_MISC_DRV_DEVICE_TEST_SRCS)
aux_source_directory (${AXCL_MISC_LOGGER_TEST_PATH} AXCL_MISC_LOGGER_TEST_SRCS)
aux_source_directory (${AXCL_MISC_MEM_TEST_PATH} AXCL_MISC_MEM_TEST_SRCS)

# add thread test
add_test_app (misc/thread
    NAME
        axcl_ut_thread
    SOURCES
        ${AXCL_MISC_THREAD_TEST_SRCS}
        ${AXCL_ROOT_DIR}/toolkit/threadx.cpp
        ${AXCL_ROOT_DIR}/toolkit/event.cpp
        ${AXCL_ROOT_DIR}/toolkit/elapser.cpp
    LINK_LIBRARIES
        axcl::runtime
        3rdparty::gtest
)

# add driver test
if (NOT SYSTEM_IS_WINDOWS)
add_test_app (misc/drv
    NAME
        axcl_ut_misc_drv
    SOURCES
        ${AXCL_MISC_DRV_HOST_TEST_SRCS}
        ${AXCL_MISC_DRV_DEVICE_TEST_SRCS}
    LINK_LIBRARIES
        axcl::runtime
)
endif()

# add logger test
add_test_app (misc/logger
    NAME
        axcl_ut_logger
    SOURCES
        ${AXCL_MISC_LOGGER_TEST_SRCS}
    LINK_LIBRARIES
        axcl::logger
        axcl::runtime
        3rdparty::spdlog
        3rdparty::gtest
)

# add memory test
if (NOT SYSTEM_IS_WINDOWS)
add_test_app (misc/mem
    NAME
        axcl_ut_mem_helper
    SOURCES
        ${AXCL_MISC_MEM_TEST_SRCS}
        ${AXCL_ROOT_DIR}/toolkit/uni_buffer.cpp
        ${AXCL_ROOT_DIR}/toolkit/mem_helper.cpp
        ${AXCL_ROOT_DIR}/toolkit/elapser.cpp
    LINK_LIBRARIES
        axcl::runtime
        3rdparty::gtest
)
endif()