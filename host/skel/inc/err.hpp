/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#pragma once
#include "axcl_base.h"

typedef enum {
    AXCL_ERR_SKEL_BASE_ERR = AX_ERR_BUTT,
    AXCL_ERR_SKEL_FAIL, /* common failure */
    AXCL_ERR_SKEL_BUTT
} AXCL_ERR_SKEL_E;

#define AXCL_ERR_SKEL(e) AX_DEF_ERR(AX_ID_SKEL, 1, (e))
