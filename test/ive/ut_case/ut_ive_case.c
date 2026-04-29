/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "ut_ive_case.h"
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

AX_BOOL g_bCaseTestSuccess = AX_FALSE;

static AX_BOOL bStartThread = AX_FALSE;
static pthread_t g_tid[SAMPLE_IVE_MAX_THREAD_NUM] = {0};
static pthread_mutex_t g_lock;

typedef struct axTEST_MULTI_CALC_T {
    AX_IVE_SRC_IMAGE_T stSrc1;
    AX_IVE_SRC_IMAGE_T stSrc2;
    AX_IVE_DST_IMAGE_T stDst;
    AX_IVE_DST_MEM_INFO_T  stHist;
    AX_IVE_SUB_CTRL_T stSubCtrl;
    FILE* pFpSrc;
    FILE* pFpDst;
} TEST_MULTI_CALC_T;

static TEST_MULTI_CALC_T s_stTestMultiCalc;
/******************************************************************************
* function : test multi calc uninit
******************************************************************************/
static AX_VOID SAMPLE_IVE_TestMultiCalc_Uninit(TEST_MULTI_CALC_T* pstTestMultiCalc)
{
    IVE_CMM_FREE(pstTestMultiCalc->stSrc1.au64PhyAddr[0], pstTestMultiCalc->stSrc1.au64VirAddr[0]);
    IVE_CMM_FREE(pstTestMultiCalc->stSrc2.au64PhyAddr[0], pstTestMultiCalc->stSrc2.au64VirAddr[0]);
    IVE_CMM_FREE(pstTestMultiCalc->stDst.au64PhyAddr[0], pstTestMultiCalc->stDst.au64VirAddr[0]);
    IVE_CMM_FREE(pstTestMultiCalc->stHist.u64PhyAddr, pstTestMultiCalc->stHist.u64VirAddr);

    if (NULL != pstTestMultiCalc->pFpSrc) {
        fclose(pstTestMultiCalc->pFpSrc);
        pstTestMultiCalc->pFpSrc = NULL;
    }

    if (NULL != pstTestMultiCalc->pFpDst) {
        fclose(pstTestMultiCalc->pFpDst);
        pstTestMultiCalc->pFpDst = NULL;
    }
}
/******************************************************************************
* function : test multi calc init
******************************************************************************/
static AX_S32 SAMPLE_IVE_TestMultiCalc_Init(TEST_MULTI_CALC_T* pstTestMultiCalc, AX_CHAR* pchSrcFileName,
        AX_CHAR* pchDstFileName, AX_U32 u32Width, AX_U32 u32Height, AX_S32 as32Type[])
{
    AX_S32 s32Ret = AX_FAILURE;
    AX_U32 u32Size;
    memset(pstTestMultiCalc, 0, sizeof(TEST_MULTI_CALC_T));

    s32Ret = SAMPLE_COMM_IVE_CreateImage(&(pstTestMultiCalc->stSrc1), (AX_IVE_IMAGE_TYPE_E)IMAGE_TYPE_SPECIFY(as32Type[0], AX_IVE_IMAGE_TYPE_U8C1), u32Width, u32Height);
    if (AX_SUCCESS != s32Ret) {
        SAMPLE_IVE_PRT("Error(%#x),Create src1 image failed!\n", s32Ret);
        SAMPLE_IVE_TestMultiCalc_Uninit(pstTestMultiCalc);
        return s32Ret;
    }
    s32Ret = SAMPLE_COMM_IVE_CreateImage(&(pstTestMultiCalc->stSrc2), (AX_IVE_IMAGE_TYPE_E)IMAGE_TYPE_SPECIFY(as32Type[0], AX_IVE_IMAGE_TYPE_U8C1), u32Width, u32Height);
    if (AX_SUCCESS != s32Ret) {
        SAMPLE_IVE_PRT("Error(%#x),Create src2 image failed!\n", s32Ret);
        SAMPLE_IVE_TestMultiCalc_Uninit(pstTestMultiCalc);
        return s32Ret;
    }
    s32Ret = SAMPLE_COMM_IVE_CreateImage(&(pstTestMultiCalc->stDst), (AX_IVE_IMAGE_TYPE_E)IMAGE_TYPE_SPECIFY(as32Type[1], AX_IVE_IMAGE_TYPE_U8C1), u32Width, u32Height);
    if (AX_SUCCESS != s32Ret) {
        SAMPLE_IVE_PRT("Error(%#x),Create dst image failed!\n", s32Ret);
        SAMPLE_IVE_TestMultiCalc_Uninit(pstTestMultiCalc);
        return s32Ret;
    }

    u32Size = AX_IVE_HIST_NUM * sizeof(AX_U32);
    s32Ret = SAMPLE_COMM_IVE_CreateMemInfo(&(pstTestMultiCalc->stHist), u32Size);
    if (AX_SUCCESS != s32Ret) {
        SAMPLE_IVE_PRT("Error(%#x),Create hist mem info failed!\n", s32Ret);
        SAMPLE_IVE_TestMultiCalc_Uninit(pstTestMultiCalc);
    }
    pstTestMultiCalc->stSubCtrl.enMode = AX_IVE_SUB_MODE_ABS;

    s32Ret = AX_FAILURE;
    pstTestMultiCalc->pFpSrc = fopen(pchSrcFileName, "rb");
    if (AX_NULL == pstTestMultiCalc->pFpSrc) {
        SAMPLE_IVE_PRT("Error,Open file %s failed!\n", pchSrcFileName);
        SAMPLE_IVE_TestMultiCalc_Uninit(pstTestMultiCalc);
        return s32Ret;
    }
    pstTestMultiCalc->pFpDst = fopen(pchDstFileName, "wb");
    if (AX_NULL == pstTestMultiCalc->pFpSrc) {
        SAMPLE_IVE_PRT("Error,Open file %s failed!\n", pchDstFileName);
        SAMPLE_IVE_TestMultiCalc_Uninit(pstTestMultiCalc);
        return s32Ret;
    }

    return AX_SUCCESS;
}
/******************************************************************************
* function : test multi calc
**************************************************************************/
static AX_S32 SAMPLE_IVE_TestMultiCalcProc(TEST_MULTI_CALC_T* pstTestMultiCalc)
{
    AX_S32 s32Ret;
    AX_U32* pu32Hist;
    AX_U32 i;
    AX_IVE_HANDLE IveHandle;
    AX_BOOL bInstant = AX_FALSE;
    AX_BOOL bBlock = AX_TRUE;
    AX_BOOL bFinish = AX_FALSE;
    AX_IVE_SRC_DATA_T stSrcData;
    AX_IVE_DST_DATA_T stDstData;
    AX_IVE_DMA_CTRL_T stDmaCtrl;
    memset(&stDmaCtrl, 0, sizeof(AX_IVE_DMA_CTRL_T));
    s32Ret = SAMPLE_COMM_IVE_ReadFile(&(pstTestMultiCalc->stSrc1), pstTestMultiCalc->pFpSrc);
    if (AX_SUCCESS != s32Ret) {
        SAMPLE_IVE_PRT("Error(%#x),Read src file failed!\n",s32Ret);
        return s32Ret;
    }

    stDmaCtrl.enMode = AX_IVE_DMA_MODE_DIRECT_COPY;
    stSrcData.u64VirAddr = pstTestMultiCalc->stSrc1.au64VirAddr[0];
    stSrcData.u64PhyAddr = pstTestMultiCalc->stSrc1.au64PhyAddr[0];
    stSrcData.u32Width = pstTestMultiCalc->stSrc1.u32Width;
    stSrcData.u32Height = pstTestMultiCalc->stSrc1.u32Height;
    stSrcData.u32Stride = pstTestMultiCalc->stSrc1.au32Stride[0];

    stDstData.u64VirAddr = pstTestMultiCalc->stSrc2.au64VirAddr[0];
    stDstData.u64PhyAddr = pstTestMultiCalc->stSrc2.au64PhyAddr[0];
    stDstData.u32Width = pstTestMultiCalc->stSrc2.u32Width;
    stDstData.u32Height = pstTestMultiCalc->stSrc2.u32Height;
    stDstData.u32Stride = pstTestMultiCalc->stSrc2.au32Stride[0];
    s32Ret = AXCL_IVE_DMA(&IveHandle, &stSrcData, &stDstData, &stDmaCtrl, bInstant);
    if (AX_SUCCESS != s32Ret) {
        SAMPLE_IVE_PRT("Error(%#x),AX_IVE_DMA failed!\n",s32Ret);
        return s32Ret;
    }

    s32Ret = AXCL_IVE_Sub(&IveHandle, &pstTestMultiCalc->stSrc1, &pstTestMultiCalc->stSrc2, &pstTestMultiCalc->stDst, &pstTestMultiCalc->stSubCtrl, bInstant);
    if (AX_SUCCESS != s32Ret) {
        SAMPLE_IVE_PRT("Error(%#x),AX_IVE_Sub failed!\n",s32Ret);
        return s32Ret;
    }

    s32Ret = AXCL_IVE_Hist(&IveHandle, &pstTestMultiCalc->stDst, &pstTestMultiCalc->stHist, bInstant);
    if (AX_SUCCESS != s32Ret) {
        SAMPLE_IVE_PRT("Error(%#x),AX_IVE_Hist failed!\n",s32Ret);
        return s32Ret;
    }

    if (bInstant == AX_FALSE) {
        s32Ret = AXCL_IVE_Query(IveHandle, &bFinish, bBlock);
        while (AX_ERR_IVE_QUERY_TIMEOUT == s32Ret) {
            usleep(100 * 1000);
            s32Ret = AXCL_IVE_Query(IveHandle, &bFinish, bBlock);
        }
        if (AX_SUCCESS != s32Ret) {
            SAMPLE_IVE_PRT("Error(%#x),AX_IVE_Query failed!\n",s32Ret);
            return s32Ret;
        }
    }

    if (g_bSave) {
        s32Ret = SAMPLE_COMM_IVE_WriteFile(&pstTestMultiCalc->stDst, pstTestMultiCalc->pFpDst);
        if (AX_SUCCESS != s32Ret) {
            SAMPLE_IVE_PRT("Error,Write dst file failed!\n");
            return s32Ret;
        }
    }

    s32Ret = axclrtMemcpy((AX_VOID *)pstTestMultiCalc->stHist.u64VirAddr, (AX_VOID *)pstTestMultiCalc->stHist.u64PhyAddr, pstTestMultiCalc->stHist.u32Size, AXCL_MEMCPY_DEVICE_TO_HOST);
    if (s32Ret) {
         SAMPLE_IVE_PRT("memcpy device memory %p to host memory %p fail, ret = 0x%x\n",
                        (AX_VOID *)pstTestMultiCalc->stHist.u64PhyAddr, (AX_VOID *)pstTestMultiCalc->stHist.u64VirAddr, s32Ret);
         return s32Ret;
    }
    pu32Hist = (AX_U32*)pstTestMultiCalc->stHist.u64VirAddr;
    if (pu32Hist[0] != pstTestMultiCalc->stSrc1.u32Width * pstTestMultiCalc->stSrc1.u32Height) {
        s32Ret = AX_FAILURE;
        SAMPLE_IVE_PRT("Test multi calc error,pu32Hist[0] = %d\n", pu32Hist[0]);
        for (i = 1; i < AX_IVE_HIST_NUM; i++) {
            if (pu32Hist[i] != 0) {
                SAMPLE_IVE_PRT("Test multi calc error, pu32Hist[%d] = %d\n", i, pu32Hist[i]);
            }
        }
    } else {
        SAMPLE_IVE_PRT("pu32Hist[0]:%d\n", pu32Hist[0]);
        SAMPLE_IVE_PRT("Test multi calc success!\n");
    }

    return s32Ret;
}
/******************************************************************************
* function : Show test multi calc sample
******************************************************************************/
AX_VOID SAMPLE_IVE_TestMultiCalc(AX_S32 as32Type[], AX_CHAR *pchSrcPath, AX_CHAR *pchDstPath, AX_U32 u32WidthSrc, AX_U32 u32HeightSrc)
{
    AX_S32 s32Ret;
    AX_U32 u32Width = u32WidthSrc;
    AX_U32 u32Height = u32HeightSrc;
    AX_CHAR* pchSrcFile = pchSrcPath;
    AX_CHAR* pchDstFile = pchDstPath;
    if (!pchSrcFile || !pchDstFile) {
        SAMPLE_IVE_PRT("Error: null pointer(src or dst path not specified)!\n");
        return;
    }

    memset(&s_stTestMultiCalc, 0, sizeof(s_stTestMultiCalc));
    s32Ret = SAMPLE_IVE_TestMultiCalc_Init(&s_stTestMultiCalc, pchSrcFile, pchDstFile, u32Width, u32Height, as32Type);
    if (AX_SUCCESS != s32Ret) {
        SAMPLE_IVE_PRT("Error(%#x),SAMPLE_IVE_TestMultiCalc_Init failed!\n", s32Ret);
        return;
    }

    s32Ret =  SAMPLE_IVE_TestMultiCalcProc(&s_stTestMultiCalc);
    if (AX_SUCCESS == s32Ret)
        SAMPLE_IVE_PRT("Process success!\n");
    else
        SAMPLE_IVE_PRT("Error:process failed!\n");

    SAMPLE_IVE_TestMultiCalc_Uninit(&s_stTestMultiCalc);
    memset(&s_stTestMultiCalc, 0, sizeof(s_stTestMultiCalc));
}

/******************************************************************************
* function : Test multi calc sample signal handle
******************************************************************************/
AX_VOID SAMPLE_IVE_TestMultiCalc_HandleSig(AX_VOID)
{
    SAMPLE_IVE_TestMultiCalc_Uninit(&s_stTestMultiCalc);
    memset(&s_stTestMultiCalc, 0, sizeof(s_stTestMultiCalc));

    AXCL_IVE_Exit();
    AXCL_SYS_Deinit();
}

/******************************************************************************
*                        For unorder cases testing                            *
******************************************************************************/
static AX_S32 GetRandInRange(AX_S32 s32Min, AX_S32 s32Max)
{
    return ((rand() % (s32Max - s32Min + 1)) + s32Min);
}

static AX_S32 SAMPLE_IVE_StartOneCase(AX_U32 u32CaseIndex)
{
    AX_S32 s32Ret = AX_FAILURE;
    AX_S32 as32Type[5] = {-1, -1, -1, -1, -1};
    AX_U32 u32WidthSrc = 1280;
    AX_U32 u32HeightSrc = 720;
    switch(u32CaseIndex) {
    case 0: {//DMA
        AX_CHAR *pchPathSrc = "/opt/data/ive/common/1280x720_u8c1_gray.yuv";
        AX_CHAR *pchPathDst = "/tmp/";
        AX_U32 u32Mode = GetRandInRange(0, 3);
        AX_U8 u8HorSegSize = 0;
        AX_U8 u8VerSegRows = 0;
        AX_U8 u8ElemSize = 0;
        AX_U32 u32WidthDst = u32WidthSrc;
        AX_U32 u32HeightDst = u32HeightSrc;
        AX_U16 u16CrpX0 = 0;
        AX_U16 u16CrpY0 = 0;
        AX_U64 u64Val = 0;
        if (u32Mode == 1) {
            do {
                do {
                    u8HorSegSize = GetRandInRange(2, 255);
                } while ((u32WidthSrc / u8HorSegSize < 1) || (u32WidthSrc % u8HorSegSize != 0));
                u8ElemSize = GetRandInRange(1, u8HorSegSize - 1);
                u32WidthDst = (u32WidthSrc / u8HorSegSize) * u8ElemSize;
            } while (u32WidthDst < 32 || u32WidthDst % 16 != 0);

            do {
                u8VerSegRows = GetRandInRange(1, 255);
            } while ((u32HeightSrc / u8VerSegRows < 1) || (u32HeightSrc % u8VerSegRows != 0));
        } else if (u32Mode == 0) {
            do {
                do {
                    u32WidthDst = GetRandInRange(32, u32WidthSrc - 1);
                } while (u32WidthDst % 16 != 0);
                u32HeightDst = GetRandInRange(1, u32HeightSrc - 1);
                u16CrpX0 = GetRandInRange(0, u32WidthDst);
                u16CrpY0 = GetRandInRange(0, u32HeightDst);
            } while (u16CrpX0 + u32WidthDst > u32WidthSrc || u16CrpY0 + u32HeightDst > u32HeightSrc);
        } else {
            u64Val = rand();
        }
        SAMPLE_IVE_PRT("---DMA--- mode:%u, x0:%u, y0:%u, w_out:%u, h_out:%u, h_seg:%u, v_seg:%u, elem_size:%u, set_val:%llu\n",
        u32Mode, u16CrpX0, u16CrpY0, u32WidthDst, u32HeightDst, u8HorSegSize, u8VerSegRows, u8ElemSize, u64Val);
        AX_CHAR achParams[512] = {0};
        snprintf(achParams, 511, "{\"mode\":%u,\"x0\":%u,\"y0\":%u,\"w_out\":%u,\"h_out\":%u,\"h_seg\":%u,\"v_seg\":%u,\"elem_size\":%u,\"set_val\":%llu}",
        u32Mode, u16CrpX0, u16CrpY0, u32WidthDst, u32HeightDst, u8HorSegSize, u8VerSegRows, u8ElemSize, u64Val);
        s32Ret = SAMPLE_IVE_DMA_TEST(as32Type, pchPathSrc, pchPathDst, u32WidthSrc, u32HeightSrc, achParams);
    }
    break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6: {//Dual picture calculate
        AX_CHAR *pchPathSrc[2] = {"/opt/data/ive/common/1280x720_u8c1_gray.yuv", "/opt/data/ive/common/1280x720_u8c1_gray_2.yuv"};
        AX_CHAR *pchPathDst = "/tmp/dualpics_test_out.yuv";
        AX_U32 u32Mode = u32CaseIndex - 1;
        AX_U8 u8AddX = GetRandInRange(0, 255);
        AX_U8 u8AddY = 255 - u8AddX;
        AX_U32 u8SubMode = GetRandInRange(0, 1);
        AX_U16 u16MseCoef = GetRandInRange(0, 65535);
        SAMPLE_IVE_PRT("---DualPics:%u--- x:%u, y:%u, mode:%u, mse_coef:%u\n", u32Mode, u8AddX, u8AddY, u8SubMode, u16MseCoef);
        AX_CHAR achParams[512] = {0};
        snprintf(achParams, 511, "{\"x\":%u,\"y\":%u,\"mode\":%u,\"mse_coef\":%u}", u8AddX, u8AddY, u8SubMode, u16MseCoef);
        s32Ret = SAMPLE_IVE_DualPicCalc_TEST(u32Mode, as32Type, pchPathSrc, pchPathDst, u32WidthSrc, u32HeightSrc, achParams);
    }
    break;
    case 7:
    case 8: {//HysEdge and CannyEdge
        AX_CHAR *pchPathSrc[2] = {"/opt/data/ive/common/1280x720_u8c1_gray.yuv", "/opt/data/ive/common/1280x720_u16c1_gray_2.yuv"};
        AX_CHAR *pchPathDst = "/tmp/edge_test_out.yuv";
        AX_U32 u32Mode = u32CaseIndex - 7;
        AX_U16 u16LowThr;
        AX_U16 u16HighThr;
        do {
            u16LowThr = GetRandInRange(0, 2047);
            u16HighThr = GetRandInRange(u16LowThr, 2047);
        } while (u16LowThr == u16HighThr);
        AX_U8 u8Thr = GetRandInRange(1, 255);
        SAMPLE_IVE_PRT("---Edge:%u--- thr_h:%u, thr_l:%u, thr:%u\n", u32Mode, u16HighThr, u16LowThr, u8Thr);
        AX_CHAR achParams[512] = {0};
        snprintf(achParams, 511, "{\"thr_h\":%u,\"thr_l\":%u,\"thr\":%u}", u16HighThr, u16LowThr, u8Thr);
        s32Ret = SAMPLE_IVE_EdgeDetection_TEST(u32Mode, as32Type, pchPathSrc, pchPathDst, u32WidthSrc, u32HeightSrc, achParams);
    }
    break;
    case 9: {//CCL
        AX_CHAR *pchPathSrc = "/opt/data/ive/ccl/ccl_input_1280x720_u8c1_bin.yuv";
        AX_CHAR *pchPathDst[2] = {"/tmp/ccl_test_out1.yuv", "/tmp/"};
        AX_U32 u32Mode = GetRandInRange(0, 1);
        SAMPLE_IVE_PRT("---CCL--- mode:%u\n", u32Mode);
        AX_CHAR achParams[512] = {0};
        snprintf(achParams, 511, "{\"mode\":%u}", u32Mode);
        s32Ret = SAMPLE_IVE_CCL_TEST(as32Type, pchPathSrc, pchPathDst, u32WidthSrc, u32HeightSrc, achParams);
    }
    break;
    case 10:
    case 11: {//Erode and Dilate
        AX_CHAR *pchPathSrc = "/opt/data/ive/common/1280x720_u8c1_bin.yuv";
        AX_CHAR *pchPathDst = "/tmp/erode_dilate_test_out.yuv";
        AX_U32 u32Mode = u32CaseIndex - 10;
        AX_U8 au8Mask[25] = {0};
        for (AX_S32 i = 0; i < 25; i++) {
            if (1 == (rand() % 2) )
                au8Mask[i] = 255;
        }
        SAMPLE_IVE_PRT("---ED:%u--- mask:{%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d}\n",
        u32Mode, au8Mask[0], au8Mask[1], au8Mask[2], au8Mask[3], au8Mask[4], au8Mask[5], au8Mask[6], au8Mask[7], au8Mask[8], au8Mask[9],
        au8Mask[10], au8Mask[11], au8Mask[12], au8Mask[13], au8Mask[14], au8Mask[15], au8Mask[16], au8Mask[17], au8Mask[18], au8Mask[19],
        au8Mask[20], au8Mask[21], au8Mask[22], au8Mask[23], au8Mask[24]);
        AX_CHAR achParams[512] = {0};
        snprintf(achParams, 511, "{\"mask\":[%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d]}",
        au8Mask[0], au8Mask[1], au8Mask[2], au8Mask[3], au8Mask[4], au8Mask[5], au8Mask[6], au8Mask[7], au8Mask[8], au8Mask[9],
        au8Mask[10], au8Mask[11], au8Mask[12], au8Mask[13], au8Mask[14], au8Mask[15], au8Mask[16], au8Mask[17], au8Mask[18], au8Mask[19],
        au8Mask[20], au8Mask[21], au8Mask[22], au8Mask[23], au8Mask[24]);
        s32Ret = SAMPLE_IVE_ED_TEST(u32Mode, as32Type, pchPathSrc, pchPathDst, u32WidthSrc, u32HeightSrc, achParams);
    }
    break;
    case 12: {//Filter
        //U8C1 (YUV420 YUV422 todo)
        AX_CHAR *pchPathSrc = "/opt/data/ive/common/1280x720_u8c1_gray.yuv";
        AX_CHAR *pchPathDst = "/tmp/filter_test_out.yuv";
        AX_S16 as16Mask[25] = {0};
        for (AX_S32 i = 0; i < 25; i++) {
            as16Mask[i] = GetRandInRange(-65535, 65535);
        }
        SAMPLE_IVE_PRT("---Filter--- mask:{%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d}\n",
        as16Mask[0], as16Mask[1], as16Mask[2], as16Mask[3], as16Mask[4], as16Mask[5], as16Mask[6], as16Mask[7], as16Mask[8], as16Mask[9],
        as16Mask[10], as16Mask[11], as16Mask[12], as16Mask[13], as16Mask[14], as16Mask[15], as16Mask[16], as16Mask[17], as16Mask[18], as16Mask[19],
        as16Mask[20], as16Mask[21], as16Mask[22], as16Mask[23], as16Mask[24]);
        AX_CHAR achParams[512] = {0};
        snprintf(achParams, 511, "{\"mask\":[%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d]}",
        as16Mask[0], as16Mask[1], as16Mask[2], as16Mask[3], as16Mask[4], as16Mask[5], as16Mask[6], as16Mask[7], as16Mask[8], as16Mask[9],
        as16Mask[10], as16Mask[11], as16Mask[12], as16Mask[13], as16Mask[14], as16Mask[15], as16Mask[16], as16Mask[17], as16Mask[18], as16Mask[19],
        as16Mask[20], as16Mask[21], as16Mask[22], as16Mask[23], as16Mask[24]);
        s32Ret = SAMPLE_IVE_Filter_TEST(as32Type, pchPathSrc, pchPathDst, u32WidthSrc, u32HeightSrc, achParams);
    }
    break;
    case 13:
    case 14: {//Hist and EqualHist
        AX_CHAR *pchPathSrc = "/opt/data/ive/common/1280x720_u8c1_gray.yuv";
        AX_CHAR *pchPathDst = "/tmp/hist_test_out.yuv";
        AX_U32 u32Mode = u32CaseIndex - 13;
        AX_U32 u32HistEqualCoef = GetRandInRange(0, 1048575);
        SAMPLE_IVE_PRT("---Hist:%u--- histeq_coef:%u\n", u32Mode, u32HistEqualCoef);
        AX_CHAR achParams[512] = {0};
        snprintf(achParams, 511, "{\"histeq_coef\":%u}", u32HistEqualCoef);
        s32Ret = SAMPLE_IVE_Hist_TEST(u32Mode, as32Type, pchPathSrc, pchPathDst, u32WidthSrc, u32HeightSrc, achParams);
    }
    break;
    case 15: {//Integ
        AX_CHAR *pchPathSrc = "/opt/data/ive/common/1280x720_u8c1_gray.yuv";
        AX_CHAR *pchPathDst = "/tmp/integ_test_out.yuv";
        AX_U32 u32OutCtl = GetRandInRange(0, 2);
        SAMPLE_IVE_PRT("---Integ--- out_ctl:%u\n", u32OutCtl);
        AX_CHAR achParams[512] = {0};
        snprintf(achParams, 511, "{\"out_ctl\":%u}", u32OutCtl);
        as32Type[0] = AX_IVE_IMAGE_TYPE_U8C1;
        if (u32OutCtl == AX_IVE_INTEG_OUT_CTRL_SUM)
            as32Type[1] = AX_IVE_IMAGE_TYPE_U32C1;
        else
            as32Type[1] = AX_IVE_IMAGE_TYPE_U64C1;
        s32Ret = SAMPLE_IVE_Integ_TEST(as32Type, pchPathSrc, pchPathDst, u32WidthSrc, u32HeightSrc, achParams);
    }
    break;
    case 16: {//MagAng
        AX_CHAR *pchPathSrc[2] = {"/opt/data/ive/common/1280x720_u16c1_gray.yuv", "/opt/data/ive/common/1280x720_u16c1_gray_2.yuv"};
        AX_CHAR *pchPathDst[2] = {"/tmp/mag_output.bin", "/tmp/ang_output.bin"};
        SAMPLE_IVE_PRT("---MagAndAng---\n");
        s32Ret = SAMPLE_IVE_MagAng_TEST(as32Type, pchPathSrc, pchPathDst, u32WidthSrc, u32HeightSrc);
    }
    break;
    case 17: {//Sobel
        AX_CHAR *pchPathSrc = "/opt/data/ive/common/1280x720_u8c1_gray.yuv";
        AX_CHAR *pchPathDst = "/tmp/sobel_test_out.yuv";
        AX_S16 as16Mask[25] = {0};
        for (AX_S32 i = 0; i < 25; i++) {
            as16Mask[i] = GetRandInRange(-65535, 65535);
        }
        SAMPLE_IVE_PRT("---Sobel--- mask:{%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d}\n",
        as16Mask[0], as16Mask[1], as16Mask[2], as16Mask[3], as16Mask[4], as16Mask[5], as16Mask[6], as16Mask[7], as16Mask[8], as16Mask[9],
        as16Mask[10], as16Mask[11], as16Mask[12], as16Mask[13], as16Mask[14], as16Mask[15], as16Mask[16], as16Mask[17], as16Mask[18], as16Mask[19],
        as16Mask[20], as16Mask[21], as16Mask[22], as16Mask[23], as16Mask[24]);
        AX_CHAR achParams[512] = {0};
        snprintf(achParams, 511, "{\"mask\":[%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d]}",
        as16Mask[0], as16Mask[1], as16Mask[2], as16Mask[3], as16Mask[4], as16Mask[5], as16Mask[6], as16Mask[7], as16Mask[8], as16Mask[9],
        as16Mask[10], as16Mask[11], as16Mask[12], as16Mask[13], as16Mask[14], as16Mask[15], as16Mask[16], as16Mask[17], as16Mask[18], as16Mask[19],
        as16Mask[20], as16Mask[21], as16Mask[22], as16Mask[23], as16Mask[24]);
        s32Ret = SAMPLE_IVE_Sobel_TEST(as32Type, pchPathSrc, pchPathDst, u32WidthSrc, u32HeightSrc, achParams);
    }
    break;
    case 18:
    case 19: {//GMM and GMM2
        //U8C1 (RGB todo)
        AX_CHAR *pchPathSrc[2] = {"/opt/data/ive/common/1280x720_u8c1_gray.yuv", "/opt/data/ive/gmm/gmm_gray_1280x720_model.bin"};
        AX_CHAR *pchPathDst[3] = {"/tmp/gmm_output_fg.bin", "/tmp/gmm_output_bg.bin", "/tmp/gmm_output_model.bin"};
        AX_U32 u32Mode = u32CaseIndex - 18;
        AX_U32 u32InitVar = GetRandInRange(0, 262143);
        AX_U32 u32MinVar = GetRandInRange(0, 262143);
        AX_U16 u16InitWeight = GetRandInRange(0, 1024);
        AX_U8 u8LearnRate = GetRandInRange(0, 128);
        AX_U8 u8BgRatio = GetRandInRange(0, 128);
        AX_U8 u8VarThr = GetRandInRange(0, 255);
        AX_U8 u8Thr = GetRandInRange(1, 255);

        AX_U32 u32MaxVar = GetRandInRange(0, 262143);
        AX_U8 u8VarThrCheck = GetRandInRange(0, 255);
        AX_S16 s16Ct = GetRandInRange(-255, 255);
        AX_CHAR achParams[512] = {0};
        if (0 == u32Mode) {
            SAMPLE_IVE_PRT("---GMM--- init_var:%u, min_var:%u, init_w:%u, lr:%u, bg_r:%u, var_thr:%u, thr:%u\n",
            u32InitVar, u32MinVar, u16InitWeight, u8LearnRate, u8BgRatio, u8VarThr, u8Thr);
            snprintf(achParams, 511, "{\"init_var\":%u,\"min_var\":%u,\"init_w\":%u,\"lr\":%u,\"bg_r\":%u,\"var_thr\":%u,\"thr\":%u}",
            u32InitVar, u32MinVar, u16InitWeight, u8LearnRate, u8BgRatio, u8VarThr, u8Thr);
        } else {
            SAMPLE_IVE_PRT("---GMM2--- init_var:%u, min_var:%u, max_var:%u, lr:%u, bg_r:%u, var_thr:%u, var_thr_chk:%u, ct:%d, thr:%u\n",
            u32InitVar, u32MinVar, u32MaxVar, u8LearnRate, u8BgRatio, u8VarThr, u8VarThrCheck, s16Ct, u8Thr);
            snprintf(achParams, 511, "{\"init_var\":%u,\"min_var\":%u,\"max_var\":%u,\"lr\":%u,\"bg_r\":%u,\"var_thr\":%u,\"var_thr_chk\":%u,\"ct\":%d,\"thr\":%u}",
            u32InitVar, u32MinVar, u32MaxVar, u8LearnRate, u8BgRatio, u8VarThr, u8VarThrCheck, s16Ct, u8Thr);
        }
        s32Ret = SAMPLE_IVE_GMM_TEST(u32Mode, as32Type, pchPathSrc, pchPathDst, u32WidthSrc, u32HeightSrc, achParams);
    }
    break;
    case 20: {//Thresh
        AX_CHAR *pchPathSrc = "/opt/data/ive/common/1280x720_u8c1_gray.yuv";
        AX_CHAR *pchPathDst = "/tmp/thresh_test_out.yuv";
        AX_U32 u32Mode = GetRandInRange(0, 7);
        AX_U8 u8LowThr;
        AX_U8 u8HightThr;
        AX_U8 u8MinVal;
        AX_U8 u8MidVal;
        AX_U8 u8MaxVal;
        do {
            u8LowThr = GetRandInRange(0, 255);
            u8HightThr = GetRandInRange(u8LowThr, 255);
        } while (u8LowThr >= u8HightThr);
        do {
            u8MinVal = GetRandInRange(0, 255);
            u8MidVal = GetRandInRange(u8MinVal, 255);
            u8MaxVal = GetRandInRange(u8MidVal, 255);
        } while (u8MinVal >= u8MidVal || u8MidVal >= u8MaxVal);
        SAMPLE_IVE_PRT("---Thresh--- mode:%u, thr_l:%u, thr_h:%u, min_val:%u, mid_val:%u, max_val:%u\n",
        u32Mode, u8LowThr, u8HightThr, u8MinVal, u8MidVal, u8MaxVal);
        AX_CHAR achParams[512] = {0};
        snprintf(achParams, 511, "{\"mode\":%u,\"thr_l\":%u,\"thr_h\":%u,\"min_val\":%u,\"mid_val\":%u,\"max_val\":%u}",
        u32Mode, u8LowThr, u8HightThr, u8MinVal, u8MidVal, u8MaxVal);
        s32Ret = SAMPLE_IVE_Thresh_TEST(as32Type, pchPathSrc, pchPathDst, u32WidthSrc, u32HeightSrc, achParams);
    }
    break;
    case 21: {//16bit to 8bit
        AX_CHAR *pchPathSrc = "/opt/data/ive/common/1280x720_u16c1_gray.yuv";
        AX_CHAR *pchPathDst = "/tmp/16bit_8bit_test_out.yuv";
        AX_U32 u32Mode = GetRandInRange(0, 3);
        AX_S16 s16Gain = GetRandInRange(-16383, 16383);
        AX_S16 s16Bias = GetRandInRange(-16384, 16383);
        SAMPLE_IVE_PRT("---16To8Bit--- mode:%u, gain:%d, bias:%d\n", u32Mode, s16Gain, s16Bias);
        AX_CHAR achParams[512] = {0};
        snprintf(achParams, 511, "{\"mode\":%u,\"gain\":%d,\"bias\":%d}", u32Mode, s16Gain, s16Bias);
        if (u32Mode == AX_IVE_16BIT_TO_8BIT_MODE_U16_TO_U8)
            as32Type[0] = AX_IVE_IMAGE_TYPE_U16C1;
        else
            as32Type[0] = AX_IVE_IMAGE_TYPE_S16C1;
        if (u32Mode == AX_IVE_16BIT_TO_8BIT_MODE_S16_TO_S8)
            as32Type[1] = AX_IVE_IMAGE_TYPE_S8C1;
        else
            as32Type[1] = AX_IVE_IMAGE_TYPE_U8C1;
        s32Ret = SAMPLE_IVE_16To8Bit_TEST(as32Type, pchPathSrc, pchPathDst, u32WidthSrc, u32HeightSrc, achParams);
    }
    break;
    case 22:
    case 23:
    case 24: {//CropResize
        AX_CHAR *pchPathSrcGray = "/opt/data/ive/common/1280x720_u8c1_gray.yuv";
        AX_CHAR *pchPathSrcNV12 = "/opt/data/ive/common/1280x720_nv12.yuv";
        AX_CHAR *pchPathDst = "/tmp/";
        AX_U32 u32Mode = GetRandInRange(0, 2);//u32CaseIndex - 22;
        AX_U32 u32EngineId;
        if (u32Mode == 0) {
            u32EngineId = GetRandInRange(0, 3);
        } else {
            u32EngineId = GetRandInRange(1, 3);
        }
        AX_U32 u32WidthDst = u32WidthSrc;
        AX_U32 u32HeightDst = u32HeightSrc;
        AX_U32 u32WidthCrop;
        AX_U32 u32HeightCrop;
        AX_BOOL bBoxVaild = AX_TRUE;
        if (u32Mode != 0) {
            do {
                u32WidthCrop = GetRandInRange(32, u32WidthSrc);
                u32HeightCrop = GetRandInRange(32, u32HeightSrc);
                u32WidthCrop = ALIGN_DOWN(u32WidthCrop, 16);
                u32HeightCrop = ALIGN_DOWN(u32HeightCrop, 2);
                if (1 == (rand() % 2)) {
                    u32WidthDst = GetRandInRange(u32WidthCrop, u32WidthSrc);
                    u32HeightDst = GetRandInRange(u32HeightCrop, u32HeightSrc);
                    u32WidthDst = ALIGN_DOWN(u32WidthDst, 16);
                    u32HeightDst = ALIGN_DOWN(u32HeightDst, 2);
                    if((u32WidthDst / u32WidthCrop) >= 16 || (u32WidthDst / u32WidthCrop) < 1
                     || (u32HeightDst / u32HeightCrop) >= 16 || (u32HeightDst / u32HeightCrop) < 1)
                        bBoxVaild = AX_FALSE;
                    else
                        bBoxVaild = AX_TRUE;
                } else {
                    u32WidthDst = GetRandInRange(32, u32WidthCrop);
                    u32HeightDst = GetRandInRange(32, u32HeightCrop);
                    u32WidthDst = ALIGN_DOWN(u32WidthDst, 16);
                    u32HeightDst = ALIGN_DOWN(u32HeightDst, 2);
                    if((u32WidthCrop / u32WidthDst) >= 32 || (u32WidthCrop / u32WidthDst) < 1
                    || (u32HeightCrop / u32HeightDst) >= 32 || (u32HeightCrop / u32HeightDst) < 1)
                        bBoxVaild = AX_FALSE;
                    else
                        bBoxVaild = AX_TRUE;
                }
            } while (!bBoxVaild);
        } else {
            u32WidthCrop = GetRandInRange(32, u32WidthSrc);
            u32WidthCrop = ALIGN_DOWN(u32WidthCrop, 16);

            if (u32EngineId != 0)
                u32HeightCrop = GetRandInRange(32, u32HeightSrc);
            else
                u32HeightCrop = GetRandInRange(32, u32HeightSrc);
            u32HeightCrop = ALIGN_UP(u32HeightCrop, 2);
        }
        SAMPLE_IVE_PRT("---CropResize:m[%u] e[%u]--- w_out:%u, h_out:%u, w:%u, h:%u\n", u32Mode, u32EngineId, u32WidthDst, u32HeightDst, u32WidthCrop, u32HeightCrop);
        AX_CHAR achParams[512] = {0};
        snprintf(achParams, 511, "{\"num\":1,\"align0\":0,\"align1\": 0,\"bcolor\":255,\"w_out\":%u,\"h_out\":%u,\"boxs\":[{\"x\":0,\"y\":0,\"w\":%u,\"h\":%u}]}",
            u32WidthDst, u32HeightDst, u32WidthCrop, u32HeightCrop);
        if (u32Mode == 0 && u32EngineId == 0) {
            if (GetRandInRange(0, 1)) {
                as32Type[0] = AX_IVE_IMAGE_TYPE_YUV420SP;
                as32Type[1] = AX_IVE_IMAGE_TYPE_YUV420SP;
                s32Ret = SAMPLE_IVE_CropResize_TEST(u32EngineId, u32Mode, as32Type, pchPathSrcNV12, pchPathDst, u32WidthSrc, u32HeightSrc, achParams);
            } else {
                as32Type[0] = AX_IVE_IMAGE_TYPE_U8C1;
                as32Type[1] = AX_IVE_IMAGE_TYPE_U8C1;
                s32Ret = SAMPLE_IVE_CropResize_TEST(u32EngineId, u32Mode, as32Type, pchPathSrcGray, pchPathDst, u32WidthSrc, u32HeightSrc, achParams);
            }
        } else {
            as32Type[0] = AX_FORMAT_YUV420_SEMIPLANAR;
            as32Type[1] = AX_FORMAT_YUV420_SEMIPLANAR;
            s32Ret = SAMPLE_IVE_CropResize_TEST(u32EngineId, u32Mode, as32Type, pchPathSrcNV12, pchPathDst, u32WidthSrc, u32HeightSrc, achParams);
        }
    }
    break;
    case 25: {//CSC
        AX_CHAR *pchPathSrc = "/opt/data/ive/common/1280x720_nv12.yuv";
        AX_CHAR *pchPathDst = "/tmp/csc_output_1280x720.rgb888";
        AX_U32 u32EngineId = GetRandInRange(1, 3);
        as32Type[0] = AX_FORMAT_YUV420_SEMIPLANAR;
        as32Type[1] = AX_FORMAT_RGB888;
        SAMPLE_IVE_PRT("---CSC:e[%u]--- Type %d to %d\n", u32EngineId, as32Type[0], as32Type[1]);
        s32Ret = SAMPLE_IVE_CSC_TEST(u32EngineId, as32Type, pchPathSrc, pchPathDst, u32WidthSrc, u32HeightSrc);
    }
    break;
    case 26:
    case 27: {//CropResize2
        AX_CHAR *pchPathSrc[2] = {"/opt/data/ive/common/1280x720_nv12.yuv", "/opt/data/ive/crop_resize2/bg_1270x720_nv12.yuv"};
        AX_CHAR *pchPathDst = "/tmp/";
        AX_U32 u32Mode = u32CaseIndex - 26;
        AX_U32 u32EngineId = GetRandInRange(1, 3);
        AX_U32 u32WidthDst = u32WidthSrc;
        AX_U32 u32HeightDst = u32HeightSrc;
        AX_U32 u32XCrop = 0, u32YCrop = 0;
        AX_U32 u32WidthCrop = 0, u32HeightCrop = 0;
        AX_U32 u32XResize = 0, u32YResize = 0;
        AX_U32 u32WidthResize = 0, u32HeightResize = 0;
        AX_BOOL bBoxVaild = AX_TRUE;
        do {
            u32WidthCrop = GetRandInRange(32, u32WidthSrc);
            u32HeightCrop = GetRandInRange(32, u32HeightSrc);
            u32WidthCrop = ALIGN_DOWN(u32WidthCrop, 16);
            u32HeightCrop = ALIGN_DOWN(u32HeightCrop, 2);
            if (1 == (rand() % 2)) {
                u32WidthResize = GetRandInRange(u32WidthCrop, u32WidthDst);
                u32HeightResize = GetRandInRange(u32HeightCrop, u32HeightDst);
                u32WidthResize = ALIGN_DOWN(u32WidthResize, 16);
                u32HeightResize = ALIGN_DOWN(u32HeightResize, 2);
                if((u32WidthResize / u32WidthCrop) >= 16 || (u32WidthResize / u32WidthCrop) < 1
                    || (u32HeightResize / u32HeightCrop) >= 16 || (u32HeightResize / u32HeightCrop) < 1)
                    bBoxVaild = AX_FALSE;
                else
                    bBoxVaild = AX_TRUE;
                if (!bBoxVaild)
                    continue;
            } else {
                u32WidthResize = GetRandInRange(32, u32WidthCrop);
                u32HeightResize = GetRandInRange(32, u32HeightCrop);
                u32WidthResize = ALIGN_DOWN(u32WidthResize, 16);
                u32HeightResize = ALIGN_DOWN(u32HeightResize, 2);
                if((u32WidthCrop / u32WidthResize) >= 32 || (u32WidthCrop / u32WidthResize) < 1
                || (u32HeightCrop / u32HeightResize) >= 32 || (u32HeightCrop / u32HeightResize) < 1)
                    bBoxVaild = AX_FALSE;
                else
                    bBoxVaild = AX_TRUE;
                if (!bBoxVaild)
                    continue;
            }
            u32XCrop = GetRandInRange(0, u32WidthSrc - 1);
            u32YCrop = GetRandInRange(0, u32HeightSrc - 1);
            u32XCrop = ALIGN_DOWN(u32XCrop, 2);
            u32YCrop = ALIGN_DOWN(u32YCrop, 2);
            if (u32WidthSrc < (u32XCrop + u32WidthCrop) || u32HeightSrc < (u32YCrop + u32HeightCrop))
                bBoxVaild = AX_FALSE;
             else
                bBoxVaild = AX_TRUE;
            if (!bBoxVaild)
                continue;

            u32XResize = GetRandInRange(0, u32WidthDst - 1);
            u32YResize = GetRandInRange(0, u32HeightDst - 1);
            if (u32WidthDst < (u32XResize + u32WidthResize) || u32HeightDst < (u32YResize + u32HeightResize))
                bBoxVaild = AX_FALSE;
             else
                bBoxVaild = AX_TRUE;
            if (!bBoxVaild)
                continue;
        } while (!bBoxVaild);

        SAMPLE_IVE_PRT("---CropResize2:m[%u] e[%u]--- res_out:[%ux%u], src_box:[%u, %u, %u, %u], dst_boxs:[%u, %u, %u, %u]\n",
        u32Mode, u32EngineId, u32WidthDst, u32HeightDst, u32XCrop, u32YCrop, u32WidthCrop, u32HeightCrop, u32XResize, u32YResize, u32WidthResize, u32HeightResize);
        AX_CHAR achParams[512] = {0};
        snprintf(achParams, 511, "{\"num\":1,\"res_out\":[{\"w\":%u,\"h\":%u}],\"src_boxs\":[{\"x\":%u,\"y\":%u,\"w\":%u,\"h\":%u}],\"dst_boxs\":[{\"x\":%u,\"y\":%u,\"w\":%u,\"h\":%u}]}",
            u32WidthDst, u32HeightDst, u32XCrop, u32YCrop, u32WidthCrop, u32HeightCrop, u32XResize, u32YResize, u32WidthResize, u32HeightResize);

        as32Type[0] = AX_FORMAT_YUV420_SEMIPLANAR;
        as32Type[1] = AX_FORMAT_YUV420_SEMIPLANAR;
        s32Ret = SAMPLE_IVE_CropResize2_TEST(u32EngineId, u32Mode, as32Type, pchPathSrc, pchPathDst, u32WidthSrc, u32HeightSrc, achParams);
    }
    break;
    default :
        SAMPLE_IVE_PRT("No support the testing case[%d]!\n", u32CaseIndex);
    break;
    }

    return s32Ret;
}

static AX_VOID *SAMPLE_IVE_TestUnorderedCases_Thread(AX_VOID *pArg)
{
    AX_S32 s32Ret;
    AX_U32 u32CaseIndex = 0;
    axclrtContext context;
    AX_U32 deviceId = *(AX_U32 *)pArg;

    s32Ret = axclrtCreateContext(&context, deviceId);
    if (AXCL_SUCC != s32Ret) {
        SAMPLE_IVE_PRT("axcl create cxt err, ret=0x%x.\n", s32Ret);
        return NULL;
    }

    while (bStartThread) {
        pthread_mutex_lock(&g_lock);
        u32CaseIndex = GetRandInRange(0, SAMPLE_IVE_MAX_OP_NUM - 1);
        SAMPLE_IVE_PRT("\n###### Start case %u ######\n", u32CaseIndex);
        s32Ret = SAMPLE_IVE_StartOneCase(u32CaseIndex);
        if (s32Ret != AX_SUCCESS) {
            SAMPLE_IVE_PRT("Case %u run failed!\n", u32CaseIndex);
            g_bCaseTestSuccess = AX_FALSE;
            pthread_mutex_unlock(&g_lock);
            axclrtDestroyContext(context);
            return NULL;
        }
        SAMPLE_IVE_PRT("\n###### Case %u run success ######\n", u32CaseIndex);
        pthread_mutex_unlock(&g_lock);
    }

    axclrtDestroyContext(context);
    return NULL;
}

AX_S32 SAMPLE_IVE_TestCases_ThreadStart(AX_U32 u32ThreadNum, AX_U32 *pu32DevId)
{
    if (u32ThreadNum > SAMPLE_IVE_MAX_THREAD_NUM) {
        SAMPLE_IVE_PRT("Error: support max thread num is %d!\n", SAMPLE_IVE_MAX_THREAD_NUM);
        return AX_FAILURE;
    }
    srand((AX_U32)time(NULL));
    bStartThread = AX_TRUE;
    g_bCaseTestSuccess = AX_TRUE;
    pthread_mutex_init(&g_lock, NULL);
    for (AX_S32 i = 0; i < u32ThreadNum; i++) {
        if (0 != pthread_create(&g_tid[i], NULL, SAMPLE_IVE_TestUnorderedCases_Thread, pu32DevId)) {
            SAMPLE_IVE_PRT("Create pthread %d failed!\n", i);
            return AX_FAILURE;
        }
    }

    return AX_SUCCESS;
}

AX_VOID SAMPLE_IVE_TestCases_ThreadStop(AX_U32 u32ThreadNum)
{
    bStartThread = AX_FALSE;
    for (AX_S32 i = 0; i < u32ThreadNum; i++) {
        if (g_tid[i] > 0) {
            pthread_join(g_tid[i], NULL);
            g_tid[i] = 0;
        }
    }
    pthread_mutex_destroy(&g_lock);
}

static AX_BOOL bStartProcess = AX_FALSE;

static pid_t g_pid[SAMPLE_IVE_MAX_PROCESS_NUM] = {0};

static void process_handle_signal(int signo)
{
    if (signo == SIGINT) {
        SAMPLE_IVE_PRT("\n[%d]: test case exit!\n", getpid());
        bStartProcess = AX_FALSE;
    } else if(signo == SIGUSR1) {
        SAMPLE_IVE_PRT("\n[%d]: test case failed!\n", getpid());
        g_bCaseTestSuccess = AX_FALSE;
    }
}

static AX_VOID *SAMPLE_IVE_TestUnorderedCases_Process(AX_VOID *pArg)
{
    AX_S32 s32Ret;
    AX_U32 u32CaseIndex = 1;
    axclrtContext context;
    AX_U32 deviceId = *(AX_U32 *)pArg;

    signal(SIGINT, process_handle_signal);
    srand((AX_U32)time(NULL));

    s32Ret = axclrtCreateContext(&context, deviceId);
    if (AXCL_SUCC != s32Ret) {
        SAMPLE_IVE_PRT("axcl create cxt err, ret=0x%x.\n", s32Ret);
        return NULL;
    }

    if (AXCL_SYS_Init() < 0) {
        SAMPLE_IVE_PRT("[%d]: Sys init failed!\n",getpid());
        goto FAIL1;
    }

    if ((s32Ret = AXCL_IVE_Init()) < 0) {
        SAMPLE_IVE_PRT("[%d]: Ive init failed, s32Ret=0x%x!\n", getpid(), s32Ret);
        goto FAIL2;
    }

    while (bStartProcess) {
        /* At present, it is found that an error occurs in the interface of
        multi process calling ivps, and the problem has not been identified ivps ret=0x800d0121*/
        u32CaseIndex = rand() % 21;
        //u32CaseIndex = rand() % SAMPLE_IVE_MAX_OP_NUM;
        SAMPLE_IVE_PRT("\n[%d]: ###### Start case %u ######\n", getpid(), u32CaseIndex);
        s32Ret = SAMPLE_IVE_StartOneCase(u32CaseIndex);
        if (s32Ret != AX_SUCCESS) {
            SAMPLE_IVE_PRT("\n[%d]: ###### Case %u run failed ######\n", getpid(), u32CaseIndex);
            goto  FAIL3;
        }
        SAMPLE_IVE_PRT("\n[%d]: ###### Case %u run success ######\n", getpid(), u32CaseIndex);
        /* After the delay increases, the ivps interface will not report errors */
        usleep(1000);
    }

    AXCL_IVE_Exit();
    AXCL_SYS_Deinit();
    axclrtDestroyContext(context);
    SAMPLE_IVE_PRT("\n###### Process id %d exit ######\n", getpid());
    return NULL;

FAIL3:
    AXCL_IVE_Exit();
FAIL2:
    AXCL_SYS_Deinit();
FAIL1:
    kill(getppid(),SIGUSR1);
    axclrtDestroyContext(context);
    SAMPLE_IVE_PRT("\n###### Process id %d exit ######\n", getpid());
    return NULL;
}

AX_S32 SAMPLE_IVE_TestCases_Process(AX_U32 u32Duration, AX_U32 u32ProcessNum)
{
    if (u32ProcessNum > SAMPLE_IVE_MAX_PROCESS_NUM) {
        SAMPLE_IVE_PRT("Error: support max process num is %d!\n", SAMPLE_IVE_MAX_PROCESS_NUM);
        return AX_FAILURE;
    }

    AX_S32 i = 0;
    bStartProcess = AX_TRUE;
    g_bCaseTestSuccess = AX_TRUE;

    for (i = 0; i < u32ProcessNum; i++) {
        fflush(NULL);
        g_pid[i] = fork();
        if(g_pid[i] < 0) {
            SAMPLE_IVE_PRT("Error: Create process failed!\n");
            for(AX_S32 j = 0 ; j < i ; j++){
                kill(g_pid[j],SIGINT);
                wait(NULL);
            }
            return AX_FAILURE;
        } else if(g_pid[i] == 0) {
            break;
        } else {
        }
    }
    if(i < u32ProcessNum) {
        SAMPLE_IVE_PRT("\n###### Run process, id %d ######\n", getpid());
        SAMPLE_IVE_TestUnorderedCases_Process(NULL);
    } else if(i == u32ProcessNum) {
        signal(SIGUSR1, process_handle_signal);
        for (AX_S32 i = 0; i < u32Duration; i++) {
            if (!g_bCaseTestSuccess)
                break;
            usleep(1000 * 1000);
        }
        for (AX_S32 i = 0; i < u32ProcessNum; i++) {
            kill(g_pid[i],SIGINT);
            wait(NULL);
        }
    }
    return AX_SUCCESS;
}