/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include <assert.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#include "ax_venc_comm.h"
#include "unity.h"

void setUp(void)
{

}

void tearDown(void)
{

}

static AX_S32 SampleVencAttrInit(AX_VENC_CHN_ATTR_T *pstChn, AX_PAYLOAD_TYPE_E enType)
{
    memset(pstChn, 0, sizeof(AX_VENC_CHN_ATTR_T));

    pstChn->stVencAttr.enType = enType;
    pstChn->stVencAttr.enLinkMode = AX_VENC_UNLINK_MODE;
    pstChn->stVencAttr.enProfile = (PT_H264 == enType) ? AX_VENC_H264_MAIN_PROFILE:AX_VENC_HEVC_MAIN_PROFILE;
    pstChn->stVencAttr.enTier = AX_VENC_HEVC_MAIN_TIER;
    pstChn->stVencAttr.enLevel = (PT_H264 == enType) ? AX_VENC_H264_LEVEL_5_1 : AX_VENC_HEVC_LEVEL_5_1;
    pstChn->stVencAttr.enStrmBitDepth = AX_VENC_STREAM_BIT_8;
    pstChn->stVencAttr.stCropCfg.bEnable = AX_FALSE;
    pstChn->stVencAttr.u32BufSize = 0;
    pstChn->stVencAttr.u32MaxPicHeight = MAX_VENC_PIC_HEIGHT;
    pstChn->stVencAttr.u32MaxPicWidth = MAX_VENC_PIC_WIDTH;
    pstChn->stVencAttr.u32PicHeightSrc = MAX_VENC_PIC_HEIGHT;
    pstChn->stVencAttr.u32PicWidthSrc = MAX_VENC_PIC_WIDTH;
    pstChn->stVencAttr.u8InFifoDepth = 2;
    pstChn->stVencAttr.u8OutFifoDepth = 2;

    /* rc attr */
    if (PT_H264 == enType) {
        pstChn->stRcAttr.enRcMode = AX_VENC_RC_MODE_H264CBR;
        pstChn->stRcAttr.s32FirstFrameStartQp = -1;
        pstChn->stRcAttr.stFrameRate.fSrcFrameRate = 30.0;
        pstChn->stRcAttr.stFrameRate.fDstFrameRate = 30.0;
        pstChn->stRcAttr.stH264Cbr.s32IntraQpDelta = 0;
        pstChn->stRcAttr.stH264Cbr.u32BitRate = VENC_MAX_BITRATE;
        pstChn->stRcAttr.stH264Cbr.u32Gop = 30;
        pstChn->stRcAttr.stH264Cbr.u32IdrQpDeltaRange = 0;
        pstChn->stRcAttr.stH264Cbr.u32MaxIprop = VENC_MAX_I_PROP;
        pstChn->stRcAttr.stH264Cbr.u32MinIprop = VENC_MIN_I_PROP;
        pstChn->stRcAttr.stH264Cbr.u32MaxIQp = VENC_MAX_QP;
        pstChn->stRcAttr.stH264Cbr.u32MinIQp = VENC_MIN_QP;
        pstChn->stRcAttr.stH264Cbr.u32MinQp = VENC_MIN_QP;
        pstChn->stRcAttr.stH264Cbr.u32MaxQp = VENC_MAX_QP;
        pstChn->stRcAttr.stH264Cbr.u32StatTime = 0;
    } else if (PT_H265 == enType) {
        pstChn->stRcAttr.enRcMode = AX_VENC_RC_MODE_H265CBR;
        pstChn->stRcAttr.s32FirstFrameStartQp = -1;
        pstChn->stRcAttr.stFrameRate.fSrcFrameRate = 30.0;
        pstChn->stRcAttr.stFrameRate.fDstFrameRate = 30.0;
        pstChn->stRcAttr.stH265Cbr.s32IntraQpDelta = 0;
        pstChn->stRcAttr.stH265Cbr.u32BitRate = VENC_MAX_BITRATE;
        pstChn->stRcAttr.stH265Cbr.u32Gop = 30;
        pstChn->stRcAttr.stH265Cbr.u32IdrQpDeltaRange = 0;
        pstChn->stRcAttr.stH265Cbr.u32MaxIprop = VENC_MAX_I_PROP;
        pstChn->stRcAttr.stH265Cbr.u32MinIprop = VENC_MIN_I_PROP;
        pstChn->stRcAttr.stH265Cbr.u32MaxIQp = VENC_MAX_QP;
        pstChn->stRcAttr.stH265Cbr.u32MinIQp = VENC_MIN_QP;
        pstChn->stRcAttr.stH265Cbr.u32MinQp = VENC_MIN_QP;
        pstChn->stRcAttr.stH265Cbr.u32MaxQp = VENC_MAX_QP;
        pstChn->stRcAttr.stH265Cbr.u32StatTime = 0;
    } else if (PT_MJPEG == enType) {

    }

    /* gop attr */
    pstChn->stGopAttr.enGopMode = AX_VENC_GOPMODE_NORMALP;

    return 0;
}


static AX_VOID Create_Destroy(AX_S32 chn, AX_VENC_CHN_ATTR_T *pstChn, AX_S32 except, const AX_CHAR* funcName, AX_S32 lineNum)
{
    AX_S32 ret;

    ret = AX_VENC_CreateChn(chn, pstChn);
    UTEST_ASSERT_EQUAL_HEX(ret, except, funcName, lineNum);
    ret = AX_VENC_DestroyChn(chn);
}

static AX_VOID UTestCreateChnH264(AX_VOID)
{
    AX_S32 chn = 0, ret;
    AX_VENC_CHN_ATTR_T stChnAttr;
    memset(&stChnAttr, 0, sizeof(AX_VENC_CHN_ATTR_T));

    ret = AX_VENC_CreateChn(chn, NULL);
    UTEST_ASSERT_EQUAL_HEX(ret, AX_ERR_VENC_NULL_PTR, __func__, __LINE__);

    SampleVencAttrInit(&stChnAttr, PT_H264);
    stChnAttr.stVencAttr.u32MaxPicWidth = MAX_VENC_PIC_WIDTH + 1;
    Create_Destroy(chn, &stChnAttr, AX_ERR_VENC_ILLEGAL_PARAM, __func__, __LINE__);

    SampleVencAttrInit(&stChnAttr, PT_H264);
    stChnAttr.stVencAttr.u32MaxPicHeight = MAX_VENC_PIC_HEIGHT + 1;
    Create_Destroy(chn, &stChnAttr, AX_ERR_VENC_ILLEGAL_PARAM, __func__,__LINE__);

    SampleVencAttrInit(&stChnAttr, PT_H264);
    stChnAttr.stVencAttr.enStrmBitDepth = AX_VENC_STREAM_BUTT;
    Create_Destroy(chn, &stChnAttr, AX_ERR_VENC_ILLEGAL_PARAM, __func__, __LINE__);

    SampleVencAttrInit(&stChnAttr, PT_H264);
    stChnAttr.stVencAttr.enType = PT_1016;
    Create_Destroy(chn, &stChnAttr, AX_ERR_VENC_ILLEGAL_PARAM, __func__, __LINE__);

    SampleVencAttrInit(&stChnAttr, PT_H264);
    stChnAttr.stVencAttr.enProfile = AX_VENC_H264_BASE_PROFILE -1;
    Create_Destroy(chn, &stChnAttr, AX_ERR_VENC_ILLEGAL_PARAM, __func__, __LINE__);

    SampleVencAttrInit(&stChnAttr, PT_H264);
    stChnAttr.stVencAttr.enProfile = AX_VENC_H264_MAIN_PROFILE;
    Create_Destroy(chn, &stChnAttr, AX_SUCCESS, __func__, __LINE__);
    /* check venc channel attr */
}

static AX_VOID UTestDestroyChn()
{
    AX_S32 chn, ret;
    AX_VENC_CHN_ATTR_T stChnAttr;
    memset(&stChnAttr, 0, sizeof(AX_VENC_CHN_ATTR_T));

    SampleVencAttrInit(&stChnAttr, PT_H264);

    for (chn = 0; chn < MAX_VENC_CHN_NUM; chn++) {
        ret = AX_VENC_CreateChn(chn, &stChnAttr);
        UTEST_ASSERT_EQUAL_HEX(ret, AX_SUCCESS, __func__, __LINE__);
        ret = AX_VENC_DestroyChn(chn);
        UTEST_ASSERT_EQUAL_HEX(ret, AX_SUCCESS, __func__, __LINE__);
    }

    ret = AX_VENC_CreateChn(-1, &stChnAttr);
    UTEST_ASSERT_EQUAL_HEX(ret, AX_ERR_VENC_INVALID_CHNID, __func__, __LINE__);
    ret = AX_VENC_CreateChn(MAX_VENC_CHN_NUM, &stChnAttr);
    UTEST_ASSERT_EQUAL_HEX(ret, AX_ERR_VENC_INVALID_CHNID, __func__, __LINE__);

    ret = AX_VENC_DestroyChn(-1);
    UTEST_ASSERT_EQUAL_HEX(ret, AX_ERR_VENC_INVALID_CHNID, __func__, __LINE__);
    ret = AX_VENC_DestroyChn(MAX_VENC_CHN_NUM);
    UTEST_ASSERT_EQUAL_HEX(ret, AX_ERR_VENC_INVALID_CHNID, __func__, __LINE__);
}

static AX_VOID UTSendFrameInit(AX_VIDEO_FRAME_INFO_T *pstFrm)
{
    memset(pstFrm, 0, sizeof(AX_VIDEO_FRAME_INFO_T));
    pstFrm->stVFrame.u32Width = MAX_VENC_PIC_WIDTH;
    pstFrm->stVFrame.u32Height = MAX_VENC_PIC_HEIGHT;
    pstFrm->stVFrame.enImgFormat = AX_FORMAT_YUV420_PLANAR;
    pstFrm->stVFrame.u32PicStride[0] = pstFrm->stVFrame.u32Width;

    pstFrm->stVFrame.u64PhyAddr[0] = 0x1000;
    pstFrm->stVFrame.u32BlkId[0] = 0x10;
}

static AX_VOID UTestSendFrame()
{
    AX_S32 chn=1, actual;
    AX_VENC_CHN_ATTR_T stChnAttr;
    AX_VIDEO_FRAME_INFO_T stFrame;
    AX_S32 s32MilliSec = 0;

    memset(&stChnAttr, 0, sizeof(AX_VENC_CHN_ATTR_T));
    memset(&stFrame, 0, sizeof(AX_VIDEO_FRAME_INFO_T));

    actual = AX_VENC_SendFrame(chn, &stFrame, s32MilliSec);
    UTEST_ASSERT_EQUAL_HEX(actual, AX_ERR_VENC_NOT_PERMIT, __func__, __LINE__);

    SampleVencAttrInit(&stChnAttr, PT_H264);
    actual = AX_VENC_CreateChn(chn, &stChnAttr);
    UTEST_ASSERT_EQUAL_HEX(actual, AX_SUCCESS, __func__, __LINE__);

    actual = AX_VENC_SendFrame(chn, NULL, s32MilliSec);
    UTEST_ASSERT_EQUAL_HEX(actual, AX_ERR_VENC_NOT_PERMIT, __func__, __LINE__);

    AX_VENC_StartRecvFrame(chn, NULL);

    UTSendFrameInit(&stFrame);
    stFrame.stVFrame.u32Width = MAX_VENC_PIC_WIDTH;
    actual = AX_VENC_SendFrame(chn, &stFrame, s32MilliSec);
    UTEST_ASSERT_EQUAL_HEX(actual, AX_ERR_VENC_ILLEGAL_PARAM, __func__, __LINE__);

    stFrame.stVFrame.u32Width = MAX_VENC_PIC_WIDTH + 1;
    actual = AX_VENC_SendFrame(chn, &stFrame, s32MilliSec);
    UTEST_ASSERT_EQUAL_HEX(actual, AX_ERR_VENC_NOT_MATCH, __func__, __LINE__);

    actual = AX_VENC_DestroyChn(chn);
    UTEST_ASSERT_EQUAL_HEX(actual, AX_ERR_VENC_NOT_PERMIT, __func__, __LINE__);
    actual = AX_VENC_StopRecvFrame(chn);
    UTEST_ASSERT_EQUAL_HEX(actual, AX_SUCCESS, __func__, __LINE__);
    actual = AX_VENC_DestroyChn(chn);
    UTEST_ASSERT_EQUAL_HEX(actual, AX_SUCCESS, __func__, __LINE__);
}

static AX_VOID UTestGetStream()
{
    AX_S32 chn=2, actual;
    AX_VENC_CHN_ATTR_T stChnAttr;
    AX_VENC_STREAM_T stStream;
    AX_S32 s32MilliSec = 0;

    memset(&stChnAttr, 0, sizeof(AX_VENC_CHN_ATTR_T));
    memset(&stStream, 0, sizeof(AX_VENC_STREAM_T));

    actual = AX_VENC_GetStream(chn, &stStream, s32MilliSec);
    UTEST_ASSERT_EQUAL_HEX(actual, AX_ERR_VENC_UNEXIST, __func__, __LINE__);

    SampleVencAttrInit(&stChnAttr, PT_H265);
    actual = AX_VENC_CreateChn(chn, &stChnAttr);
    UTEST_ASSERT_EQUAL_HEX(actual, AX_SUCCESS, __func__, __LINE__);

    actual = AX_VENC_GetStream(chn, NULL, s32MilliSec);
    UTEST_ASSERT_EQUAL_HEX(actual, AX_ERR_VENC_NULL_PTR, __func__, __LINE__);

    actual = AX_VENC_DestroyChn(chn);
    UTEST_ASSERT_EQUAL_HEX(actual, AX_SUCCESS, __func__, __LINE__);
}

static AX_VOID UTestReleaseStream()
{
    AX_S32 chn=2, actual;
    AX_VENC_CHN_ATTR_T stChnAttr;
    AX_VENC_STREAM_T stStream;

    memset(&stChnAttr, 0, sizeof(AX_VENC_CHN_ATTR_T));
    memset(&stStream, 0, sizeof(AX_VENC_STREAM_T));

    SampleVencAttrInit(&stChnAttr, PT_H265);
    actual = AX_VENC_CreateChn(chn, &stChnAttr);
    UTEST_ASSERT_EQUAL_HEX(actual, AX_SUCCESS, __func__, __LINE__);

    actual = AX_VENC_ReleaseStream(chn, NULL);
    UTEST_ASSERT_EQUAL_HEX(actual, AX_ERR_VENC_NULL_PTR, __func__, __LINE__);

    actual = AX_VENC_DestroyChn(chn);
    UTEST_ASSERT_EQUAL_HEX(actual, AX_SUCCESS, __func__, __LINE__);
}

AX_S32 UTestParamsCheck(int argc, char *argv[])
{
    AX_S32 s32Ret = 0;


    RUN_TEST(UTestCreateChnH264);
    RUN_TEST(UTestDestroyChn);
    RUN_TEST(UTestSendFrame);
    RUN_TEST(UTestGetStream);
    RUN_TEST(UTestReleaseStream);

    return s32Ret;
}