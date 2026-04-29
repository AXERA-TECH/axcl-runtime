/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#ifndef _TEST_PARAMS_CHECK_H_
#define _TEST_PARAMS_CHECK_H_

#include "unity.h"


#define UTEST_ASSERT_EQUAL_HEX(actual, except, funcName, lineNum) \
    do { \
        if ((actual) != (except)) \
            printf("[%s:%d]: Error,except 0x%x, but get 0x%x.\n", funcName, lineNum, except, actual); \
    } while (0)


AX_S32 UTestParamsCheck(int argc, char *argv[]);


#endif