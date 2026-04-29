/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <sys/prctl.h>

#include "ut_common_vdec_api.h"
#include "ut_common_vdec_utils.h"
#include "ut_vdec_test.h"


extern AX_U64 g_u64GetFrmTag;

AX_S32 SampleVdecRecvFrame(AX_VDEC_GRP VdGrp, AX_VDEC_CHN VdChn, SAMPLE_VDEC_CONTEXT_T *pstVdecCtx)
{
    AX_S32 ret;
    AX_S32 sRet = AX_SUCCESS;
    AX_VIDEO_FRAME_INFO_T stFrameInfo[SAMPLE_VDEC_FRAME_CNT];
    AX_VIDEO_FRAME_INFO_T *pstFrameInfo;
    AX_S32 sMilliSec = AX_ERR_VDEC_UNKNOWN;
    int ci = 0;
    int ii = 0;
    SAMPLE_VDEC_CMD_PARAM_T *pstCmd = NULL;
    SAMPLE_VDEC_USERPIC_T *pstVdecUserPic = NULL;

    if (pstVdecCtx == NULL) {
        SAMPLE_LOG("VdGrp=%d, pstVdecCtx == NULL", VdGrp);
        sRet = -1;
        goto ERR_RET;
    }

    pstCmd = &pstVdecCtx->tCmdParam;
    pstVdecUserPic = &pstVdecCtx->stVdecUserPic;
    sMilliSec = pstCmd->sMilliSec;

    for (ii = 0; ii < SAMPLE_VDEC_FRAME_CNT; ii++) {
        pstFrameInfo = &stFrameInfo[ii];

        memset(pstFrameInfo, 0, sizeof(AX_VIDEO_FRAME_INFO_T));
        SAMPLE_LOG_N("VdGrp=%d, VdChn=%d, sMilliSec:%d", VdGrp, VdChn, sMilliSec);

        ret = AXCL_VDEC_GetChnFrame(VD_GRP_API_MGR_ID(VdGrp), VdChn, pstFrameInfo, sMilliSec);
        if (ret == AX_SUCCESS) {
            g_u64GetFrmTag ++;
            SAMPLE_LOG("VdGrp=%d, VdChn=%d, AXCL_VDEC_GetChnFrame AX_SUCCESS\n",
                       VdGrp, VdChn);
        }
        else if (ret == AX_ERR_VDEC_QUEUE_EMPTY) {
            /* no data in unblock mode or timeout mode */
            SAMPLE_LOG("VdGrp=%d, VdChn=%d, AXCL_VDEC_GetChnFrame AX_ERR_VDEC_QUEUE_EMPTY\n",
                       VdGrp, VdChn);
            usleep(20 * 1000);
            sRet = ret;
            goto ERR_RET_RELEASE;
        }
        else if (ret == AX_ERR_VDEC_UNEXIST) {
            SAMPLE_WARN_LOG("VdGrp=%d, VdChn=%d, AXCL_VDEC_GetChnFrame AX_ERR_VDEC_UNEXIST \n",
                            VdGrp, VdChn);
            usleep(20 * 1000);
            sRet = ret;
            goto ERR_RET_RELEASE;
        }
        else if (ret == AX_ERR_VDEC_FLOW_END) {
            SAMPLE_LOG("VdGrp=%d, VdChn=%d, AXCL_VDEC_GetChnFrame AX_ERR_VDEC_FLOW_END\n",
                              VdGrp, VdChn);
            sRet = ret;
            goto ERR_RET_RELEASE;
        }
        else if (ret == AX_ERR_VDEC_STRM_ERROR) {
            SAMPLE_WARN_LOG("VdGrp=%d, VdChn=%d, AXCL_VDEC_GetChnFrame AX_ERR_VDEC_STRM_ERROR\n",
                            VdGrp, VdChn);
            sRet = ret;
            goto ERR_RET_RELEASE;
        }
        else if (AX_ERR_VDEC_NOT_PERM == ret) {
            SAMPLE_CRIT_LOG("VdGrp=%d, VdChn=%d, AXCL_VDEC_GetChnFrame AX_ERR_VDEC_NOT_PERM\n",
                            VdGrp, VdChn);
            sRet = ret;
            goto ERR_RET_RELEASE;
        }
        else {
            SAMPLE_CRIT_LOG("VdGrp=%d, VdChn=%d, AXCL_VDEC_GetChnFrame FAILED! ret=0x%x %s\n",
                            VdGrp, VdChn, ret, AX_VdecRetStr(ret));
            sRet = ret;
            goto ERR_RET_RELEASE;
        }

        SAMPLE_LOG("VdGrp=%d, VdChn:%d, ii:%d, after AXCL_VDEC_GetChnFrame "
                    ".u32Width:%d, .bEndOfStream:%d, .u64PhyAddr[0]:0x%llx, "
                    ".u64VirAddr[0]:0x%llx, BlkId[0]:0x%x, u64PTS:%lld, ."
                    "u64SeqNum:%lld, .u32TimeRef:%d, .u64PrivateData:0x%llx\n",
                    VdGrp, VdChn, ii,
                    pstFrameInfo->stVFrame.u32Width, pstFrameInfo->bEndOfStream,
                    pstFrameInfo->stVFrame.u64PhyAddr[0], pstFrameInfo->stVFrame.u64VirAddr[0],
                    pstFrameInfo->stVFrame.u32BlkId[0], pstFrameInfo->stVFrame.u64PTS,
                    pstFrameInfo->stVFrame.u64SeqNum, pstFrameInfo->stVFrame.u32TimeRef,
                    pstFrameInfo->stVFrame.u64PrivateData);
    }

    for (ii = 0; ii < SAMPLE_VDEC_FRAME_CNT; ii++) {
        pstFrameInfo = &stFrameInfo[ii];
        if (pstFrameInfo->bEndOfStream == AX_FALSE) {
            if (pstFrameInfo->stVFrame.u64PhyAddr[0] == 0) {
                if (pstVdecUserPic->usrPicChnEnaCnt && !pstCmd->tChnCfg[VdChn].bUserPicEnable) {
                    SAMPLE_LOG("VdGrp=%d, VdChn=%d, AXCL_VDEC_GetChnFrame FAILED! "
                               "pstFrameInfo->stVFrame.u64PhyAddr[0] == 0\n",
                               VdGrp, VdChn);
                } else {
                    SAMPLE_CRIT_LOG("VdGrp=%d, VdChn=%d, AXCL_VDEC_GetChnFrame FAILED! "
                                    "pstFrameInfo->stVFrame.u64PhyAddr[0] == 0\n",
                                    VdGrp, VdChn);
                    sRet = AX_ERR_VDEC_UNKNOWN;
                    goto ERR_RET_RELEASE;
                }
            }
        }
    }

    for (ii = 0; ii < SAMPLE_VDEC_FRAME_CNT; ii++) {
        pstFrameInfo = &stFrameInfo[ii];

        if (!pstFrameInfo->stVFrame.u64PhyAddr[0])
            continue;

        if (pstCmd->DestMD5) {
            AX_CHAR md5_str[33];

            memset(md5_str, 0, sizeof(md5_str));
            OutputFileCheckMD5(VdGrp, VdChn, pstFrameInfo, md5_str);
        }
    }

    SAMPLE_LOG_N("VdGrp=%d, VdChn:%d ", VdGrp, VdChn);

ERR_RET_RELEASE:
    for (ii = 0; ii < SAMPLE_VDEC_FRAME_CNT; ii++) {
        pstFrameInfo = &stFrameInfo[ii];

        SAMPLE_LOG_N("VdGrp=%d, VdChn:%d, ii:%d", VdGrp, VdChn, ii);

        if (!pstFrameInfo->stVFrame.u64PhyAddr[0])
            continue;

        SAMPLE_LOG("VdGrp=%d, VdChn:%d, ii:%d, before AX_VDEC_ReleaseChnFrame "
                    ".u64PhyAddr[0]:0x%llx, BlkId[0]:0x%x, u64SeqNum:%lld\n",
                    VdGrp, VdChn, ii, pstFrameInfo->stVFrame.u64PhyAddr[0],
                    pstFrameInfo->stVFrame.u32BlkId[0], pstFrameInfo->stVFrame.u64SeqNum);

        ret = AXCL_VDEC_ReleaseChnFrame(VD_GRP_API_MGR_ID(VdGrp), VdChn, pstFrameInfo);
        if (ret) {
            if (AX_ERR_VDEC_FLOW_END != ret) {
                sRet = ret;
                SAMPLE_CRIT_LOG("VdGrp=%d, VdChn:%d, AX_VDEC_ReleaseChnFrame FAILED! res:0x%x %s \n"
                                "u64PhyAddr[0]:0x%llX, BlkId[0]:0x%x, BlkId[1]:0x%x\n",
                                VdGrp, VdChn, ret, AX_VdecRetStr(ret), pstFrameInfo->stVFrame.u64PhyAddr[0],
                                pstFrameInfo->stVFrame.u32BlkId[0], pstFrameInfo->stVFrame.u32BlkId[1]);
                continue;
            }
        }

        if (sRet != AX_SUCCESS) {
            continue;
        }

        if (pstFrameInfo->bEndOfStream == AX_TRUE) {
            SAMPLE_LOG("VdGrp=%d, bEndOfStream == AX_TRUE, ret AX_ERR_VDEC_FLOW_END", VdGrp);
            sRet = AX_ERR_VDEC_FLOW_END;
            continue;
        }
    }

ERR_RET:
    for (ii = 0; ii < SAMPLE_VDEC_FRAME_CNT; ii++) {
        pstFrameInfo = &stFrameInfo[ii];
        SAMPLE_LOG_N("VdGrp=%d, VdChn:%d, .u64PTS:%lld, sRet0x%x %s\n",
                    VdGrp, VdChn, pstFrameInfo->stVFrame.u64PTS,
                    sRet, AX_VdecRetStr(sRet));
    }

    (void)ci;

    return sRet;
}



AX_S32 SampleVdecInputModeFrame(SAMPLE_VDEC_ARGS_T *pstFuncArgs,
                                SAMPLE_INPUT_FILE_INFO_T *pstStreamInfo,
                                SAMPLE_STREAM_BUF_T *pstStreamBuf)
{
    AX_S32 ret = 0;
    AX_S32 sRet = 0;
    AX_VDEC_STREAM_T tStrInfo = {0};
    AX_VDEC_GRP VdGrp = 0;
    SAMPLE_VDEC_CMD_PARAM_T *pstCmd = NULL;
    SAMPLE_VDEC_CONTEXT_T *pstCtx = NULL;

    size_t sReadLen = 0;
    AX_U32 uSendPicNum = 0;
    AX_S32 sMilliSec = AX_ERR_VDEC_UNKNOWN;
    AX_U64 uTmpPts = 0;

    if (NULL == pstFuncArgs) {
        SAMPLE_CRIT_LOG("NULL == pstFuncArgs\n");
        ret = AX_ERR_VDEC_UNKNOWN;
        goto ERR_RET;
    }

    VdGrp = pstFuncArgs->VdGrp;
    if (NULL == pstStreamInfo) {
        SAMPLE_CRIT_LOG("VdGrp=%d, NULL == pstStreamInfo\n", VdGrp);
        ret = AX_ERR_VDEC_UNKNOWN;
        goto ERR_RET;
    }

    if (NULL == pstStreamBuf) {
        SAMPLE_CRIT_LOG("VdGrp=%d, NULL == pstStreamBuf\n", VdGrp);
        ret = AX_ERR_VDEC_UNKNOWN;
        goto ERR_RET;
    }

    if (pstStreamBuf->tBufAddr.pVirAddr == NULL) {
        SAMPLE_CRIT_LOG("pstStreamBuf->tBufAddr.pVirAddr == NULL\n");
        ret = AX_ERR_VDEC_UNKNOWN;
        goto ERR_RET;
    }

    pstCtx = pstFuncArgs->pstCtx;
    if (NULL == pstCtx) {
        SAMPLE_CRIT_LOG("pstFuncArgs->pstCtx == NULL\n");
        ret = AX_ERR_VDEC_UNKNOWN;
        goto ERR_RET;
    }

    pstCmd = &pstCtx->tCmdParam;
    if (pstCmd == NULL) {
        SAMPLE_CRIT_LOG("VdGrp=%d, pstCmd == NULL", VdGrp);
        ret = AX_ERR_VDEC_UNKNOWN;
        goto ERR_RET;
    }

    sMilliSec = pstCmd->sMilliSec;
    pstStreamInfo->enDecType = pstCmd->enDecType;

    if (pstCmd->enSkipFrame) {
        tStrInfo.u64PTS = 0;
    }

    SAMPLE_LOG("begin to decoder. uBufSize=%d\n", pstStreamBuf->uBufSize);

    while (1) {
        SAMPLE_LOG("VdGrp=%d, pstCtx->GrpStatus[VdGrp]:%d\n",
                   VdGrp, pstCtx->GrpStatus[VdGrp]);

        switch (pstCtx->GrpStatus[VdGrp]) {
            case AX_VDEC_GRP_UNEXIST:
            case AX_VDEC_GRP_CREATED:
                continue;
            case AX_VDEC_GRP_START_RECV:
                break;
            default:
                ret = AX_ERR_VDEC_FLOW_END;
                goto ERR_RET;
        }

        if (pstCmd->bFfmpegEnable) {
            sRet = SampleVdecFfmpegExtractOnePic(&pstCtx->stFfmpeg, &pstCtx->stBitStreamInfo,
                                                pstStreamBuf, &sReadLen);
            if (sRet) {
                SAMPLE_CRIT_LOG("VdGrp=%d, SampleVdecFfmpegExtractOnePic FAILED! ret:0x%x\n", VdGrp, sRet);
                ret = AX_ERR_VDEC_UNKNOWN;
                goto ERR_RET;
            }
        } else {
            sRet = StreamFileParserReadFrame(pstStreamInfo, pstStreamBuf, &sReadLen);
            if (sRet) {
                SAMPLE_CRIT_LOG("VdGrp=%d, StreamParserReadFrameH264 FAILED! ret:0x%x\n", VdGrp, sRet);
                ret = AX_ERR_VDEC_UNKNOWN;
                goto ERR_RET;
            }
        }

        SAMPLE_LOG("VdGrp=%d, before AX_VDEC_SendStream, uSendPicNum:%d, sReadLen:0x%lx",
                   VdGrp, uSendPicNum, sReadLen);

        if (sReadLen > 0) {
            tStrInfo.pu8Addr = pstStreamBuf->tBufAddr.pVirAddr;
            tStrInfo.u64PhyAddr = 0;
            tStrInfo.u32StreamPackLen = (AX_U32)sReadLen;  /*stream len*/
            tStrInfo.bEndOfFrame = AX_TRUE;
            tStrInfo.bEndOfStream = AX_FALSE;
            if (pstCmd->enSkipFrame) {
                tStrInfo.u64PTS = uTmpPts + 1;
            } else {
                sRet = AXCL_SYS_GetCurPTS(&tStrInfo.u64PTS);
                if (sRet) {
                    SAMPLE_CRIT_LOG("VdGrp=%d, AX_SYS_GetCurPTS FAILED! ret:0x%x\n", VdGrp, sRet);
                    ret = sRet;
                    goto ERR_RET;
                }
            }
            tStrInfo.u64PrivateData = AX_VDEC_PRIVATE_DATA_VERIFY_NUM;
            tStrInfo.u64UserData = AX_VDEC_USER_DATA_VERIFY_NUM;
        } else {
            break;
        }

        SAMPLE_LOG("VdGrp=%d, tStrInfo.pu8Addr:%p, tStrInfo.u64PhyAddr:0x%llx, sRecvPicNum:%d ",
                   VdGrp, tStrInfo.pu8Addr, tStrInfo.u64PhyAddr, pstCmd->sRecvPicNum);

        SAMPLE_LOG("VdGrp=%d, .u64PTS:%lld .bEndOfStream:%d, .bEndOfFrame:%d, .bSkipDisplay:%d",
                   VdGrp, tStrInfo.u64PTS,
                   tStrInfo.bEndOfStream, tStrInfo.bEndOfFrame, tStrInfo.bSkipDisplay);

        sRet = AXCL_VDEC_SendStream(VD_GRP_API_MGR_ID(VdGrp), &tStrInfo, sMilliSec);
        if (sRet == AX_SUCCESS) {
            SAMPLE_LOG("VdGrp=%d, AXCL_VDEC_SendStream AX_SUCCESS, uSendPicNum:%d", VdGrp, uSendPicNum);
        }
        else if (sRet == AX_ERR_VDEC_FLOW_END) {
            SAMPLE_LOG("VdGrp=%d, AXCL_VDEC_SendStream ret AX_ERR_VDEC_FLOW_END, uSendPicNum:%d",
                          VdGrp, uSendPicNum);
            break;
        }
        else if (sRet == AX_ERR_VDEC_QUEUE_FULL) {
            usleep(1000);
            continue;
        }
        else if ((sRet == AX_ERR_VDEC_NOT_PERM) || (sRet == AX_ERR_VDEC_NOT_MATCH)) {
            SAMPLE_ERR_LOG("VdGrp=%d, AXCL_VDEC_SendStream FAILED! ret:0x%x %s",
                           VdGrp, sRet, AX_VdecRetStr(sRet));
            usleep(1000);
            continue;
        }
        else if ((sRet == AX_ERR_VDEC_NOT_SUPPORT)
                || (sRet == AX_ERR_VDEC_NOMEM)
                || (sRet == AX_ERR_VDEC_NOBUF)) {
            SAMPLE_CRIT_LOG("VdGrp=%d, AXCL_VDEC_SendStream FAILED! ret:0x%x %s",
                            VdGrp, sRet, AX_VdecRetStr(sRet));

            ret = SampleVdecSendEndOfStream(VdGrp);
            if (ret) {
                SAMPLE_CRIT_LOG("VdGrp=%d, SampleVdecSendEndOfStream FAILED! ret:0x%x %s",
                                VdGrp, ret, AX_VdecRetStr(ret));
            }
            goto ERR_RET;
        }
        else {
            SAMPLE_CRIT_LOG("VdGrp=%d, AXCL_VDEC_SendStream FAILED! ret:0x%x %s\n",
                            VdGrp, sRet, AX_VdecRetStr(sRet));
            ret = AX_ERR_VDEC_UNKNOWN;
            goto ERR_RET;
        }

        uSendPicNum ++;

        if (AX_VDEC_RECV_MODE_AFTER_SEND == pstCmd->enSelectMode) {
            for (int VdChn = 0; VdChn < AX_DEC_MAX_CHN_NUM; VdChn++) {
                if (pstFuncArgs->bChnEnable[VdChn] == AX_FALSE) continue;

                pstCtx->pOutputFilePath[VdChn] = pstCmd->tChnCfg[VdChn].pOutputFilePath;
                SAMPLE_LOG_N("u32GrpCount:%d, u32ChnCount:%d, VdGrp=%d, VdChn:%d, VdGrp:%d, VdChn:%d",
                            pstGrpSet->u32GrpCount, pstGrpSet->stChnSet[VdGrp].u32ChnCount,
                            VdGrp, VdChn, VdGrp, VdChn);

                ret = SampleVdecRecvFrame(VdGrp, VdChn, pstCtx);
                if (ret == AX_SUCCESS) {
                }
                else if (ret == AX_ERR_VDEC_STRM_ERROR) {
                    SAMPLE_WARN_LOG("VdGrp=%d, VdChn=%d, __VdecRecvFrame ret=0x%x %s\n",
                                    VdGrp, VdChn, ret, AX_VdecRetStr(ret));
                    continue;
                }
                else if (ret == AX_ERR_VDEC_QUEUE_EMPTY) {
                    SAMPLE_ERR_LOG("VdGrp=%d, VdChn=%d, __VdecRecvFrame ret=0x%x %s\n",
                                   VdGrp, VdChn, ret, AX_VdecRetStr(ret));
                    continue;
                }
                else if (ret == AX_ERR_VDEC_UNEXIST) {
                    SAMPLE_ERR_LOG("VdGrp=%d, VdChn=%d, __VdecRecvFrame ret=0x%x %s \n",
                                   VdGrp, VdChn, ret, AX_VdecRetStr(ret));
                    continue;
                }
                else if (AX_ERR_VDEC_NOT_PERM == ret) {
                    SAMPLE_CRIT_LOG("VdGrp=%d, VdChn=%d, __VdecRecvFrame ret=0x%x %s \n",
                                    VdGrp, VdChn, ret, AX_VdecRetStr(ret));
                    continue;
                }
                else if (ret == AX_ERR_VDEC_FLOW_END) {
                    SAMPLE_LOG("VdGrp=%d, VdChn=%d, __VdecRecvFrame ret=0x%x %s\n",
                               VdGrp, VdChn, ret, AX_VdecRetStr(ret));
                    continue;
                }
                else {
                    SAMPLE_CRIT_LOG("VdGrp=%d, VdChn=%d, __VdecRecvFrame FAILED! ret=0x%x %s\n",
                                    VdGrp, VdChn, ret, AX_VdecRetStr(ret));
                    goto ERR_RET;
                }
            }
        }
    }

    return 0;

ERR_RET:
    return ret;
}

AX_S32 VdecGrpChnSwitchTest(SAMPLE_VDEC_ARGS_T *pstFuncArgs, SAMPLE_STREAM_BUF_T *pstStreamBuf)
{
    AX_S32 ret = AX_SUCCESS;
    AX_S32 sRet = AX_SUCCESS;
    AX_VDEC_RECV_PIC_PARAM_T tRecvParam;
    AX_VDEC_GRP VdGrp = pstFuncArgs->VdGrp;
    SAMPLE_INPUT_FILE_INFO_T tStreamInfo = {0};

    SAMPLE_VDEC_CONTEXT_T *pstVdecCtx;
    SAMPLE_VDEC_CMD_PARAM_T *pstCmd;
    SAMPLE_VDEC_TEST_CTRL_T *pstTestCtrl;

    pstVdecCtx = pstFuncArgs->pstCtx;
    pstCmd = &pstVdecCtx->tCmdParam;
    pstTestCtrl = &pstVdecCtx->stVdecTestCtrl;

    memset(&tRecvParam, 0, sizeof(tRecvParam));

    if ((pstCmd->enDecType != PT_H264) && (pstCmd->enDecType != PT_H265)) {
        SAMPLE_CRIT_LOG("VdGrp=%d, pstCmd->enDecType:%d\n",
                        VdGrp, pstCmd->enDecType);
        goto ERR_RET;
    }

    if (pstCmd->sTestCnt > 0) {
        pstTestCtrl->s32TestCnt = pstCmd->sTestCnt;
    }

    if (pstTestCtrl->s32TestCnt <= 1) {
        pstTestCtrl->s32TestCnt = 3;
    }

    while (1) {
        sRet = AXCL_VDEC_StartRecvStream(VD_GRP_API_MGR_ID(VdGrp), &tRecvParam);
        if (sRet != AX_SUCCESS) {
            SAMPLE_CRIT_LOG("VdGrp=%d, AXCL_VDEC_StartRecvStream FAILED! ret:0x%x %s\n",
                            VdGrp, sRet, AX_VdecRetStr(sRet));
            goto ERR_RET;
        } else {
            pstVdecCtx->GrpStatus[VdGrp] = AX_VDEC_GRP_START_RECV;
        }

        SAMPLE_LOG("VdGrp=%d, pstVdecCtx:%p, pstCtx->GrpStatus[VdGrp]:%d\n",
                   VdGrp, pstVdecCtx, pstVdecCtx->GrpStatus[VdGrp]);

        SAMPLE_LOG_N("VdGrp=%d, pstCmd->enInputMode:%d \n", VdGrp, pstCmd->enInputMode);

        if ((AX_VDEC_INPUT_MODE_FRAME == pstCmd->enInputMode)
                || (AX_VDEC_INPUT_MODE_NAL == pstCmd->enInputMode)) {
            tStreamInfo.fInput = pstVdecCtx->pInputFd[VdGrp];
            tStreamInfo.sFileSize = pstVdecCtx->oInputFileSize[VdGrp];

            sRet = SampleVdecInputModeFrame(pstFuncArgs, &tStreamInfo, pstStreamBuf);
            if (sRet) {
                if (sRet != AX_ERR_VDEC_FLOW_END) {
                    SAMPLE_CRIT_LOG("VdGrp=%d, VdecInputModeFrame FAILED! ret:0x%x\n", VdGrp, sRet);
                    goto ERR_RET_STOP_RECV;
                }
            }
        }
        else {
            SAMPLE_CRIT_LOG("VdGrp=%d, Unsupport enInputMode:%d\n", VdGrp, pstCmd->enInputMode);
            goto ERR_RET_STOP_RECV;
        }

        sRet = AXCL_VDEC_StopRecvStream(VD_GRP_API_MGR_ID(VdGrp));
        if (sRet) {
            SAMPLE_CRIT_LOG("VdGrp=%d, AXCL_VDEC_StopRecvStream FAILED! ret:0x%x %s",
                            VdGrp, sRet, AX_VdecRetStr(sRet));
        } else {
            pstVdecCtx->GrpStatus[VdGrp] = AX_VDEC_GRP_STOP_RECV;
        }

        SAMPLE_LOG("VdGrp=%d, AXCL_VDEC_StopRecvStream Done! s32TestCnt:%d, GrpStatus:%d\n",
                      VdGrp, pstTestCtrl->s32TestCnt, pstVdecCtx->GrpStatus[VdGrp]);

        if (pstTestCtrl->s32TestCnt <= 0) {
            ret = SampleVdecForceEof(VdGrp, pstVdecCtx);
            if (ret) {
                SAMPLE_CRIT_LOG("VdGrp=%d, SampleVdecForceEof FAILED! ret:0x%x %s",
                                VdGrp, ret, AX_VdecRetStr(ret));
            }
            pstVdecCtx->bRecvFlowEnd = AX_TRUE;
            break;
        } else {
            rewind(tStreamInfo.fInput);

            sRet = SampleVdecFfmpegDeinit(&pstVdecCtx->stFfmpeg, VdGrp);
            if (sRet != AX_SUCCESS) {
                SAMPLE_CRIT_LOG("VdGrp=%d, SampleVdecFfmpegDeinit FAILED! ret:0x%x\n",
                                VdGrp, sRet);
            }

            sRet = SampleVdecFfmpegInit(&pstVdecCtx->stFfmpeg, pstCmd->pInputFilePath, &pstVdecCtx->stBitStreamInfo);
            if (sRet != AX_SUCCESS) {
                SAMPLE_CRIT_LOG("VdGrp=%d, SampleVdecFfmpegInit FAILED! ret:0x%x\n",
                                VdGrp, sRet);
            }

            pstTestCtrl->s32TestCnt -= 1;
        }

        if (pstTestCtrl->s32TestCnt % 2) {
            pstFuncArgs->bChnEnable[0] = AX_FALSE;
            pstFuncArgs->bChnEnable[1] = AX_TRUE;
            AXCL_VDEC_DisableChn(VD_GRP_API_MGR_ID(VdGrp), 0);
            AXCL_VDEC_EnableChn(VD_GRP_API_MGR_ID(VdGrp), 1);
        } else {
            pstFuncArgs->bChnEnable[0] = AX_TRUE;
            pstFuncArgs->bChnEnable[1] = AX_FALSE;
            AXCL_VDEC_EnableChn(VD_GRP_API_MGR_ID(VdGrp), 0);
            AXCL_VDEC_DisableChn(VD_GRP_API_MGR_ID(VdGrp), 1);
        }
    }

    sleep(1);

    return sRet;

ERR_RET_STOP_RECV:
    sRet = AXCL_VDEC_StopRecvStream(VD_GRP_API_MGR_ID(VdGrp));
    if (sRet) {
        SAMPLE_CRIT_LOG("VdGrp=%d, AXCL_VDEC_StopRecvStream FAILED! ret:0x%x", VdGrp, sRet);
    } else {
        pstVdecCtx->GrpStatus[VdGrp] = AX_VDEC_GRP_STOP_RECV;
        SAMPLE_LOG("VdGrp=%d, AXCL_VDEC_StopRecvStream Done!\n", VdGrp);
    }
ERR_RET:
    return sRet;
}





