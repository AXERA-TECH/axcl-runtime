/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#ifndef _TEST_CASE_H_
#define _TEST_CASE_H_

#include "unity.h"


int UTestParamsCheck(int argc, char *argv[]);





#define USE_AXCL_VENC

#define UTEST_ASSERT_EQUAL_HEX(actual, except, funcName, lineNum) \
    do { \
        if ((actual) != (except)) \
            printf("[%s:%d]: Error,except 0x%x, but get 0x%x.\n", funcName, lineNum, except, actual); \
    } while (0)



#ifdef USE_AXCL_VENC

#define AX_VENC_Init AXCL_VENC_Init
#define AX_VENC_Deinit AXCL_VENC_Deinit
#define AX_VENC_CreateChn AXCL_VENC_CreateChn
#define AX_VENC_CreateChnEx AXCL_VENC_CreateChnEx
#define AX_VENC_DestroyChn  AXCL_VENC_DestroyChn
#define AX_VENC_SendFrame AXCL_VENC_SendFrame
#define AX_VENC_SendFrameEx AXCL_VENC_SendFrameEx
#define AX_VENC_SelectGrp AXCL_VENC_SelectGrp
#define AX_VENC_SelectClearGrp AXCL_VENC_SelectClearGrp
#define AX_VENC_SelectGrpAddChn  AXCL_VENC_SelectGrpAddChn
#define AX_VENC_SelectGrpDeleteChn  AXCL_VENC_SelectGrpDeleteChn
#define AX_VENC_SelectGrpQuery AXCL_VENC_SelectGrpQuery
#define AX_VENC_GetFd AXCL_VENC_GetFd
#define AX_VENC_GetStream AXCL_VENC_GetStream
#define AX_VENC_ReleaseStream AXCL_VENC_ReleaseStream
#define AX_VENC_GetStreamBufInfo AXCL_VENC_GetStreamBufInfo
#define AX_VENC_StartRecvFrame AXCL_VENC_StartRecvFrame
#define AX_VENC_StopRecvFrame AXCL_VENC_StopRecvFrame
#define AX_VENC_ResetChn AXCL_VENC_ResetChn
#define AX_VENC_SetRoiAttr AXCL_VENC_SetRoiAttr
#define AX_VENC_GetRoiAttr AXCL_VENC_GetRoiAttr
#define AX_VENC_SetRcParam AXCL_VENC_SetRcParam
#define AX_VENC_GetRcParam AXCL_VENC_GetRcParam
#define AX_VENC_SetVuiParam AXCL_VENC_SetVuiParam
#define AX_VENC_GetVuiParam AXCL_VENC_GetVuiParam
#define AX_VENC_SetChnAttr AXCL_VENC_SetChnAttr
#define AX_VENC_GetChnAttr AXCL_VENC_GetChnAttr
#define AX_VENC_SetRateJamStrategy AXCL_VENC_SetRateJamStrategy
#define AX_VENC_GetRateJamStrategy AXCL_VENC_GetRateJamStrategy
#define AX_VENC_SetSuperFrameStrategy AXCL_VENC_SetSuperFrameStrategy
#define AX_VENC_GetSuperFrameStrategy AXCL_VENC_GetSuperFrameStrategy
#define AX_VENC_SetIntraRefresh AXCL_VENC_SetIntraRefresh
#define AX_VENC_GetIntraRefresh AXCL_VENC_GetIntraRefresh
#define AX_VENC_SetUsrData AXCL_VENC_SetUsrData
#define AX_VENC_GetUsrData AXCL_VENC_GetUsrData
#define AX_VENC_SetSliceSplit AXCL_VENC_SetSliceSplit
#define AX_VENC_GetSliceSplit AXCL_VENC_GetSliceSplit
#define AX_VENC_RequestIDR AXCL_VENC_RequestIDR
#define AX_VENC_QueryStatus AXCL_VENC_QueryStatus
#define AX_VENC_SetJpegParam AXCL_VENC_SetJpegParam
#define AX_VENC_GetJpegParam AXCL_VENC_GetJpegParam
#define AX_VENC_JpegEncodeOneFrame AXCL_VENC_JpegEncodeOneFrame
#define AX_VENC_JpegGetThumbnail AXCL_VENC_JpegGetThumbnail

#endif




#endif