# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#


# add runtime benchmark tool
add_sample_app (engine
    NAME
        axcl_run_model
    PATH
        engine/apps/benchmark
        engine/utilities
        engine/common
        engine/middleware
    COMPILE_DEFINITIONS
        ENV_AXCL_RUNTIME_API_ENABLE
        ENV_AXCL_NATIVE_API_ENABLE
        ENV_CHIP_SERIES_MC50
    LINK_LIBRARIES
        axcl::runtime
        axcl::native::npu
)

# add runtime sample classification
if (NOT SYSTEM_IS_WINDOWS)
add_sample_app (engine
    NAME
        axcl_sample_classification
    PATH
        engine/apps/classification
        engine/utilities
        engine/common
        engine/middleware
    COMPILE_DEFINITIONS
        ENV_AXCL_RUNTIME_API_ENABLE
        ENV_AXCL_NATIVE_API_ENABLE
        ENV_CHIP_SERIES_MC50

    LINK_LIBRARIES
        axcl::runtime
        axcl::native::npu
        3rdparty::opencv
)

# add runtime sample yolov5s
add_sample_app (engine
    NAME
        axcl_sample_yolov5s
    PATH
        engine/apps/yolov5s
        engine/utilities
        engine/common
        engine/middleware
    COMPILE_DEFINITIONS
        ENV_AXCL_RUNTIME_API_ENABLE
        ENV_AXCL_NATIVE_API_ENABLE
        ENV_CHIP_SERIES_MC50

    LINK_LIBRARIES
        axcl::runtime
        axcl::native::npu
        3rdparty::opencv
)
endif()