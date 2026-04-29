# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#


# add sys sample
add_sample_app (sys
    NAME
        axcl_sample_sys
    LINK_LIBRARIES
        axcl::runtime
        axcl::native::sys
)
