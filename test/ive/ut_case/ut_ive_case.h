/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#ifndef __UT_IVE_CASE_H__
#define __UT_IVE_CASE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include "../ut_ive_common.h"

#define SAMPLE_IVE_MAX_OP_NUM (28)
#define SAMPLE_IVE_MAX_THREAD_NUM (16)
#define SAMPLE_IVE_MAX_PROCESS_NUM (16)

extern AX_BOOL g_bCaseTestSuccess;

AX_S32 SAMPLE_IVE_DMA_TEST(AX_S32 as32Type[], AX_CHAR *pchSrcPath, AX_CHAR *pchDstPath, AX_U32 u32WidthSrc, AX_U32 u32HeightSrc, AX_CHAR *pchParamsList);

AX_VOID SAMPLE_IVE_DMA_TEST_HandleSig(AX_VOID);

AX_S32 SAMPLE_IVE_DualPicCalc_TEST(AX_U32 u32Mode, AX_S32 as32Type[], AX_CHAR **pchSrcPath, AX_CHAR *pchDstPath, AX_U32 u32WidthSrc, AX_U32 u32HeightSrc, AX_CHAR *pchParamsList);

AX_VOID SAMPLE_IVE_DualPicCalc_TEST_HandleSig(AX_VOID);

AX_S32 SAMPLE_IVE_EdgeDetection_TEST(AX_U32 u32Mode, AX_S32 as32Type[], AX_CHAR **pchSrcPath, AX_CHAR *pchDstPath, AX_U32 u32WidthSrc, AX_U32 u32HeightSrc, AX_CHAR *pchParamsList);

AX_VOID SAMPLE_IVE_EdgeDetection_TEST_HandleSig(AX_VOID);

AX_S32 SAMPLE_IVE_CCL_TEST(AX_S32 as32Type[], AX_CHAR *pchSrcPath, AX_CHAR **pchDstPath, AX_U32 u32WidthSrc, AX_U32 u32HeightSrc, AX_CHAR *pchParamsList);

AX_VOID SAMPLE_IVE_CCL_TEST_HandleSig(AX_VOID);

AX_S32 SAMPLE_IVE_ED_TEST(AX_U32 u32Mode, AX_S32 as32Type[], AX_CHAR *pchSrcPath, AX_CHAR *pchDstPath, AX_U32 u32WidthSrc, AX_U32 u32HeightSrc, AX_CHAR *pchParamsList);

AX_VOID SAMPLE_IVE_ED_TEST_HandleSig(AX_VOID);

AX_S32 SAMPLE_IVE_Filter_TEST(AX_S32 as32Type[], AX_CHAR *pchSrcPath, AX_CHAR *pchDstPath, AX_U32 u32WidthSrc, AX_U32 u32HeightSrc, AX_CHAR *pchParamsList);

AX_VOID SAMPLE_IVE_Filter_TEST_HandleSig(AX_VOID);

AX_S32 SAMPLE_IVE_Hist_TEST(AX_U32 u32Mode, AX_S32 as32Type[], AX_CHAR *pchSrcPath, AX_CHAR *pchDstPath, AX_U32 u32WidthSrc, AX_U32 u32HeightSrc, AX_CHAR *pchParamsList);

AX_VOID SAMPLE_IVE_Hist_TEST_HandleSig(AX_VOID);

AX_S32 SAMPLE_IVE_Integ_TEST(AX_S32 as32Type[], AX_CHAR *pchSrcPath, AX_CHAR *pchDstPath, AX_U32 u32WidthSrc, AX_U32 u32HeightSrc, AX_CHAR *pchParamsList);

AX_VOID SAMPLE_IVE_Integ_TEST_HandleSig(AX_VOID);

AX_S32 SAMPLE_IVE_MagAng_TEST(AX_S32 as32Type[], AX_CHAR **pchSrcPath, AX_CHAR **pchDstPath, AX_U32 u32WidthSrc, AX_U32 u32HeightSrc);

AX_VOID SAMPLE_IVE_MagAng_TEST_HandleSig(AX_VOID);

AX_S32 SAMPLE_IVE_Sobel_TEST(AX_S32 as32Type[], AX_CHAR *pchSrcPath, AX_CHAR *pchDstPath, AX_U32 u32WidthSrc, AX_U32 u32HeightSrc, AX_CHAR *pchParamsList);

AX_VOID SAMPLE_IVE_Sobel_TEST_HandleSig(AX_VOID);

AX_S32 SAMPLE_IVE_GMM_TEST(AX_U32 u32Mode, AX_S32 as32Type[], AX_CHAR **pchSrcPath, AX_CHAR **pchDstPath, AX_U32 u32WidthSrc, AX_U32 u32HeightSrc, AX_CHAR *pchParamsList);

AX_VOID SAMPLE_IVE_GMM_TEST_HandleSig(AX_VOID);

AX_S32 SAMPLE_IVE_Thresh_TEST(AX_S32 as32Type[], AX_CHAR *pchSrcPath, AX_CHAR *pchDstPath, AX_U32 u32WidthSrc, AX_U32 u32HeightSrc, AX_CHAR *pchParamsList);

AX_VOID SAMPLE_IVE_Thresh_TEST_HandleSig(AX_VOID);

AX_S32 SAMPLE_IVE_16To8Bit_TEST(AX_S32 as32Type[], AX_CHAR *pchSrcPath, AX_CHAR *pchDstPath, AX_U32 u32WidthSrc, AX_U32 u32HeightSrc, AX_CHAR *pchParamsList);

AX_VOID SAMPLE_IVE_16To8Bit_TEST_HandleSig(AX_VOID);

AX_S32 SAMPLE_IVE_CropResize_TEST(AX_U32 u32Engine, AX_U32 u32Mode, AX_S32 as32Type[], AX_CHAR *pchSrcPath, AX_CHAR *pchDstPath, AX_U32 u32WidthSrc, AX_U32 u32HeightSrc, AX_CHAR *pchParamsList);

AX_VOID SAMPLE_IVE_CropResize_TEST_HandleSig(AX_VOID);

AX_S32 SAMPLE_IVE_CSC_TEST(AX_U32 u32Engine, AX_S32 as32Type[], AX_CHAR *pchSrcPath, AX_CHAR *pchDstPath, AX_U32 u32WidthSrc, AX_U32 u32HeightSrc);

AX_VOID SAMPLE_IVE_CSC_TEST_HandleSig(AX_VOID);

AX_VOID SAMPLE_IVE_TestMultiCalc(AX_S32 as32Type[], AX_CHAR *pchSrcPath, AX_CHAR *pchDstPath, AX_U32 u32WidthSrc, AX_U32 u32HeightSrc);

AX_VOID SAMPLE_IVE_TestMultiCalc_HandleSig(AX_VOID);

AX_S32 SAMPLE_IVE_TestCases_ThreadStart(AX_U32 u32ThreadNum, AX_U32 *pu32DevId);

AX_VOID SAMPLE_IVE_TestCases_ThreadStop(AX_U32 u32ThreadNum);

AX_S32 SAMPLE_IVE_CropResize2_TEST(AX_U32 u32Engine, AX_U32 u32Mode, AX_S32 as32Type[], AX_CHAR **pchSrcPath, AX_CHAR *pchDstPath, AX_U32 u32WidthSrc, AX_U32 u32HeightSrc, AX_CHAR *pchParamsList);

AX_VOID SAMPLE_IVE_CropResize2_TEST_HandleSig(AX_VOID);

AX_S32 SAMPLE_IVE_TestCases_Process(AX_U32 u32Duration, AX_U32 u32ProcessNum);

#endif