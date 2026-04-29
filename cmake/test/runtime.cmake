# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# add rt test
add_test_app (rt/rt NAME axcl_ut_rt_init)

# add device test
add_test_app (rt/device NAME axcl_ut_rt_device)

# add context test
add_test_app (rt/context NAME axcl_ut_rt_context)

# add stream test
add_test_app (rt/stream NAME axcl_ut_rt_stream)

# add memory test
add_test_app (rt/memory
    NAME
        axcl_ut_rt_memory
    SOURCES
        ${AXCL_ROOT_DIR}/toolkit/dma_buffer.cpp
        ${AXCL_ROOT_DIR}/toolkit/elapser.cpp
    LINK_LIBRARIES
        axcl::logger
        3rdparty::spdlog
)

# add engine test
add_test_app (rt/engine NAME axcl_ut_rt_engine)

# add engine test
add_test_app (rt/gather NAME axcl_ut_rt_gather)

# add engine test
add_test_app (rt/parallel NAME axcl_ut_rt_parallel)

# add latency test
add_test_app (rt/latency
    NAME
        axcl_ut_rt_latency
    SOURCES
        ${AXCL_ROOT_DIR}/toolkit/elapser.cpp
)

# add p2p test
add_test_app (rt/p2p NAME axcl_ut_rt_p2p)

# add event test
add_test_app (rt/event NAME axcl_ut_rt_event)

# add usrwork test
add_test_app (rt/usrwork
    NAME
        axcl_ut_rt_usrwork
    SOURCES
        ${AXCL_ROOT_DIR}/toolkit/elapser.cpp
    LINK_LIBRARIES
        axcl::runtime
        3rdparty::gtest
)
