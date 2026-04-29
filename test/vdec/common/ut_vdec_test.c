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
#include "ut_vdec_reset.h"
#include "ut_vdec_test.h"
#include "ut_common_vdec_cmd.h"

#include "h264_md5.h"
#include "h265_md5.h"

#include "axcl.h"



extern AX_U64 g_u64GetFrmTag;

/* only for basketballdrive_1920x1080_3M.bin. */
#define UT_STAND_VDEC_FPS_1080P 460


static volatile AX_S32 s_ThreadExit = 0;
static volatile SAMPLE_VDEC_CONTEXT_T *s_pstVdecCtx = NULL;
static AX_BOOL s_groupSuccess = AX_FALSE;
static SAMPLE_VDEC_GRP_SET_ARGS_T stGrpSetArgs;

static AX_U64 s_u64GetFrameNum[AX_VDEC_MAX_GRP_NUM][AX_DEC_MAX_CHN_NUM] = {0};
AX_BOOL s_bStartRecvFrm[AX_VDEC_MAX_GRP_NUM];
AX_BOOL bSeqNumValid[AX_VDEC_MAX_GRP_NUM] = {0};
AX_BOOL bTimeRefValid[AX_VDEC_MAX_GRP_NUM] = {0};
AX_BOOL bPTSValid[AX_VDEC_MAX_GRP_NUM] = {0};

AX_VDEC_GRP VdGrpApiMgr[AX_VDEC_MAX_GRP_NUM] = {0};
AX_VDEC_GRP VdGrpUTMgr[AX_VDEC_MAX_GRP_NUM] = {0};
AX_BOOL bCreatGrpEx = AX_FALSE;

AX_U32 old_width = 0;
AX_U32 old_Height = 0;
AX_U32 get_num = 0;

static void _VdecSigInt(int sigNo)
{
    AX_S32 s32Ret = 0;
    SAMPLE_VDEC_CMD_PARAM_T *pstCmd = NULL;
    SAMPLE_VDEC_CONTEXT_T *pstVdecCtx = NULL;
    AX_VDEC_GRP uStartGrpId = 0;
    AX_VDEC_GRP VdGrp = 0;
    AX_VDEC_GRP_STATUS_T stGrpStatus;

    s_ThreadExit = 1;

    if (s_pstVdecCtx == NULL) {
        SAMPLE_LOG("s_pstVdecCtx == NULL\n");
        goto ERR_RET;
    }

    pstVdecCtx = (SAMPLE_VDEC_CONTEXT_T *)s_pstVdecCtx;

    pstCmd = &pstVdecCtx->tCmdParam;

    uStartGrpId = pstCmd->uStartGrpId;
    for (VdGrp = uStartGrpId; VdGrp < (pstCmd->uGrpCount + uStartGrpId); VdGrp++) {
        memset(&stGrpStatus, 0, sizeof(stGrpStatus));
        s32Ret = AXCL_VDEC_QueryStatus(VD_GRP_API_MGR_ID(VdGrp), &stGrpStatus);
        if (s32Ret == AX_SUCCESS) {
        }
        else if (s32Ret == AX_ERR_VDEC_UNEXIST || s32Ret == AX_ERR_VDEC_NOT_PERM) {
            continue;
        }
        else {
            SAMPLE_CRIT_LOG("VdGrp=%d, AXCL_VDEC_QueryStatus ret:0x%x %s\n",
                            VdGrp, s32Ret, AX_VdecRetStr(s32Ret));
            continue;
        }

        if (stGrpStatus.bStartRecvStream) {
            s32Ret = AXCL_VDEC_StopRecvStream(VD_GRP_API_MGR_ID(VdGrp));
            if (s32Ret) {
                SAMPLE_CRIT_LOG("VdGrp=%d, AXCL_VDEC_StopRecvStream ret:0x%x %s\n",
                                VdGrp, s32Ret, AX_VdecRetStr(s32Ret));
                continue;
            }
            pstVdecCtx->GrpStatus[VdGrp] = AX_VDEC_GRP_STOP_RECV;
            SAMPLE_LOG("VdGrp=%d, AXCL_VDEC_StopRecvStream Done!\n", VdGrp);
        }

        while (1) {
            s32Ret = AXCL_VDEC_ResetGrp(VD_GRP_API_MGR_ID(VdGrp));
            if (s32Ret != AX_ERR_VDEC_BUSY) {
                break;
            }
            usleep(10000);
        }

        if (s32Ret) {
            SAMPLE_CRIT_LOG("VdGrp=%d, AXCL_VDEC_ResetGrp ret:0x%x %s\n",
                            VdGrp, s32Ret, AX_VdecRetStr(s32Ret));
            continue;
        }

        pstVdecCtx->GrpStatus[VdGrp] = AX_VDEC_GRP_RESET;

        SAMPLE_LOG("VdGrp=%d, reset done! pstVdecCtx:%p", VdGrp, pstVdecCtx);
    }
ERR_RET:
    s_ThreadExit += 1;

    SAMPLE_LOG("Catch signal %d, s_ThreadExit:%d ==============\n\n\n", sigNo, s_ThreadExit);

    if (s_ThreadExit > 3) {
        exit(0);
    }

    return;
}

static AX_S32 __VdecRecvFrame(AX_VDEC_GRP VdGrp, AX_VDEC_CHN VdChn, SAMPLE_VDEC_CONTEXT_T *pstVdecCtx)
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
    AX_U32 u32UsrPicBlkId = 0;
    AX_S32 usrPicGetChnCnt = 0;
    AX_BOOL c_cmp = 1;
    AX_U32 change = 0;

    if (pstVdecCtx == NULL) {
        SAMPLE_LOG("VdGrp=%d, pstVdecCtx == NULL", VdGrp);
        sRet = -1;
        goto ERR_RET;
    }

    pstCmd = &pstVdecCtx->tCmdParam;
    pstVdecUserPic = &pstVdecCtx->stVdecUserPic;
    u32UsrPicBlkId = pstVdecUserPic->stUserPic.stFrmInfo[VdChn].stVFrame.u32BlkId[0];
    sMilliSec = pstCmd->sMilliSec;

    for (ii = 0; ii < SAMPLE_VDEC_FRAME_CNT; ii++) {
        pstFrameInfo = &stFrameInfo[ii];

        memset(pstFrameInfo, 0, sizeof(AX_VIDEO_FRAME_INFO_T));

        ret = AXCL_VDEC_GetChnFrame(VD_GRP_API_MGR_ID(VdGrp), VdChn, pstFrameInfo, sMilliSec);
        if (ret == AX_SUCCESS) {
            s_u64GetFrameNum[VdGrp][VdChn] += 1;
            SAMPLE_LOG("VdGrp=%d, VdChn=%d, AXCL_VDEC_GetChnFrame AX_SUCCESS, %lld\n",
                       VdGrp, VdChn, s_u64GetFrameNum[VdGrp][VdChn]);
            if (pstCmd->enSkipFrame) {
                SAMPLE_LOG("sample_vdec_ut AXCL_VDEC_GetChnFrame PTS: %lld! \n", pstFrameInfo->stVFrame.u64PTS);
            }
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
            SAMPLE_LOG("VdGrp=%d, VdChn=%d, AXCL_VDEC_GetChnFrame AX_ERR_VDEC_FLOW_END %lld\n",
                              VdGrp, VdChn, s_u64GetFrameNum[VdGrp][VdChn]);
            sRet = ret;
            goto ERR_RET_RELEASE;
        }
        else if (ret == AX_ERR_VDEC_STRM_ERROR) {
            s_u64GetFrameNum[VdGrp][VdChn] += 1;
            SAMPLE_LOG("VdGrp=%d, VdChn=%d, AXCL_VDEC_GetChnFrame AX_ERR_VDEC_STRM_ERROR, %lld\n",
                            VdGrp, VdChn, s_u64GetFrameNum[VdGrp][VdChn]);
            sRet = ret;
            goto ERR_RET_RELEASE;
        }
        else if (AX_ERR_VDEC_NOT_PERM == ret) {
            SAMPLE_WARN_LOG("VdGrp=%d, VdChn=%d, AXCL_VDEC_GetChnFrame AX_ERR_VDEC_NOT_PERM\n",
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

#ifdef AX_VDEC_POOL_REFCNT_TEST
        int bi;

        while(1) {
            for (bi = 0; bi < SAMPLE_VDEC_REF_BLK_CNT; bi++) {
                if (pstVdecCtx->blkRef[VdGrp][bi] == AX_INVALID_BLOCKID) {
                    pstVdecCtx->blkRef[VdGrp][bi] = pstFrameInfo->stVFrame.u32BlkId[0];
                    SAMPLE_LOG("save pstVdecCtx->blkRef[VdGrp=%d][bi=%d]:0x%x",
                               VdGrp, bi, pstVdecCtx->blkRef[VdGrp][bi]);
                    break;
                }
            }

            if (bi < SAMPLE_VDEC_REF_BLK_CNT) {
                break;
            }
        }
#endif
        if (pstCmd->bModifyFrmSize && ((old_width && (old_width != pstFrameInfo->stVFrame.u32Width))
           || (old_Height && (old_Height != pstFrameInfo->stVFrame.u32Height)))) {
            change = 1;
        }
        if (get_num == 6) {
            c_cmp = 0;
        }
        if (pstCmd->bModifyFrmSize) {
            old_width = pstFrameInfo->stVFrame.u32Width;
            old_Height = pstFrameInfo->stVFrame.u32Height;
        }

        SAMPLE_LOG("VdGrp=%d, VdChn:%d, ii:%d, after AX_VDEC_GetChnFrame "
                    ".u32Width:%d, .bEndOfStream:%d, .u64PhyAddr[0]:0x%llx, "
                    ".u64VirAddr[0]:0x%llx, BlkId[0]:0x%x, u64PTS:%lld, .u64SeqNum:%lld, "
                    ".u32TimeRef:%d, .u64PrivateData:0x%llx\n",
                    VdGrp, VdChn, ii,
                    pstFrameInfo->stVFrame.u32Width, pstFrameInfo->bEndOfStream,
                    pstFrameInfo->stVFrame.u64PhyAddr[0], pstFrameInfo->stVFrame.u64VirAddr[0],
                    pstFrameInfo->stVFrame.u32BlkId[0], pstFrameInfo->stVFrame.u64PTS,
                    pstFrameInfo->stVFrame.u64SeqNum, pstFrameInfo->stVFrame.u32TimeRef,
                    pstFrameInfo->stVFrame.u64PrivateData);

        if (pstFrameInfo->stVFrame.u64PTS) bPTSValid[VdGrp] = AX_TRUE;
        if (pstFrameInfo->stVFrame.u64SeqNum) bSeqNumValid[VdGrp] = AX_TRUE;
        if (pstFrameInfo->stVFrame.u32TimeRef) bTimeRefValid[VdGrp] = AX_TRUE;

        if (pstCmd->bSendUserdata) {
            SAMPLE_INFO_LOG("stVFrame.u64PrivateData:0x%llx, stVFrame.u64UserData:0x%llx",
                          pstFrameInfo->stVFrame.u64PrivateData, pstFrameInfo->stVFrame.u64UserData);
        }
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

    SampleVdecFeatureTest(VdGrp, pstCmd);

    for (ii = 0; ii < SAMPLE_VDEC_FRAME_CNT; ii++) {
        pstFrameInfo = &stFrameInfo[ii];

        if (!pstFrameInfo->stVFrame.u64PhyAddr[0])
            continue;

        if (pstCmd->DestMD5 && pstCmd->bModifyFrmSize && (get_num < 360)) {
            AX_CHAR md5[33];
            const char *str = NULL;

            memset(md5, 0, sizeof(md5));
            OutputFileCheckMD5(VdGrp, VdChn, pstFrameInfo, md5);

            if (pstCmd->enDecType == PT_H264) {
                if (pstCmd->enOutputOrder == AX_VDEC_OUTPUT_ORDER_DEC) {
                    if (1280 == pstFrameInfo->stVFrame.u32Width)
                        str = h264_md5_orig_720p[get_num];
                    else
                        str = h264_md5_orig_1080p[get_num];
                } else {
                    if (1280 == pstFrameInfo->stVFrame.u32Width)
                        str = h264_md5_720p[get_num];
                    else
                        str = h264_md5_1080p[get_num];
                }
            } else if (pstCmd->enDecType == PT_H265) {
                if (pstCmd->enOutputOrder == AX_VDEC_OUTPUT_ORDER_DEC) {
                    if (1280 == pstFrameInfo->stVFrame.u32Width)
                        str = h265_md5_orig_720p[get_num];
                    else
                        str = h265_md5_orig_1080p[get_num];
                } else {
                    if (1280 == pstFrameInfo->stVFrame.u32Width)
                        str = h265_md5_720p[get_num];
                    else
                        str = h265_md5_1080p[get_num];
                }
            }

            if (c_cmp) {
                if (strcmp(md5, str)) {
                    SAMPLE_CRIT_LOG("AXCL_VDEC_GetChnFrame ERROR! frame md5:%s, right md5:%s, get_num:%d, c_cmp:%d, u32Width:%d,\n",
                                    md5, str, get_num, c_cmp, pstFrameInfo->stVFrame.u32Width);
                    sRet = AX_ERR_VDEC_NOT_MATCH;
                }
            } else {
                if (change && (((pstCmd->enOutputOrder == AX_VDEC_OUTPUT_ORDER_DEC) && (1920 != pstFrameInfo->stVFrame.u32Width))
                    || ((pstCmd->enOutputOrder == AX_VDEC_OUTPUT_ORDER_DISP) && (1280 != pstFrameInfo->stVFrame.u32Width)))) {
                    SAMPLE_CRIT_LOG("AXCL_VDEC_GetChnFrame ERROR! frame u32Width:%d, get_num:%d, c_cmp:%d\n",
                                    pstFrameInfo->stVFrame.u32Width, get_num, c_cmp);
                    sRet = AX_ERR_VDEC_NOT_MATCH;
                }
            }
            get_num ++;
        }

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

        SAMPLE_LOG("VdGrp=%d, VdChn:%d, ii:%d, before AXCL_VDEC_ReleaseChnFrame "
                    ".u64PhyAddr[0]:0x%llx, BlkId[0]:0x%x, u64SeqNum:%lld\n",
                    VdGrp, VdChn, ii, pstFrameInfo->stVFrame.u64PhyAddr[0],
                    pstFrameInfo->stVFrame.u32BlkId[0], pstFrameInfo->stVFrame.u64SeqNum);
        if (pstCmd->uResetCase == AX_VDEC_RESET_CASE_FORCE_BLOCK) {
            AX_U32 u32FrameBufCnt = pstCmd->tChnCfg[VdChn].u32FrameBufCnt;
            if (s_u64GetFrameNum[VdGrp][VdChn] < u32FrameBufCnt) {
                ut_vdecResetSaveFrm(VdGrp, VdChn, pstFrameInfo);
                continue;
            }
        }

        ret = AXCL_VDEC_ReleaseChnFrame(VD_GRP_API_MGR_ID(VdGrp), VdChn, pstFrameInfo);
        if (ret) {
            if (AX_ERR_VDEC_FLOW_END != ret) {
                sRet = ret;
                SAMPLE_CRIT_LOG("VdGrp=%d, VdChn:%d, AXCL_VDEC_ReleaseChnFrame FAILED! res:0x%x %s \n"
                                "u64PhyAddr[0]:0x%llX, BlkId[0]:0x%x, BlkId[1]:0x%x\n",
                                VdGrp, VdChn, ret, AX_VdecRetStr(ret), pstFrameInfo->stVFrame.u64PhyAddr[0],
                                pstFrameInfo->stVFrame.u32BlkId[0], pstFrameInfo->stVFrame.u32BlkId[1]);
                continue;
            }
        }

        /* This is only valid when using the user image Insert feature.
         * AX_VDEC_SetUserPic. */
        if (pstVdecUserPic->usrPicChnEnaCnt) {
            SAMPLE_LOG("VdGrp=%d, VdChn:%d, usrPicChnEnaCnt BlkId[0]:0x%x, u32UsrPicBlkId:0x%x",
                       VdGrp, VdChn, pstFrameInfo->stVFrame.u32BlkId[0], u32UsrPicBlkId);

            if (pstFrameInfo->stVFrame.u32BlkId[0] == u32UsrPicBlkId) {
                pstVdecUserPic->usrPicGet[VdChn] = AX_TRUE;
            }

            usrPicGetChnCnt = 0;
            for (ci = 0; ci < AX_DEC_MAX_CHN_NUM; ci++) {
                if (pstCmd->tChnCfg[ci].bUserPicEnable && pstVdecUserPic->usrPicGet[ci]) {
                    usrPicGetChnCnt++;
                }
            }

            if (usrPicGetChnCnt == pstVdecUserPic->usrPicChnEnaCnt)
                pstVdecUserPic->bAllChnGetUsrPic = AX_TRUE;
        }

        if (!pstCmd->recvStmAfUsrPic) {
            if (pstVdecUserPic->bAllChnGetUsrPic) {
                SAMPLE_LOG("VdGrp=%d, bAllChnGetUsrPic == AX_TRUE, ret AX_ERR_VDEC_FLOW_END", VdGrp);
                sRet = AX_ERR_VDEC_FLOW_END;
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

    return sRet;
}


static void *_VdecRecvThread(void *arg)
{
    AX_S32 sRet = 0;
    AX_S32 ret = 0;
    SAMPLE_VDEC_GRP_SET_ARGS_T *pstGrpSetArgs = (SAMPLE_VDEC_GRP_SET_ARGS_T *)arg;
    SAMPLE_VDEC_ARGS_T *pstVdecGrpArgs = NULL;
    SAMPLE_VDEC_CONTEXT_T *pstVdecCtx = NULL;
    SAMPLE_VDEC_CONTEXT_T *pstCtx_Grp0 = NULL;
    AX_VDEC_GRP_SET_INFO_T stGrpSet;
    AX_VDEC_GRP_SET_INFO_T *pstGrpSet = &stGrpSet;
    int gi, ci;
    AX_VDEC_GRP VdGrpApi = 0;
    AX_VDEC_GRP VdGrpUt = 0;
    AX_VDEC_CHN VdChn = 0;
    int flow_end_total_cnt = 0;
    SAMPLE_VDEC_CMD_PARAM_T *pstCmd = NULL;
    AX_U32 uGrpCount = 0;
    AX_BOOL bRecvFlowEnd[AX_VDEC_MAX_GRP_NUM] = {0};
    AX_VDEC_GRP uStartGrpId = -1;
    AX_VDEC_GRP maxGrpId = 0;
    AX_U32 noWorkingGrpCnt = 0;
    AX_U32 quitingGrpCnt = 0;
    axclrtContext context;
    axclError ax_ret;

    if (arg == NULL) {
        SAMPLE_CRIT_LOG("arg == NULL");
        return NULL;
    }

    ax_ret = axclrtCreateContext(&context, pstGrpSetArgs->device_id);
    if (AXCL_SUCC != ax_ret) {
        SAMPLE_CRIT_LOG("axclrtCreateContext error");
        return NULL;
    }

    AX_CHAR cPthreadName[16];
    snprintf(cPthreadName, 16, "SampleVdecRecv");
    prctl(PR_SET_NAME, cPthreadName);

    pstVdecGrpArgs = &pstGrpSetArgs->stVdecGrpArgs[0];
    if (pstVdecGrpArgs == NULL) {
        axclrtDestroyContext(context);
        SAMPLE_CRIT_LOG("pstVdecGrpArgs[0] == NULL");
        return NULL;
    }

    pstVdecCtx = pstVdecGrpArgs->pstCtx;
    if (pstVdecCtx == NULL) {
        axclrtDestroyContext(context);
        SAMPLE_CRIT_LOG("pstVdecCtx == NULL");
        return NULL;
    }

    pstCtx_Grp0 = pstVdecCtx;
    pstCmd = &pstVdecCtx->tCmdParam;
    uGrpCount = pstCmd->uGrpCount;
    uStartGrpId = pstCmd->uStartGrpId;
    maxGrpId = pstCmd->uGrpCount + uStartGrpId;

    memset(pstGrpSet, 0, sizeof(AX_VDEC_GRP_SET_INFO_T));
    SAMPLE_LOG("uGrpCount:%d Enter while(1) SelectGrp \n", uGrpCount);

    while (1) {
        if (s_ThreadExit > 1) {
            SAMPLE_LOG("s_ThreadExit:%d, so break\n", s_ThreadExit);
            break;
        }

        noWorkingGrpCnt = 0;
        quitingGrpCnt = 0;
        for (VdGrpUt = uStartGrpId; VdGrpUt < maxGrpId; VdGrpUt++) {
            switch (pstVdecCtx->GrpStatus[VdGrpUt]) {
                case AX_VDEC_GRP_UNEXIST:
                case AX_VDEC_GRP_CREATED:
                    noWorkingGrpCnt++;
                    break;
                case AX_VDEC_GRP_START_RECV:
                case AX_VDEC_GRP_STOP_RECV:
                    break;
                default:
                    quitingGrpCnt++;
                    break;
            }
        }

        VdGrpUt = 0;
        if (noWorkingGrpCnt == pstCmd->uGrpCount) {
            if (!s_groupSuccess && pstVdecCtx->RecvThdWait) {
                SAMPLE_LOG("s_groupSuccess:%d, pstVdecCtx->RecvThdWait:%d\n",
                           s_groupSuccess, pstVdecCtx->RecvThdWait);
                goto ERR_RET;
            } else {
                continue;
            }
        }

        if (quitingGrpCnt == pstCmd->uGrpCount) {
            SAMPLE_LOG("All groups are in the RESET or DESTROYED state. quitingGrpCnt:%d, pstCmd->uGrpCount:%d",
                              quitingGrpCnt, pstCmd->uGrpCount);
            goto ERR_RET;
        }

        if ((pstCmd->uResetCase == AX_VDEC_RESET_CASE_FORCE_BLOCK) &&
            (!s_bStartRecvFrm[VdGrpUt])) {
            usleep(30* 1000);
            continue;
        }

        SAMPLE_LOG("before SelectGrp \n");
        sRet = AXCL_VDEC_SelectGrp(pstGrpSet, 1000);
        if (sRet == AX_SUCCESS) {
            if (pstGrpSet->u32GrpCount == 0) {
                SAMPLE_ERR_LOG("AXCL_VDEC_SelectGrp AX_SUCCESS but u32GrpCount:0");
            } else {
                SAMPLE_LOG("AXCL_VDEC_SelectGrp u32GrpCount:%d", pstGrpSet->u32GrpCount);
            }
        }
        else if (AX_ERR_VDEC_TIMED_OUT == sRet) {
            SAMPLE_LOG("AXCL_VDEC_SelectGrp AX_ERR_VDEC_TIMED_OUT u32GrpCount:%d",
                              pstGrpSet->u32GrpCount);

            if (pstGrpSet->u32GrpCount == 0) {
                if (!s_groupSuccess && pstVdecCtx->RecvThdWait) {
                    SAMPLE_LOG("s_groupSuccess:%d && pstVdecCtx->RecvThdWait:%d",
                               s_groupSuccess, pstVdecCtx->RecvThdWait);
                    goto ERR_RET;
                } else {
                    continue;
                }
            }
        }
        else if (AX_ERR_VDEC_NOT_INIT == sRet) {
            SAMPLE_LOG("AX_ERR_VDEC_NOT_INIT, Need Call AXCL_VDEC_Init first");
            goto ERR_RET;
        }
        else if (AX_ERR_VDEC_FLOW_END == sRet) {
            SAMPLE_LOG("AXCL_VDEC_SelectGrp AX_ERR_VDEC_FLOW_END");
            if (!pstCmd->bResetTest) {
                goto ERR_RET;
            } else {
                continue;
            }
        }
        else {
            SAMPLE_CRIT_LOG("AXCL_VDEC_SelectGrp FAILED! ret:0x%x %s",
                            sRet, AX_VdecRetStr(sRet));
            goto ERR_RET;
        }

        SAMPLE_LOG("after SelectGrp, u32GrpCount:%d", pstGrpSet->u32GrpCount);
        for (gi = 0; gi < pstGrpSet->u32GrpCount; gi++) {
            VdGrpApi = pstGrpSet->stChnSet[gi].VdGrp;
            VdGrpUt = VD_GRP_UT_MGR_ID(VdGrpApi);
            if (VdGrpUt < uStartGrpId) {
                SAMPLE_CRIT_LOG("VdGrp:%d < uStartGrpId:%d", VdGrpUt, uStartGrpId);
                goto ERR_RET;
            }

            pstVdecGrpArgs = &pstGrpSetArgs->stVdecGrpArgs[VdGrpUt - uStartGrpId];
            if (pstVdecGrpArgs == NULL) {
                SAMPLE_LOG("VdGrp=%d, pstVdecGrpArgs == NULL", VdGrpUt);
                continue;
            }

            pstVdecCtx = pstVdecGrpArgs->pstCtx;
            if (pstVdecCtx == NULL) {
                SAMPLE_LOG("VdGrp=%d, pstVdecCtx == NULL", VdGrpUt);
                continue;
            }

            switch (pstVdecCtx->GrpStatus[VdGrpUt]) {
                case AX_VDEC_GRP_START_RECV:
                case AX_VDEC_GRP_STOP_RECV:
                    SAMPLE_LOG("VdGrp=%d, pstVdecCtx->GrpStatus[VdGrp]=%d\n",
                                VdGrpUt, pstVdecCtx->GrpStatus[VdGrpUt]);
                    break;
                default:
                    continue;
            }

            for (ci = 0; ci < pstGrpSet->stChnSet[gi].u32ChnCount; ci++) {
                VdChn = pstGrpSet->stChnSet[gi].VdChn[ci];
                if (pstCmd->tChnCfg[VdChn].pOutputFilePath != NULL) {
                    pstVdecCtx->pOutputFilePath[VdChn] = pstCmd->tChnCfg[VdChn].pOutputFilePath;
                }

                SAMPLE_LOG("u32GrpCount:%d, u32ChnCount:%d, VdGrp=%d, VdChn:%d, gi:%d, ci:%d",
                            pstGrpSet->u32GrpCount, pstGrpSet->stChnSet[gi].u32ChnCount,
                            VdGrpUt, VdChn, gi, ci);

                ret = __VdecRecvFrame(VdGrpUt, VdChn, pstVdecCtx);
                if (ret == AX_SUCCESS) {
                    pstCtx_Grp0->u64SelectFrameCnt++;
                }
                else if (ret == AX_ERR_VDEC_STRM_ERROR) {
                    SAMPLE_LOG("VdGrp=%d, VdChn=%d, _VdecRecvFrame ret=0x%x AX_ERR_VDEC_STRM_ERR\n",
                                       VdGrpUt, VdChn, ret);
                    continue;
                }
                else if (ret == AX_ERR_VDEC_QUEUE_EMPTY) {
                    SAMPLE_ERR_LOG("VdGrp=%d, VdChn=%d, _VdecRecvFrame ret=0x%x %s\n",
                                   VdGrpUt, VdChn, ret, AX_VdecRetStr(ret));
                    continue;
                }
                else if (ret == AX_ERR_VDEC_UNEXIST) {
                    SAMPLE_WARN_LOG("VdGrp=%d, VdChn=%d, _VdecRecvFrame ret=0x%x %s \n",
                                   VdGrpUt, VdChn, ret, AX_VdecRetStr(ret));
                    continue;
                }
                else if (AX_ERR_VDEC_NOT_PERM == ret) {
                    SAMPLE_WARN_LOG("VdGrp=%d, VdChn=%d, _VdecRecvFrame ret=0x%x %s \n",
                                    VdGrpUt, VdChn, ret, AX_VdecRetStr(ret));
                    continue;
                }
                else if (ret == AX_ERR_VDEC_FLOW_END) {
                    if (pstVdecCtx->stVdecUserPic.bAllChnGetUsrPic) {
                        pstCtx_Grp0->u64SelectFrameCnt++;
                    }

                    SAMPLE_LOG("VdGrp=%d, VdChn=%d, _VdecRecvFrame ret=0x%x %s\n",
                               VdGrpUt, VdChn, ret, AX_VdecRetStr(ret));
                    continue;
                }
                else {
                    SAMPLE_CRIT_LOG("VdGrp=%d, VdChn=%d, _VdecRecvFrame FAILED! ret=0x%x %s\n",
                                    VdGrpUt, VdChn, ret, AX_VdecRetStr(ret));
                    goto ERR_RET;
                }
            }

            if (ret == AX_ERR_VDEC_FLOW_END) {
                SAMPLE_LOG("VdGrp=%d, gi:%d, flow_end_total_cnt:%d, ->bRecvFlowEnd[VdGrp]:%d\n",
                           VdGrpUt, gi, flow_end_total_cnt, bRecvFlowEnd[VdGrpUt]);

                if (bRecvFlowEnd[VdGrpUt] == AX_FALSE) {
                    for (ci = 0; ci < AX_DEC_MAX_CHN_NUM; ci++)
                        ut_vdecResetReleaseFrm(VdGrpUt, ci);

                    flow_end_total_cnt++;
                    bRecvFlowEnd[VdGrpUt] = AX_TRUE;
                    SAMPLE_LOG("VdGrp=%d, gi:%d, flow_end_total_cnt:%d, ->bRecvFlowEnd[VdGrp]:%d\n",
                               VdGrpUt, gi, flow_end_total_cnt, bRecvFlowEnd[VdGrpUt]);
                }
            }
        }
#ifdef AX_VDEC_POOL_REFCNT_TEST
        AX_S32 sRefCnt = 0;
        for (int bi = 0; bi < SAMPLE_VDEC_REF_BLK_CNT; bi++) {
            if (pstVdecCtx->blkRef[VdGrp][bi] != AX_INVALID_BLOCKID) {

                // ret = AX_POOL_DecreaseRefCnt(pstVdecCtx->blkRef[VdGrp][bi], AX_ID_VDEC);
                // if (ret) {
                //     sRet = ret;
                //     SAMPLE_CRIT_LOG("VdGrp=%d, VdChn:%d, AX_POOL_DecreaseRefCnt FAILED! res:0x%x %s \n"
                //                     "blkRef[%d]:0x%x\n",
                //                     VdGrp, VdChn, ret, AX_VdecRetStr(ret), bi, pstVdecCtx->blkRef[VdGrp][bi]);
                //     goto ERR_RET;
                // }

                // SAMPLE_LOG("AX_POOL_DecreaseRefCnt blkRef[bi:%d]:0x%x", bi, pstVdecCtx->blkRef[VdGrp][bi]);

                // ret = AX_POOL_CheckRefCnt(pstVdecCtx->blkRef[VdGrp][bi], &sRefCnt);
                // if (ret) {
                //     sRet = ret;
                //     SAMPLE_CRIT_LOG("VdGrp=%d, VdChn:%d, AX_POOL_CheckRefCnt FAILED! res:0x%x %s u32BlkId:0x%x\n",
                //                     VdGrp, VdChn, ret, AX_VdecRetStr(ret), pstVdecCtx->blkRef[VdGrp][bi]);
                //     goto ERR_RET;
                // }

                // SAMPLE_LOG("VdGrp=%d, VdChn:%d, u32BlkId:0x%x sRefCnt:%d\n",
                //            VdGrp, VdChn, pstVdecCtx->blkRef[VdGrp][bi], sRefCnt);


                pstVdecCtx->blkRef[VdGrp][bi] = AX_INVALID_BLOCKID;
            }
        }
#endif
        if (flow_end_total_cnt == uGrpCount) {
            SAMPLE_LOG("flow_end_total_cnt:%d flow end!\n", flow_end_total_cnt);
            break;
        }
    }

ERR_RET:
    for (gi = 0; gi < uGrpCount; gi++) {
            VdGrpUt = gi;
            pstVdecGrpArgs = &pstGrpSetArgs->stVdecGrpArgs[VdGrpUt];
            if (pstVdecGrpArgs == NULL) {
                SAMPLE_LOG("VdGrp=%d, pstVdecGrpArgs == NULL", VdGrpUt);
                continue;
            }

            pstVdecCtx = pstVdecGrpArgs->pstCtx;
            if (pstVdecCtx == NULL) {
                SAMPLE_LOG("VdGrp=%d, pstVdecCtx == NULL", VdGrpUt);
                continue;
            }
            pstVdecCtx->bRecvFlowEnd = AX_TRUE;
    }

    if (pstCmd->bQuitWait == AX_TRUE) {
        SAMPLE_LOG("VdecRecvFrame Finished! Now waiting forever until press q");

        while(('q' != getchar())) {
            sleep(1);
        }

        for (gi = 0; gi < uGrpCount; gi++) {
            VdGrpUt = gi;
            pstVdecGrpArgs = &pstGrpSetArgs->stVdecGrpArgs[VdGrpUt];
            if (pstVdecGrpArgs == NULL) {
                SAMPLE_LOG("VdGrp=%d, pstVdecGrpArgs == NULL", VdGrpUt);
                continue;
            }

            pstVdecCtx = pstVdecGrpArgs->pstCtx;
            if (pstVdecCtx == NULL) {
                SAMPLE_LOG("VdGrp=%d, pstVdecCtx == NULL", VdGrpUt);
                continue;
            }
            pstVdecCtx->bGrpQuitWait[VdGrpUt] = AX_FALSE;
            SAMPLE_LOG("VdGrp=%d, bGrpQuitWait:%d", VdGrpUt, pstVdecCtx->bGrpQuitWait[VdGrpUt]);
        }
    }

    for (gi = 0; gi < uGrpCount; gi++) {
        VdGrpUt = gi;
        for (ci = 0; ci < AX_VDEC_MAX_CHN_NUM; ci++) {
            if (pstVdecCtx->pOutputFilePath[ci]) {
                free(pstVdecCtx->pOutputFilePath[ci]);
                pstVdecCtx->pOutputFilePath[ci] = NULL;
            }
        }
    }

    if (pstCtx_Grp0->u64SelectFrameCnt) {
        g_u64GetFrmTag += 1;
    } else {
        SAMPLE_CRIT_LOG("s_ThreadExit:%d, pstGrpSet->u32GrpCount:%d",
                        s_ThreadExit, pstGrpSet->u32GrpCount);
    }

    if (s_ThreadExit == 0) {
        s_ThreadExit += 2;
    }

    gettimeofday(&pstVdecCtx->Timeend, NULL);
    AX_U32 total_usec = 1000000 * (pstVdecCtx->Timeend.tv_sec - pstVdecCtx->Timebegin.tv_sec)
                        + pstVdecCtx->Timeend.tv_usec - pstVdecCtx->Timebegin.tv_usec;
    float total_msec = (float)total_usec / 1000.f;
    float msec_per_frame = total_msec / (float)pstCtx_Grp0->u64SelectFrameCnt;
    float avgFps = 1000.f / msec_per_frame;
    float minFps = 0;

    SAMPLE_INFO_LOG("uGrpCount=%d, msec per frame: %.1f, AVG FPS: %.1f. total msec:%.1f, total frame count:%lld \n",
                 uGrpCount, msec_per_frame, avgFps, total_msec, pstCtx_Grp0->u64SelectFrameCnt);

    SAMPLE_LOG(" RecvThread exit, last VdGrp=%d, VdChn:%d\n", VdGrpUt, VdChn);

    if (pstCmd->bMonitorFps) {
        minFps = 1.f * UT_STAND_VDEC_FPS_1080P * (100 - pstCmd->fpsDeviation) / 100;
        SAMPLE_LOG("fpsDeviation:%d, minFps:%1.f, avgFps:%1.f", pstCmd->fpsDeviation, minFps, avgFps);
        if (avgFps < minFps) {
            SAMPLE_CRIT_LOG("avgFps:%1.f < minFps:%1.f", avgFps, minFps);
        }
    }

    if (pstCmd->bCheckFrmParam && pstCtx_Grp0->u64SelectFrameCnt > 1) {
        for (gi = 0; gi < uGrpCount; gi++) {
            VdGrpUt = gi;
            if (bPTSValid[gi]) break;
        }

        if (gi == uGrpCount) {
            SAMPLE_CRIT_LOG("Failed. All u64PTS is 0.");
        }

        for (gi = 0; gi < uGrpCount; gi++) {
            VdGrpUt = gi;
            if (bSeqNumValid[gi]) break;
        }

        if (gi == uGrpCount) {
            SAMPLE_CRIT_LOG("Failed. All u64SeqNum is 0.");
        }

        for (gi = 0; gi < uGrpCount; gi++) {
            VdGrpUt = gi;
            if (bTimeRefValid[gi]) break;
        }

        if (gi == uGrpCount) {
            SAMPLE_CRIT_LOG("Failed. All u32TimeRef is 0.");
        }
    }

    /* step04: destory thread context */
    axclrtDestroyContext(context);

    return NULL;
}

static void *__VdecGrpChnRecvThread(void *arg)
{
    SAMPLE_VDEC_GRP_CHN_RECV_ARGS_T *pstFuncArgs = (SAMPLE_VDEC_GRP_CHN_RECV_ARGS_T *)arg;
    AX_VDEC_GRP VdGrp = 0;
    AX_S32 ret = 0;
    AX_VDEC_CHN VdChn = 0;
    SAMPLE_VDEC_CONTEXT_T *pstCtx = NULL;
    SAMPLE_VDEC_CMD_PARAM_T *pstCmd = NULL;
    AX_U32 recvFrmCnt = 0;
    AX_U32 sleepTime = 10;
    axclrtContext context;
    axclError ax_ret;

    if (arg == NULL) {
        SAMPLE_CRIT_LOG("arg == NULL");
        return NULL;
    }

    ax_ret = axclrtCreateContext(&context, pstFuncArgs->device_id);
    if (AXCL_SUCC != ax_ret) {
        SAMPLE_CRIT_LOG("axclrtCreateContext error\n");
        return NULL;
    }

    VdChn = pstFuncArgs->VdChn;
    VdGrp = pstFuncArgs->VdGrp;

    pstCtx = pstFuncArgs->pstCtx;
    if (NULL == pstCtx) {
        axclrtDestroyContext(context);
        SAMPLE_CRIT_LOG("pstFuncArgs->pstCtx == NULL");
        return NULL;
    }

    pstCmd = &pstCtx->tCmdParam;
    SAMPLE_LOG_N("VdGrp=%d, VdChn=%d, __VdecGrpChnRecvThread pstCtx:%p, pstFuncArgs->pstCtx:%p\n",
                VdGrp, VdChn, pstCtx, pstFuncArgs->pstCtx);

    while (1) {
        if (pstCtx->GrpStatus[VdGrp] == AX_VDEC_GRP_START_RECV) {
            break;
        }

        if (pstCtx->GrpStatus[VdGrp] == AX_VDEC_GRP_STOP_RECV) {
            break;
        }

        usleep(1000);
        SAMPLE_LOG("VdGrp=%d, VdChn=%d, pstCtx->GrpStatus[VdGrp]:%d\n",
                   VdGrp, VdChn, pstCtx->GrpStatus[VdGrp]);
    }


    while (1) {
        if (s_ThreadExit > 1) {
            SAMPLE_LOG("VdGrp=%d, VdChn:%d, s_ThreadExit:%d\n", VdGrp, VdChn, s_ThreadExit);
            break;
        }

        SAMPLE_LOG_N("VdGrp=%d, VdChn=%d", VdGrp, VdChn);

        if (pstCmd->bCheckLeftPic) {
            AX_VDEC_GRP_STATUS_T stGrpStatus;
            memset(&stGrpStatus, 0x0, sizeof(AX_VDEC_GRP_STATUS_T));
            if (recvFrmCnt == 1) {

                while (sleepTime > 0) {
                    usleep(1000 * 1000);
                    sleepTime--;
                }

                ret = AXCL_VDEC_QueryStatus(VD_GRP_API_MGR_ID(VdGrp), &stGrpStatus);
                if (AX_SUCCESS == ret) {
                    if ((pstCmd->enDecType == PT_JPEG || pstCmd->enDecType == PT_MJPEG)
                        && pstCmd->s32VdecVirtChn) {
                        if ((stGrpStatus.u32LeftPics[0] == 0) &&
                            (stGrpStatus.u32LeftPics[pstCmd->s32VdecVirtChn] != 0))
                            SAMPLE_INFO_LOG("================= VdGrp=%d, AXCL_VDEC_QueryStatus: "
                                         "s32VdecVirtChn=%d, u32LeftPics=%d\n",
                                         VdGrp, pstCmd->s32VdecVirtChn, stGrpStatus.u32LeftPics[pstCmd->s32VdecVirtChn]);
                        else
                            SAMPLE_CRIT_LOG("================= VdGrp=%d, AXCL_VDEC_QueryStatus: s32VdecVirtChn=%d, u32LeftPics=%d-%d-%d\n",
                                            VdGrp, pstCmd->s32VdecVirtChn, stGrpStatus.u32LeftPics[0],
                                            stGrpStatus.u32LeftPics[1], stGrpStatus.u32LeftPics[2]);
                    } else {
                        SAMPLE_LOG("================= VdGrp=%d, AXCL_VDEC_QueryStatus: u32LeftPics=%d-%d-%d\n",
                                     VdGrp, stGrpStatus.u32LeftPics[0],
                                     stGrpStatus.u32LeftPics[1], stGrpStatus.u32LeftPics[2]);
                    }
                } else {
                    SAMPLE_CRIT_LOG("VdGrp=%d,  AXCL_VDEC_QueryStatus FAILED! ret:0x%x", VdGrp, ret);
                }
            }
        }

        ret = __VdecRecvFrame(VdGrp, VdChn, pstCtx);
        if (ret != AX_SUCCESS) {
            if (ret == AX_ERR_VDEC_QUEUE_EMPTY) {
                SAMPLE_LOG("VdGrp=%d, VdChn=%d, _VdecRecvFrame ret=0x%x %s\n",
                           VdGrp, VdChn, ret, AX_VdecRetStr(ret));
                continue;
            }
            else if (ret == AX_ERR_VDEC_UNEXIST) {
                SAMPLE_ERR_LOG("VdGrp=%d, VdChn=%d, _VdecRecvFrame ret=0x%x %s \n",
                               VdGrp, VdChn, ret, AX_VdecRetStr(ret));
                continue;
            }
            else if (AX_ERR_VDEC_NOT_PERM == ret) {
                SAMPLE_WARN_LOG("VdGrp=%d, VdChn=%d, _VdecRecvFrame ret=0x%x %s \n",
                                VdGrp, VdChn, ret, AX_VdecRetStr(ret));
                continue;
            }
            else if (ret == AX_ERR_VDEC_STRM_ERROR) {
                SAMPLE_WARN_LOG("VdGrp=%d, VdChn=%d, _VdecRecvFrame ret=0x%x %s\n",
                                VdGrp, VdChn, ret, AX_VdecRetStr(ret));
                continue;
            }
            else if (ret == AX_ERR_VDEC_FLOW_END) {
                if (pstCtx->bRecvFlowEnd == AX_FALSE)
                    pstCtx->bRecvFlowEnd = AX_TRUE;
                SAMPLE_LOG("VdGrp=%d, VdChn=%d, _VdecRecvFrame ret=0x%x %s\n",
                           VdGrp, VdChn, ret, AX_VdecRetStr(ret));
                break;
            }

            SAMPLE_CRIT_LOG("VdGrp=%d, VdChn=%d, _VdecRecvFrame FAILED! ret=0x%x %s\n",
                            VdGrp, VdChn, ret, AX_VdecRetStr(ret));
            break;
        } else {
            recvFrmCnt++;
        }
    }

    if (recvFrmCnt) {
        g_u64GetFrmTag += 1;
    } else {
        SAMPLE_CRIT_LOG("VdGrp=%d, VdChn=%d, s_ThreadExit:%d",
                        VdGrp, VdChn, s_ThreadExit);
    }

    if (s_ThreadExit == 0) {
        s_ThreadExit += 1;
        SAMPLE_LOG("++s_ThreadExit:%d\n", s_ThreadExit);
    }

    /* step04: destory thread context */
    axclrtDestroyContext(context);

    SAMPLE_LOG("VdGrp=%d, VdChn:%d, RecvThread exit\n",
               VdGrp, VdChn);
    return NULL;
}



static AX_S32 __VdecInputModeFrame(SAMPLE_VDEC_ARGS_T *pstFuncArgs,
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
    AX_BOOL bContSendStm = AX_TRUE;
    AX_S32 sMilliSec = AX_ERR_VDEC_UNKNOWN;
    AX_BOOL bPerfTest = AX_FALSE;
    AX_BOOL bReadFrm = AX_TRUE;
    UT_VDEC_RESET_PARAM_T utVdecResetParam;
    AX_U64 uTmpPts = 0;
    AX_VDEC_CHN_ATTR_T *pstVdChnAttr = NULL;


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

    memset(&utVdecResetParam, 0x0, sizeof(UT_VDEC_RESET_PARAM_T));
    sMilliSec = pstCmd->sMilliSec;
    bPerfTest = pstCmd->bPerfTest;
    pstStreamInfo->enDecType = pstCmd->enDecType;

    utVdecResetParam.pstCtx = pstCtx;
    if (pstCmd->enSkipFrame) {
        tStrInfo.u64PTS = 0;
    }

    SAMPLE_LOG("begin to decoder. uBufSize=%d\n", pstStreamBuf->uBufSize);

    while (1) {
        if (s_ThreadExit > 1)
            break;
        if (pstCmd->sLoopDecNum <= 0) {
            sRet = SampleVdecSendEndOfStream(VdGrp);
            if (sRet) {
                SAMPLE_CRIT_LOG("VdGrp=%d, SampleVdecSendEndOfStream FAILED! ret:0x%x %s",
                                VdGrp, sRet, AX_VdecRetStr(sRet));
                ret = AX_ERR_VDEC_UNKNOWN;
                goto ERR_RET;
            }
            SAMPLE_LOG("VdGrp=%d, s_ThreadExit:%d, SampleVdecSendEndOfStream Done! break sendstream while(1)",
                       VdGrp, s_ThreadExit);

            if (AX_VDEC_RECV_MODE_AFTER_SEND == pstCmd->enSelectMode) {
                ret = AX_ERR_VDEC_FLOW_END;
                s_ThreadExit += 1;
                goto ERR_RET;
            }

            break;
        }

        SAMPLE_LOG("VdGrp=%d, pstCtx->GrpStatus[VdGrp]:%d", VdGrp, pstCtx->GrpStatus[VdGrp]);

        switch (pstCtx->GrpStatus[VdGrp]) {
            case AX_VDEC_GRP_UNEXIST:
            case AX_VDEC_GRP_CREATED:
            case AX_VDEC_GRP_STOP_RECV:
            case AX_VDEC_GRP_RESET:
                continue;
            case AX_VDEC_GRP_START_RECV:
                break;
            default:
                ret = AX_ERR_VDEC_FLOW_END;
                goto ERR_RET;
        }

        SAMPLE_LOG("VdGrp=%d, bReadFrm:%d, pstCmd->enInputMode:%d, pstCmd->bFfmpegEnable:%d",
                   VdGrp, bReadFrm, pstCmd->enInputMode, pstCmd->bFfmpegEnable);

        if (bReadFrm) {
            if ((pstCmd->enInputMode !=AX_VDEC_INPUT_MODE_STREAM)
                && ((pstCmd->enDecType == PT_H264) || (pstCmd->enDecType == PT_H265))) {
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
            } else if (pstCmd->enDecType == PT_JPEG) {
                if (!(bPerfTest && uSendPicNum)) {
                    sRet = StreamParserReadFrameJpeg(pstStreamInfo, pstStreamBuf, &sReadLen);
                    if (sRet) {
                        SAMPLE_CRIT_LOG("VdGrp=%d, StreamParserReadFrameJpeg FAILED! ret:0x%x\n", VdGrp, sRet);
                        ret = AX_ERR_VDEC_UNKNOWN;
                        goto ERR_RET;
                    }
                }
            }
        }

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
            if (pstCmd->enDecType == PT_JPEG) {
                if (!pstCmd->bDynRes) {
                    SAMPLE_LOG("pstCmd->sLoopDecNum:%d --", pstCmd->sLoopDecNum);
                    pstCmd->sLoopDecNum--;
                }
                else if (pstCmd->bDynRes && (pstStreamInfo->fInput == pstCtx->pNewInputFd[VdGrp])) {
                    SAMPLE_LOG("pstCmd->sLoopDecNum:%d --", pstCmd->sLoopDecNum);
                    pstCmd->sLoopDecNum--;
                    fseek(pstStreamInfo->fInput, 0, SEEK_SET);
                    pstStreamInfo->fInput = pstCtx->pInputFd[VdGrp];
                    pstStreamInfo->curPos = 0;
                    pstStreamInfo->sFileSize = pstCtx->oInputFileSize[VdGrp];
               }
               else if (pstCmd->bDynRes && (pstStreamInfo->fInput == pstCtx->pInputFd[VdGrp])) {
                    fseek(pstStreamInfo->fInput, 0, SEEK_SET);
                    pstStreamInfo->fInput = pstCtx->pNewInputFd[VdGrp];
                    pstStreamInfo->curPos = 0;
                    pstStreamInfo->sFileSize = pstCtx->oNewInputFileSize[VdGrp];
                }
            } else {
                pstCmd->sLoopDecNum--;
            }

            if (pstCmd->sLoopDecNum > 0) {
                if (pstCmd->bFfmpegEnable && pstCmd->enDecType != PT_JPEG) {
                    if ((pstCmd->enInputMode != AX_VDEC_INPUT_MODE_STREAM)
                            && ((pstCmd->enDecType == PT_H264) || (pstCmd->enDecType == PT_H265))) {
                        sRet = SampleVdecFfmpegDeinit(&pstCtx->stFfmpeg, pstCtx->stBitStreamInfo.VdGrp);
                        if (sRet != AX_SUCCESS) {
                            SAMPLE_CRIT_LOG("VdGrp=%d, SampleVdecFfmpegDeinit FAILED! ret:0x%x\n",
                                        VdGrp, sRet);
                        }
                        pstCtx->stBitStreamInfo.VdGrp = VdGrp;
                        sRet = SampleVdecFfmpegInit(&pstCtx->stFfmpeg, pstCmd->pInputFilePath, &pstCtx->stBitStreamInfo);
                        if (sRet != AX_SUCCESS) {
                            SAMPLE_CRIT_LOG("VdGrp=%d, SampleVdecFfmpegInit FAILED! ret:0x%x\n",
                                        VdGrp, sRet);
                        }
                        continue;
                    }
                } else {
                    fseek(pstStreamInfo->fInput, 0, SEEK_SET);
                    if (pstCmd->enDecType == PT_JPEG)
                        pstStreamInfo->curPos = 0;
                    continue;
                }
            } else {
                SAMPLE_LOG("VdGrp=%d, Notice! pstCmd->sLoopDecNum: %d\n", VdGrp, pstCmd->sLoopDecNum);
                continue;
            }
        }

        SAMPLE_LOG("VdGrp=%d, before AX_VDEC_SendStream, uSendPicNum:%d, sReadLen:0x%lx",
                   VdGrp, uSendPicNum, sReadLen);

        SAMPLE_LOG("VdGrp=%d, tStrInfo.pu8Addr:%p, tStrInfo.u64PhyAddr:0x%llx, sRecvPicNum:%d ",
                   VdGrp, tStrInfo.pu8Addr, tStrInfo.u64PhyAddr, pstCmd->sRecvPicNum);

        SAMPLE_LOG("VdGrp=%d, s_ThreadExit:%d, .u64PTS:%lld .bEndOfStream:%d, .bEndOfFrame:%d, .bSkipDisplay:%d",
                   VdGrp, s_ThreadExit, tStrInfo.u64PTS,
                   tStrInfo.bEndOfStream, tStrInfo.bEndOfFrame, tStrInfo.bSkipDisplay);

        if ((pstCtx->Timebegin.tv_sec == 0) && (pstCtx->Timebegin.tv_usec == 0)) {
            gettimeofday(&pstCtx->Timebegin, NULL);
        }

        if (pstCmd->enSkipFrame) {
            if (uSendPicNum % 2) {
                uTmpPts = tStrInfo.u64PTS;
                tStrInfo.u64PTS = -1;
            }
        }

        if (pstCmd->bModifyFrmSize) {
            if (uSendPicNum && (uSendPicNum <= 6))
                usleep(6000);

            if (uSendPicNum == 3) {
                /* channel 1 for scaler down */
                AX_VDEC_CHN VdChn = 1;
                pstVdChnAttr = pstFuncArgs->pstVdChnAttr[VdChn];

                pstVdChnAttr->enOutputMode = 2;
                pstVdChnAttr->u32PicWidth = 1280;
                pstVdChnAttr->u32PicHeight = 720;

                sRet = AXCL_VDEC_SetChnAttr(VD_GRP_API_MGR_ID(VdGrp), VdChn, pstVdChnAttr);
                if (sRet != AX_SUCCESS) {
                    SAMPLE_CRIT_LOG("VdGrp=%d, AXCL_VDEC_SetChnAttr FAILED! ret:0x%x %s\n",
                                    VdGrp, sRet, AX_VdecRetStr(sRet));
                    goto ERR_RET;
                }
             }

             if ((pstCmd->enOutputOrder == AX_VDEC_OUTPUT_ORDER_DEC) && (uSendPicNum == 6)) {
                pstVdChnAttr = pstFuncArgs->pstVdChnAttr[1];

                pstVdChnAttr->enOutputMode = 0;
                pstVdChnAttr->u32PicWidth = 1920;
                pstVdChnAttr->u32PicHeight = 1080;
                pstVdChnAttr->u32FrameStride = 2048;

                sRet = AXCL_VDEC_SetChnAttr(VD_GRP_API_MGR_ID(VdGrp), 1, pstVdChnAttr);
                if (sRet != AX_SUCCESS) {
                    ret = sRet;
                    SAMPLE_CRIT_LOG("VdGrp=%d, AXCL_VDEC_SetChnAttr FAILED! ret:0x%x %s\n",
                                    VdGrp, sRet, AX_VdecRetStr(sRet));
                    goto ERR_RET;
                }
             }
        }

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
            bReadFrm = AX_FALSE;
            usleep(1000);
            continue;
        }
        else if (sRet == AX_ERR_VDEC_NOT_MATCH) {
            SAMPLE_ERR_LOG("VdGrp=%d, AXCL_VDEC_SendStream FAILED! ret:0x%x %s",
                           VdGrp, sRet, AX_VdecRetStr(sRet));
            bReadFrm = AX_FALSE;
            usleep(1000);

            if (s_pstVdecCtx->GrpStatus[VdGrp] >= AX_VDEC_GRP_RESET) {
                break;
            }

            continue;
        }
        else if (sRet == AX_ERR_VDEC_NOT_PERM) {
            bReadFrm = AX_FALSE;
            usleep(1000);

            if (s_pstVdecCtx->GrpStatus[VdGrp] == AX_VDEC_GRP_STOP_RECV) {
                SAMPLE_WARN_LOG("VdGrp=%d, AX_VDEC_SendStream FAILED! ret:0x%x %s",
                                VdGrp, sRet, AX_VdecRetStr(sRet));
                break;
            } else if (s_pstVdecCtx->GrpStatus[VdGrp] >= AX_VDEC_GRP_RESET) {
                break;
            } else {
                SAMPLE_ERR_LOG("VdGrp=%d, AX_VDEC_SendStream FAILED! ret:0x%x %s",
                               VdGrp, sRet, AX_VdecRetStr(sRet));
            }
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
            ret = sRet;
            goto ERR_RET;
        }
        else {
            SAMPLE_CRIT_LOG("VdGrp=%d, AXCL_VDEC_SendStream FAILED! ret:0x%x %s\n",
                            VdGrp, sRet, AX_VdecRetStr(sRet));
            ret = AX_ERR_VDEC_UNKNOWN;
            goto ERR_RET;
        }

        bReadFrm = AX_TRUE;
        uSendPicNum ++;

        if (AX_VDEC_RECV_MODE_AFTER_SEND == pstCmd->enSelectMode) {
            for (int VdChn = 0; VdChn < AX_DEC_MAX_CHN_NUM; VdChn++) {
                if (pstFuncArgs->bChnEnable[VdChn] == AX_FALSE) continue;

                pstCtx->pOutputFilePath[VdChn] = pstCmd->tChnCfg[VdChn].pOutputFilePath;
                SAMPLE_LOG_N("u32GrpCount:%d, u32ChnCount:%d, VdGrp=%d, VdChn:%d, VdGrp:%d, VdChn:%d",
                            pstGrpSet->u32GrpCount, pstGrpSet->stChnSet[VdGrp].u32ChnCount,
                            VdGrp, VdChn, VdGrp, VdChn);

                ret = __VdecRecvFrame(VdGrp, VdChn, pstCtx);
                if (ret == AX_SUCCESS) {
                    g_u64GetFrmTag += 1;
                }
                else if (ret == AX_ERR_VDEC_STRM_ERROR) {
                    SAMPLE_WARN_LOG("VdGrp=%d, VdChn=%d, __VdecRecvFrame ret=0x%x %s\n",
                                    VdGrp, VdChn, ret, AX_VdecRetStr(ret));
                    g_u64GetFrmTag += 1;
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
                    SAMPLE_WARN_LOG("VdGrp=%d, VdChn=%d, __VdecRecvFrame ret=0x%x %s \n",
                                    VdGrp, VdChn, ret, AX_VdecRetStr(ret));
                    continue;
                }
                else if (ret == AX_ERR_VDEC_FLOW_END) {
                    SAMPLE_LOG("VdGrp=%d, VdChn=%d, __VdecRecvFrame ret=0x%x %s\n",
                               VdGrp, VdChn, ret, AX_VdecRetStr(ret));
                    g_u64GetFrmTag += 1;
                    continue;
                }
                else {
                    SAMPLE_CRIT_LOG("VdGrp=%d, VdChn=%d, __VdecRecvFrame FAILED! ret=0x%x %s\n",
                                    VdGrp, VdChn, ret, AX_VdecRetStr(ret));
                    goto ERR_RET;
                }
            }
        }

        if (pstCmd->bResetTest) {
            utVdecResetParam.uSendPicNum = uSendPicNum;
            utVdecResetParam.enResetCase = pstCmd->uResetCase;
            utVdecResetParam.bStartRecvFrm = &s_bStartRecvFrm[VdGrp];
            ret = ut_vdecReset(VdGrp, &utVdecResetParam);
            if (ret != AX_SUCCESS) {
                SAMPLE_CRIT_LOG("VdGrp=%d,  ut_vdecReset FAILED! ret=0x%x %s\n",
                                VdGrp, ret, AX_VdecRetStr(ret));
                goto ERR_RET;
            }
        }

        if (pstFuncArgs->tUsrPicArgs.pstVdecUserPic->usrPicChnEnaCnt
                &&  (uSendPicNum == pstCmd->usrPicIdx)) {
            sRet = VdecUserPicEnable(VdGrp, pstFuncArgs->tUsrPicArgs.pstVdecUserPic,
                                     &bContSendStm, pstCtx);
            if (sRet != AX_SUCCESS){
                SAMPLE_CRIT_LOG("VdGrp=%d, VdecUserPicEnable FAILED! ret:0x%x %s",
                                VdGrp, sRet, AX_VdecRetStr(sRet));
                goto ERR_RET;
            }

            if (!bContSendStm) {
                sReadLen = 0;
                SAMPLE_LOG("pstCmd->sLoopDecNum:%d --", pstCmd->sLoopDecNum);
                pstCmd->sLoopDecNum--;
                break;
            }
        }

        if (uSendPicNum == pstCmd->sRecvPicNum) {
            SAMPLE_LOG("pstCmd->sLoopDecNum:%d --", pstCmd->sLoopDecNum);
            pstCmd->sLoopDecNum--;
        }
    }

    return 0;

ERR_RET:
    return ret;
}



static AX_S32 __VdecCreateUserPool(SAMPLE_VDEC_ARGS_T *pstFuncArgs, AX_VDEC_CHN_ATTR_T *pstChnSet)
{
    AX_VDEC_CHN_ATTR_T *pstVdChnAttr[AX_DEC_MAX_CHN_NUM];
    AX_FRAME_COMPRESS_INFO_T tCompressInfo;
    AX_U32 FrameSize = 0;
    AX_VDEC_GRP VdGrp = AX_INVALID_ID;
    AX_VDEC_CHN VdChn = 0;
    AX_VDEC_GRP_ATTR_T *pstVdGrpAttr = NULL;
    AX_POOL_CONFIG_T *pstPoolConfig = NULL;
    AX_S32 s32Ret = AX_SUCCESS;
    AX_S32 sRet = AX_SUCCESS;
    int tmp_ci = 0;

    if (pstFuncArgs == NULL) {
        SAMPLE_CRIT_LOG("pstFuncArgs == NULL\n");
        s32Ret = AX_ERR_VDEC_NULL_PTR;
        goto ERR_RET;
    }

    VdGrp = pstFuncArgs->VdGrp;

    if (pstChnSet == NULL) {
        SAMPLE_CRIT_LOG("pstChnSet == NULL\n");
        s32Ret = AX_ERR_VDEC_NULL_PTR;
        goto ERR_RET;
    }

    tCompressInfo.enCompressMode = AX_COMPRESS_MODE_NONE;
    tCompressInfo.u32CompressLevel = 0;

    pstVdGrpAttr = &pstFuncArgs->tVdGrpAttr;

    for (VdChn = 0; VdChn < AX_DEC_MAX_CHN_NUM; VdChn++) {
        if (pstFuncArgs->bChnEnable[VdChn] == AX_FALSE) continue;

        pstVdChnAttr[VdChn] = pstChnSet + VdChn;
        if (pstVdChnAttr[VdChn] == NULL) {
            SAMPLE_CRIT_LOG("pstVdChnAttr[VdChn] == NULL\n");
            s32Ret = AX_ERR_VDEC_NULL_PTR;
            goto ERR_RET;
        }

        pstPoolConfig = &pstFuncArgs->tPoolConfig[VdChn];

        FrameSize = AX_VDEC_GetPicBufferSize(pstVdChnAttr[VdChn]->u32FrameStride,
                                            pstVdChnAttr[VdChn]->u32PicHeight,
                                            pstVdChnAttr[VdChn]->enImgFormat,
                                            &tCompressInfo, pstVdGrpAttr->enCodecType);

        pstPoolConfig->MetaSize = 512;
        pstPoolConfig->BlkCnt = pstVdChnAttr[VdChn]->u32OutputFifoDepth;

        SAMPLE_LOG("Get FrameSize is 0x%x %d, BlkCnt:%d\n",
                    FrameSize, FrameSize, pstPoolConfig->BlkCnt);

        pstPoolConfig->BlkSize = FrameSize;
        pstPoolConfig->CacheMode = POOL_CACHE_MODE_NONCACHE;
        // memset(pstPoolConfig->PartitionName, 0, sizeof(pstPoolConfig->PartitionName));
        snprintf((AX_CHAR *)pstPoolConfig->PartitionName, AX_MAX_PARTITION_NAME_LEN, "anonymous");
    }

    for (VdChn = 0; VdChn < AX_DEC_MAX_CHN_NUM; VdChn++) {
        if (pstFuncArgs->bChnEnable[VdChn] == AX_FALSE) continue;

        pstPoolConfig = &pstFuncArgs->tPoolConfig[VdChn];
        pstFuncArgs->PoolId[VdChn] = AXCL_POOL_CreatePool(pstPoolConfig);
        if (AX_INVALID_POOLID == pstFuncArgs->PoolId[VdChn]) {
            SAMPLE_CRIT_LOG("VdGrp=%d, AXCL_POOL_CreatePool FAILED! BlkCnt:%d, BlkSize:0x%llx\n",
                            VdGrp, pstPoolConfig->BlkCnt,
                            pstPoolConfig->BlkSize);
            goto ERR_RET_DESTROY_POOL;
        }
    }

    for (VdChn = 0; VdChn < AX_DEC_MAX_CHN_NUM; VdChn++) {
        if (pstFuncArgs->bChnEnable[VdChn] == AX_FALSE) continue;
        s32Ret = AXCL_VDEC_AttachPool(VD_GRP_API_MGR_ID(VdGrp), VdChn, pstFuncArgs->PoolId[VdChn]);
        if (s32Ret != AX_SUCCESS) {
            SAMPLE_CRIT_LOG("VdGrp=%d, VdChn=%d, AXCL_VDEC_AttachPool FAILED! PoolId[%d] ret:0x%x %s\n",
                            VdGrp, VdChn, pstFuncArgs->PoolId[VdChn],
                            s32Ret, AX_VdecRetStr(s32Ret));
            goto ERR_RET_DETACH_POOL;
        }
    }

    return s32Ret;

ERR_RET_DETACH_POOL:
    tmp_ci = VdChn;
    for (VdChn = 0; VdChn < AX_DEC_MAX_CHN_NUM; VdChn++) {
        if (pstFuncArgs->bChnEnable[VdChn] == AX_FALSE) continue;

        sRet = AXCL_VDEC_DetachPool(VD_GRP_API_MGR_ID(VdGrp), VdChn);
        if (sRet) {
            SAMPLE_CRIT_LOG("VdGrp=%d, VdChn=%d, AXCL_VDEC_DetachPool FAILED! ret:0x%x %s",
                            VdGrp, VdChn, sRet, AX_VdecRetStr(sRet));
        }
    }

ERR_RET_DESTROY_POOL:
    tmp_ci = VdChn;
    for (VdChn = 0; VdChn < tmp_ci; VdChn++) {
        if (pstFuncArgs->bChnEnable[VdChn] == AX_FALSE) continue;

        if (pstFuncArgs->PoolId[VdChn] == AX_INVALID_POOLID) continue;

        sRet = AXCL_POOL_DestroyPool(pstFuncArgs->PoolId[VdChn]);
        if (sRet) {
            SAMPLE_CRIT_LOG("VdGrp=%d, VdChn=%d, PoolId[VdChn]:%d, AXCL_POOL_DestroyPool FAILED! ret:0x%x %s",
                            VdGrp, VdChn, pstFuncArgs->PoolId[VdChn], sRet, AX_VdecRetStr(sRet));
        }
    }

ERR_RET:
    return s32Ret;
}


static AX_S32 __VdecGrpCreate(AX_VDEC_GRP VdGrp, AX_VDEC_GRP *pVdGrpApi, AX_VDEC_GRP_ATTR_T *pstVdGrpAttr,
                              SAMPLE_VDEC_CMD_PARAM_T *pstCmd, AX_U32 uStreamBufSize)
{
    AX_S32 s32Ret = AX_SUCCESS;
    AX_VDEC_GRP_ATTR_T stVdGrpAttr;
    AX_VDEC_GRP VdGrpApi = AX_ERR_INVALID_GRPID;

    pstVdGrpAttr->enCodecType = pstCmd->enDecType;
    pstVdGrpAttr->u32MaxPicWidth = pstCmd->u32MaxPicWidth;  /*Max pic width*/
    pstVdGrpAttr->u32MaxPicHeight = pstCmd->u32MaxPicHeight;  /*Max pic height*/
    pstVdGrpAttr->u32StreamBufSize = uStreamBufSize;
    pstVdGrpAttr->enInputMode = pstCmd->enInputMode;
    pstCmd->enFrameBufSrc = POOL_SOURCE_PRIVATE;

    if (pstCmd->enFrameBufSrc == POOL_SOURCE_USER) {
        pstVdGrpAttr->bSdkAutoFramePool = AX_FALSE;
    } else if (pstCmd->enFrameBufSrc == POOL_SOURCE_PRIVATE) {
        pstVdGrpAttr->bSdkAutoFramePool = AX_TRUE;
    } else {
        SAMPLE_CRIT_LOG("Unsupport enFrameBufSrc:%d\n",
                        pstCmd->enFrameBufSrc);
        goto ERR_RET;
    }

    pstVdGrpAttr->bSkipSdkStreamPool = AX_FALSE;


    if (pstCmd->bCreatGrpEx) {
        s32Ret = AXCL_VDEC_CreateGrpEx(&VdGrpApi, pstVdGrpAttr);
        if (s32Ret != AX_SUCCESS) {
            SAMPLE_CRIT_LOG("AXCL_VDEC_CreateGrpEx FAILED! ret:0x%x %s\n",
                            s32Ret, AX_VdecRetStr(s32Ret));
            goto ERR_RET;
        }
        *pVdGrpApi = VdGrpApi;
    } else {
        s32Ret = AXCL_VDEC_CreateGrp(VdGrp, pstVdGrpAttr);
        if (s32Ret != AX_SUCCESS) {
            SAMPLE_CRIT_LOG("VdGrp=%d, AXCL_VDEC_CreateGrp FAILED! ret:0x%x %s\n",
                            VdGrp, s32Ret, AX_VdecRetStr(s32Ret));
            goto ERR_RET;
        }
        *pVdGrpApi = VdGrpApi = VdGrp;
    }

    s32Ret = AXCL_VDEC_GetGrpAttr(VdGrpApi, &stVdGrpAttr);
    if (s32Ret != AX_SUCCESS) {
        SAMPLE_CRIT_LOG("VdGrpApi=%d, AXCL_VDEC_GetGrpAttr FAILED! ret:0x%x %s\n",
                        VdGrpApi, s32Ret, AX_VdecRetStr(s32Ret));
        goto ERR_RET;
    }

    SAMPLE_LOG("VdGrpApi=%d, ->enCodecType:%d, ->enInputMode:%d, "
               "->u32StreamBufSize:%d, ->bSdkAutoFramePool:%d",
               VdGrpApi, stVdGrpAttr.enCodecType, stVdGrpAttr.enInputMode,
               stVdGrpAttr.u32StreamBufSize, stVdGrpAttr.bSdkAutoFramePool);

ERR_RET:
    return s32Ret;
}

static AX_S32 __VdecInputModeStream(SAMPLE_VDEC_ARGS_T *pstFuncArgs,
                                   SAMPLE_INPUT_FILE_INFO_T *pstStreamInfo,
                                   SAMPLE_STREAM_BUF_T *pstStreamBuf)
{
    AX_S32 ret = 0;
    AX_S32 sRet = 0;
    AX_VDEC_STREAM_T tStrInfo = {0};
    AX_VDEC_GRP VdGrp = 0;
    SAMPLE_VDEC_CMD_PARAM_T *pstCmd = NULL;
    SAMPLE_VDEC_CONTEXT_T *pstCtx = NULL;
    FILE *fInput = NULL;
    off_t inputFileSize = 0;
    size_t fread_sz = 0;

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

    fInput = pstStreamInfo->fInput;
    rewind(fInput);

    inputFileSize = pstStreamInfo->sFileSize;
    AX_U32 uLeftSize = inputFileSize;
    AX_U32 uReadPackSize = pstCmd->sStreamSize;

    while (1) {
        if ((s_ThreadExit > 1) || (pstCmd->sLoopDecNum <= 0)) {
            sRet = SampleVdecSendEndOfStream(VdGrp);
            if (sRet) {
                SAMPLE_CRIT_LOG("VdGrp=%d, SampleVdecSendEndOfStream FAILED! ret:0x%x %s",
                                VdGrp, sRet, AX_VdecRetStr(sRet));
                ret = AX_ERR_VDEC_UNKNOWN;
                goto ERR_RET;
            }
            SAMPLE_LOG("VdGrp=%d, s_ThreadExit:%d, SampleVdecSendEndOfStream Done! break sendstream while(1)",
                       VdGrp, s_ThreadExit);
            break;
        }

        uReadPackSize = ((uLeftSize < pstCmd->sStreamSize) ? uLeftSize : pstCmd->sStreamSize);
        fread_sz = fread(pstStreamBuf->tBufAddr.pVirAddr, 1, uReadPackSize, fInput);
        uLeftSize = inputFileSize - uReadPackSize;
        tStrInfo.pu8Addr = pstStreamBuf->tBufAddr.pVirAddr;
        tStrInfo.u64PhyAddr = 0;
        tStrInfo.u32StreamPackLen = uReadPackSize;
        tStrInfo.bEndOfStream = AX_FALSE;

        if ((pstCtx->Timebegin.tv_sec == 0) && (pstCtx->Timebegin.tv_usec == 0)) {
            gettimeofday(&pstCtx->Timebegin, NULL);
        }

        SAMPLE_LOG("VdGrp=%d, pts:%lld inputFileSize:0x%lx fread_sz:0x%lx\n",
                    VdGrp, tStrInfo.u64PTS, inputFileSize, fread_sz);
        sRet = AXCL_VDEC_SendStream(VD_GRP_API_MGR_ID(VdGrp), &tStrInfo, -1);
        if (sRet != AX_SUCCESS) {
            SAMPLE_CRIT_LOG("VdGrp=%d, AXCL_VDEC_SendStream FAILED! ret:0x%x %s\n",
                            VdGrp, sRet, AX_VdecRetStr(sRet));
            ret = AX_ERR_VDEC_UNKNOWN;
            goto ERR_RET;
        }

        inputFileSize -= fread_sz;
        if (inputFileSize == 0) {
            SAMPLE_LOG("pstCmd->sLoopDecNum:%d --", pstCmd->sLoopDecNum);
            pstCmd->sLoopDecNum--;
            fseek(pstStreamInfo->fInput, 0, SEEK_SET);
            inputFileSize = pstStreamInfo->sFileSize;
            uLeftSize = inputFileSize;
            uReadPackSize = pstCmd->sStreamSize;
            continue;
        }
    }


    SAMPLE_LOG("VdGrp=%d, pts:%lld AXCL_VDEC_SendStream done! loop \n",
               VdGrp, tStrInfo.u64PTS);

    return AX_ERR_VDEC_FLOW_END;

ERR_RET:
    return ret;
}

static AX_S32 __VdecGrpSendStream(SAMPLE_VDEC_ARGS_T *pstFuncArgs, SAMPLE_STREAM_BUF_T *pstStreamBuf)
{
    AX_S32 sRet = AX_SUCCESS;
    AX_VDEC_RECV_PIC_PARAM_T tRecvParam;
    AX_VDEC_GRP VdGrp = pstFuncArgs->VdGrp;
    SAMPLE_INPUT_FILE_INFO_T tStreamInfo = {0};

    SAMPLE_VDEC_CONTEXT_T *pstVdecCtx;
    SAMPLE_VDEC_CMD_PARAM_T *pstCmd;
    AX_VDEC_DISPLAY_MODE_E enDisplayMode = AX_VDEC_DISPLAY_MODE_PREVIEW;

    pstVdecCtx = pstFuncArgs->pstCtx;
    pstCmd = &pstVdecCtx->tCmdParam;

    sRet = AXCL_VDEC_SetDisplayMode(VD_GRP_API_MGR_ID(VdGrp), pstCmd->enDisplayMode);
    if (sRet != AX_SUCCESS) {
        SAMPLE_CRIT_LOG("AXCL_VDEC_SetDisplayMode failed! ret:0x%x %s\n", sRet, AX_VdecRetStr(sRet));
        goto ERR_RET;
    }

    sRet = AXCL_VDEC_GetDisplayMode(VD_GRP_API_MGR_ID(VdGrp), &enDisplayMode);
    if (sRet != AX_SUCCESS) {
        SAMPLE_CRIT_LOG("AXCL_VDEC_GetDisplayMode failed! ret:0x%x %s\n", sRet, AX_VdecRetStr(sRet));
        goto ERR_RET;
    }

    if (enDisplayMode != pstCmd->enDisplayMode) {
        SAMPLE_CRIT_LOG("VdGrp=%d, Get enDisplayMode:%d != pstCmd->enDisplayMode:%d\n",
                        VdGrp, enDisplayMode, pstCmd->enDisplayMode);
    }

    if (s_ThreadExit) {
        SAMPLE_LOG("s_ThreadExit:%d\n", s_ThreadExit);
    }

    SAMPLE_LOG("sLoopDecNum:%d\n", pstCmd->sLoopDecNum);

    while (!s_ThreadExit && (pstCmd->sLoopDecNum > 0)) {
        memset(&tRecvParam, 0, sizeof(tRecvParam));
        tRecvParam.s32RecvPicNum = pstCmd->sRecvPicNum;
        sRet = AXCL_VDEC_StartRecvStream(VD_GRP_API_MGR_ID(VdGrp), &tRecvParam);
        if (sRet != AX_SUCCESS) {
            SAMPLE_CRIT_LOG("VdGrp=%d, AXCL_VDEC_StartRecvStream FAILED! ret:0x%x %s\n",
                            VdGrp, sRet, AX_VdecRetStr(sRet));
            goto ERR_RET;
        } else {
            pstVdecCtx->GrpStatus[VdGrp] = AX_VDEC_GRP_START_RECV;
        }

        SAMPLE_LOG_N("VdGrp=%d, pstVdecCtx:%p, pstCtx->GrpStatus[VdGrp]:%d\n",
                   VdGrp, pstVdecCtx, pstVdecCtx->GrpStatus[VdGrp]);

        SAMPLE_LOG_N("VdGrp=%d, pstCmd->enInputMode:%d \n", VdGrp, pstCmd->enInputMode);
        if (s_ThreadExit > 1) {
            SAMPLE_LOG("VdGrp=%d, s_ThreadExit:%d, so goto exit", VdGrp, s_ThreadExit);
            sRet = SampleVdecSendEndOfStream(VdGrp);
            if (sRet != AX_SUCCESS) {
                SAMPLE_CRIT_LOG("VdGrp=%d, SampleVdecSendEndOfStream FAILED! ret:0x%x %s\n",
                                VdGrp, sRet, AX_VdecRetStr(sRet));
                goto ERR_RET_STOP_RECV;
            }
        } else {
            if ((AX_VDEC_INPUT_MODE_FRAME == pstCmd->enInputMode)
                    || (AX_VDEC_INPUT_MODE_NAL == pstCmd->enInputMode)) {
                tStreamInfo.fInput = pstVdecCtx->pInputFd[VdGrp];
                tStreamInfo.sFileSize = pstVdecCtx->oInputFileSize[VdGrp];

                sRet = __VdecInputModeFrame(pstFuncArgs, &tStreamInfo, pstStreamBuf);
                if (sRet) {
                    if (sRet != AX_ERR_VDEC_FLOW_END) {
                        SAMPLE_CRIT_LOG("VdGrp=%d, VdecInputModeFrame FAILED! ret:0x%x\n", VdGrp, sRet);
                        goto ERR_RET_STOP_RECV;
                    }
                }
            }
            else if ((AX_VDEC_INPUT_MODE_COMPAT == pstCmd->enInputMode)
                    || (AX_VDEC_INPUT_MODE_STREAM == pstCmd->enInputMode)) {

                if (AX_VDEC_RECV_MODE_AFTER_SEND == pstCmd->enSelectMode) {
                    SAMPLE_CRIT_LOG("VdGrp=%d, The current enInputMode:%d does not support the RECV_MODE_AFTER_SEND.\n",
                                    VdGrp, pstCmd->enInputMode);
                    goto ERR_RET_STOP_RECV;
                }

                tStreamInfo.fInput = pstVdecCtx->pInputFd[VdGrp];
                tStreamInfo.sFileSize = pstVdecCtx->oInputFileSize[VdGrp];
                fseek(tStreamInfo.fInput, 0, SEEK_SET);

                sRet = __VdecInputModeStream(pstFuncArgs, &tStreamInfo, pstStreamBuf);
                if (sRet) {
                    if (sRet != AX_ERR_VDEC_FLOW_END) {
                        SAMPLE_CRIT_LOG("VdGrp=%d, VdecInputModeStream FAILED! ret:0x%x\n", VdGrp, sRet);
                        goto ERR_RET_STOP_RECV;
                    }
                }
            }
            else {
                SAMPLE_CRIT_LOG("VdGrp=%d, Unsupport enInputMode:%d\n", VdGrp, pstCmd->enInputMode);
                goto ERR_RET_STOP_RECV;
            }
        }

        sRet = AXCL_VDEC_StopRecvStream(VD_GRP_API_MGR_ID(VdGrp));
        if (sRet) {
            SAMPLE_CRIT_LOG("VdGrp=%d, AXCL_VDEC_StopRecvStream FAILED! ret:0x%x %s",
                            VdGrp, sRet, AX_VdecRetStr(sRet));
        } else {
            pstVdecCtx->GrpStatus[VdGrp] = AX_VDEC_GRP_STOP_RECV;
        }

        SAMPLE_LOG("VdGrp=%d, AXCL_VDEC_StopRecvStream Done! sLoopDecNum:%d\n",
                      VdGrp, pstCmd->sLoopDecNum);
        if (s_ThreadExit) {
            break;
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
    s_ThreadExit += 1;
    SAMPLE_LOG("++s_ThreadExit:%d\n", s_ThreadExit);
    return sRet;
}

static AX_S32 __VdecInitUsrPicArgs(SAMPLE_VDEC_ARGS_T *pstFuncArgs, SAMPLE_VDEC_USERPIC_T *pstVdecUserPic)
{
    SAMPLE_VDEC_CMD_PARAM_T *pstCmd = NULL;
    SAMPLE_VDEC_CONTEXT_T *pstVdecCtx = NULL;
    SAMPLE_VDEC_USRPIC_ARGS_T *pstUsrPicArgs = NULL;
    AX_VDEC_GRP VdGrp = 0;
    AX_VDEC_CHN VdChn = 0;
    AX_S32 sRet = AX_SUCCESS;

    if (pstFuncArgs == NULL) {
        sRet = AX_ERR_VDEC_UNKNOWN;
        SAMPLE_CRIT_LOG("pstFuncArgs == NULL\n");
        goto ERR_RET;
    }
    VdGrp = pstFuncArgs->VdGrp;
    if (pstVdecUserPic == NULL) {
        sRet = AX_ERR_VDEC_UNKNOWN;
        SAMPLE_CRIT_LOG("pstVdecUserPic == NULL\n");
        goto ERR_RET;
    }

    pstVdecCtx = pstFuncArgs->pstCtx;
    if (NULL == pstVdecCtx) {
        sRet = AX_ERR_VDEC_UNKNOWN;
        SAMPLE_CRIT_LOG("VdGrp=%d, pstFuncArgs->pstCtx == NULL\n", VdGrp);
        goto ERR_RET;
    }

    pstCmd = &pstVdecCtx->tCmdParam;
    pstVdecUserPic->recvStmAfUsrPic = pstCmd->recvStmAfUsrPic;
    pstVdecUserPic->s32RecvPicNumBak = pstCmd->sRecvPicNum;
    pstUsrPicArgs = &pstFuncArgs->tUsrPicArgs;
    pstUsrPicArgs->VdGrp = VdGrp;
    pstUsrPicArgs->bUsrInstant = pstCmd->bUsrInstant;
    pstUsrPicArgs->enDecType = pstCmd->enDecType;
    pstUsrPicArgs->pstVdecUserPic = pstVdecUserPic;
    for (VdChn = 0; VdChn < AX_DEC_MAX_CHN_NUM; VdChn++) {
        if (pstFuncArgs->bChnEnable[VdChn] == AX_FALSE) continue;
        pstUsrPicArgs->tChnParam[VdChn].bChnEnable = AX_TRUE;
        pstUsrPicArgs->tChnParam[VdChn].u32PicWidth = pstCmd->tChnCfg[VdChn].u32PicWidth;
        pstUsrPicArgs->tChnParam[VdChn].u32PicHeight = pstCmd->tChnCfg[VdChn].u32PicHeight;
        pstUsrPicArgs->tChnParam[VdChn].enImgFormat = pstCmd->tChnCfg[VdChn].enImgFormat;
        pstUsrPicArgs->tChnParam[VdChn].pUsrPicFilePath = pstCmd->tChnCfg[VdChn].pUsrPicFilePath;
        pstUsrPicArgs->tChnParam[VdChn].bUserPicEnable = pstCmd->tChnCfg[VdChn].bUserPicEnable;
    }

ERR_RET:
    return sRet;
}

static void *_VdecGroupThreadMain(void *arg)
{
    SAMPLE_VDEC_ARGS_T *pstFuncArgs = (SAMPLE_VDEC_ARGS_T *)arg;
    AX_S32 sRet = 0;
    AX_S32 s32Ret = 0;
    int res = 0;
    AX_VDEC_GRP VdGrp = 0;
    AX_VDEC_GRP VdGrpApi = 0;
    AX_VDEC_CHN VdChn = 0;
    FILE *fInput = NULL;
    off_t inputFileSize = 0;
    //size_t read_size = 0;
    AX_VDEC_GRP_ATTR_T *pstVdGrpAttr = NULL;
    SAMPLE_STREAM_BUF_T tStreamBuf = {0};
    AX_VDEC_STREAM_T stStrmBuf = {0};

    SAMPLE_VDEC_CMD_PARAM_T *pstCmd = NULL;
    SAMPLE_VDEC_CONTEXT_T *pstVdecCtx = NULL;
    AX_U32 tmp_size = 0;
    AX_S32 ErrorCode = 0;
    AX_BOOL bCreateUserPool = AX_FALSE;
    AX_S32 ci = 0;
    AX_U32 uBufSize = 0;
    axclrtContext context;
    axclError ret;
    AX_VOID *pHostVirAddr = NULL;
    size_t read_size = 0;

    if (pstFuncArgs == NULL) {
        SAMPLE_CRIT_LOG("pstFuncArgs == NULL\n");
        return NULL;
    }

    pstVdecCtx = pstFuncArgs->pstCtx;
    if (NULL == pstVdecCtx) {
        SAMPLE_CRIT_LOG("VdGrp=%d, pstFuncArgs->pstCtx == NULL\n", VdGrp);
        return NULL;
    }

    ret = axclrtCreateContext(&context, pstVdecCtx->device_id);
    if (AXCL_SUCC != ret) {
        SAMPLE_CRIT_LOG("axclrtCreateContext error\n");
        return NULL;
    }

    VdGrp = pstFuncArgs->VdGrp;
    SAMPLE_LOG("VdGrp=%d begin\n", VdGrp);

    AX_CHAR cPthreadName[16];
    snprintf(cPthreadName, 16, "SampleVdec%d", VdGrp);
    prctl(PR_SET_NAME, cPthreadName);

    pstVdecCtx->GrpPID[VdGrp] = gettid();

    pstCmd = &pstVdecCtx->tCmdParam;
    SAMPLE_LOG_N("pstCmd->sTestCnt:%d, pstCmd->sLoopDecNum:%d",
                pstCmd->sTestCnt, pstCmd->sLoopDecNum);

    if (pstCmd->pInputFilePath == NULL) {
        axclrtDestroyContext(context);
        SAMPLE_CRIT_LOG("VdGrp=%d, Please input pInputFilePath\n", VdGrp);
        return NULL;
    }

    fInput = fopen(pstCmd->pInputFilePath, "rb");
    if (fInput == NULL) {
        axclrtDestroyContext(context);
        SAMPLE_CRIT_LOG("VdGrp=%d, Unable to open input file:%s\n", VdGrp, pstCmd->pInputFilePath);
        return NULL;
    }

    res = fseek(fInput, 0L, SEEK_END);
    if (res) {
        SAMPLE_CRIT_LOG("VdGrp=%d, fseek FAILED! ret:%d\n", VdGrp, res);
        goto ERR_RET_FCLOSE;
    }

    inputFileSize = ftello(fInput);
    rewind(fInput);
    pstVdecCtx->pInputFd[VdGrp] = fInput;
    pstVdecCtx->oInputFileSize[VdGrp] = inputFileSize;

    if (pstCmd->bDynRes) {
        if (pstCmd->pNewInputFilePath == NULL) {
            SAMPLE_CRIT_LOG("VdGrp=%d, Please input pInputFilePath\n", VdGrp);
            return NULL;
        }

        fInput = fopen(pstCmd->pNewInputFilePath, "rb");
        if (fInput == NULL) {
            SAMPLE_CRIT_LOG("VdGrp=%d, Unable to open input file:%s\n", VdGrp, pstCmd->pNewInputFilePath);
            return NULL;
        }

        res = fseek(fInput, 0L, SEEK_END);
        if (res) {
            SAMPLE_CRIT_LOG("VdGrp=%d, fseek FAILED! ret:%d\n", VdGrp, res);
            goto ERR_RET_FCLOSE;
        }

        inputFileSize = ftello(fInput);
        rewind(fInput);
        pstVdecCtx->pNewInputFd[VdGrp] = fInput;
        pstVdecCtx->oNewInputFileSize[VdGrp] = inputFileSize;
    }

    SAMPLE_BITSTREAM_INFO_T *pstBitStreamInfo = &pstVdecCtx->stBitStreamInfo;
    inputFileSize = pstVdecCtx->oInputFileSize[VdGrp] > pstVdecCtx->oNewInputFileSize[VdGrp] ?
                        pstVdecCtx->oInputFileSize[VdGrp] : pstVdecCtx->oNewInputFileSize[VdGrp];
    SAMPLE_LOG("bDynRes:%d file:%s, pInputFd:%p, FileSize: %lx :%ld",
               pstCmd->bDynRes, pstCmd->pInputFilePath, pstVdecCtx->pInputFd[VdGrp],
               inputFileSize, pstVdecCtx->oInputFileSize[VdGrp]);

    if (pstCmd->bFfmpegEnable) {
        if (((pstCmd->enDecType == PT_H264) || (pstCmd->enDecType == PT_H265))) {
            pstBitStreamInfo->VdGrp = VdGrp;
            s32Ret = SampleVdecFfmpegInit(&pstVdecCtx->stFfmpeg, pstCmd->pInputFilePath, pstBitStreamInfo);
            if (s32Ret != AX_SUCCESS) {
                SAMPLE_CRIT_LOG("VdGrp=%d, SampleVdecFfmpegInit FAILED! ret:0x%x\n",
                            VdGrp, s32Ret);
                goto ERR_RET_FCLOSE;
            }
        }
    }

    if (pstCmd->highRes) {
        tStreamBuf.uBufSize = STREAM_BUFFER_MAX_SIZE_HIGH_RES;
    }
    else {
        if (pstCmd->enDecType == PT_JPEG) {
            uBufSize = inputFileSize > STREAM_BUFFER_MIN_SIZE ? inputFileSize : STREAM_BUFFER_MIN_SIZE;
            tStreamBuf.uBufSize = uBufSize > STREAM_BUFFER_MAX_SIZE ? STREAM_BUFFER_MAX_SIZE : uBufSize;
        } else {
            tStreamBuf.uBufSize = inputFileSize > STREAM_BUFFER_MAX_SIZE ? STREAM_BUFFER_MAX_SIZE : inputFileSize;
            if (pstCmd->enInputMode ==AX_VDEC_INPUT_MODE_STREAM) {
                tStreamBuf.uBufSize = (pstCmd->sStreamSize * 2) > STREAM_BUFFER_MAX_SIZE ?
                                            (pstCmd->sStreamSize * 2) : STREAM_BUFFER_MAX_SIZE;
            }
        }
    }

    sRet = axclrtMallocHost(&pHostVirAddr, tStreamBuf.uBufSize);
    if (AX_SUCCESS != sRet) {
        SAMPLE_CRIT_LOG("axclrtMallocHost FAILED! uStrmSize:0x%x s32Ret:0x%x\n",
                         tStreamBuf.uBufSize, sRet);
        goto ERR_RET_FFMPEG_DEINIT;
    }
    tStreamBuf.tBufAddr.pVirAddr = pHostVirAddr;
    tStreamBuf.pBufBeforeFill = tStreamBuf.tBufAddr.pVirAddr;
    tStreamBuf.pBufAfterFill = tStreamBuf.tBufAddr.pVirAddr;
    tStreamBuf.bRingbuf = AX_FALSE;


    AX_VDEC_STREAM_T stStreamBuf;
    const AX_VDEC_STREAM_T *pstStreamBuf = &stStrmBuf;
    AX_VDEC_BITSTREAM_INFO_T stStreamHeadInfo;
    AX_VDEC_BITSTREAM_INFO_T *pstStreamHeadInfo = &stStreamHeadInfo;
    memset(&stStreamBuf, 0, sizeof(AX_VDEC_STREAM_T));
    memset(pstStreamHeadInfo, 0, sizeof(AX_VDEC_BITSTREAM_INFO_T));

    stStreamBuf.pu8Addr = tStreamBuf.tBufAddr.pVirAddr;
    stStreamBuf.u64PhyAddr = tStreamBuf.tBufAddr.u64PhyAddr;
    stStreamBuf.u32StreamPackLen = tStreamBuf.uBufSize;

    if (pstCmd->bExtractHead) {
        size_t parse_len = stStrmBuf.u32StreamPackLen = stStreamBuf.u32StreamPackLen;
        read_size = fread(pHostVirAddr, sizeof(AX_U8), (size_t)parse_len, fInput);
        if (read_size != parse_len) {
            SAMPLE_CRIT_LOG("fread FAILED! read_size:%ld != parse_len:%ld\n",
                            read_size, parse_len);
            goto ERR_RET_FFMPEG_DEINIT;
        }
        rewind(fInput);
        /*for device slave memory*/
        s32Ret = AXCL_SYS_MemAlloc(&stStrmBuf.u64PhyAddr, (AX_VOID **)&stStrmBuf.pu8Addr,
                                    tStreamBuf.uBufSize, 0x100, (AX_S8 *)"vdec_input_stream");
        if (s32Ret != AX_SUCCESS) {
            SAMPLE_CRIT_LOG("VdGrp=%d, AX_SYS_MemAlloc FAILED! uBufSize:0x%x ret:0x%x\n",
                             VdGrp, tStreamBuf.uBufSize, s32Ret);
            goto ERR_RET_FFMPEG_DEINIT;
        }
        s32Ret = axclrtMemcpy((AX_VOID *)stStrmBuf.u64PhyAddr, pHostVirAddr, read_size, AXCL_MEMCPY_HOST_TO_DEVICE);
        if (AX_SUCCESS != s32Ret) {
            SAMPLE_CRIT_LOG("axclrtMemcpy FAILED! ret:0x%x\n", s32Ret);
            goto ERR_RET_FREE_STREAM;
        }

        sRet = AXCL_VDEC_ExtractStreamHeaderInfo(pstStreamBuf, pstCmd->enDecType,
                                               pstStreamHeadInfo);

        if (sRet != AX_SUCCESS) {
            SAMPLE_ERR_LOG("VdGrp=%d, AXCL_VDEC_ExtractStreamHeaderInfo FAILED! ret:0x%x %s\n",
                            VdGrp, sRet, AX_VdecRetStr(sRet));
        } else {
            SAMPLE_INFO_LOG("VdGrp=%d, pstStreamHeadInfo->u32RefFramesNum:%d, "
                        "->u32Width:%d, ->u32Height:%d\n",
                        VdGrp, pstStreamHeadInfo->u32RefFramesNum,
                        pstStreamHeadInfo->u32Width, pstStreamHeadInfo->u32Height);
        }
    }

    if (pstCmd->enDecType == PT_H264 || pstCmd->enDecType == PT_H265) {
        for (VdChn = 0; VdChn < AX_DEC_MAX_CHN_NUM; VdChn++) {
            if (pstCmd->tChnCfg[VdChn].bChnEnable == AX_FALSE) continue;
            if ((pstCmd->tChnCfg[VdChn].u32PicWidth == 0) || (pstCmd->tChnCfg[VdChn].u32PicHeight == 0)) {
                if (sRet == AX_SUCCESS) {
                    for (ci = 0; ci < AX_DEC_MAX_CHN_NUM; ci++) {
                        if (pstCmd->tChnCfg[ci].enOutputMode == AX_VDEC_OUTPUT_ORIGINAL) {
                            pstCmd->tChnCfg[ci].u32PicWidth = pstVdecCtx->stBitStreamInfo.nWidth;
                            pstCmd->tChnCfg[ci].u32PicHeight = pstVdecCtx->stBitStreamInfo.nHeight;
                        } else {
                            if (pstCmd->tChnCfg[ci].u32PicWidth == 0) {
                                pstCmd->tChnCfg[ci].u32PicWidth = pstCmd->u32MaxPicWidth;
                            }

                            if (pstCmd->tChnCfg[ci].u32PicHeight == 0) {
                                pstCmd->tChnCfg[ci].u32PicHeight = pstCmd->u32MaxPicHeight;
                            }
                        }
                    }

                    if (pstCmd->u32MaxPicWidth < pstVdecCtx->stBitStreamInfo.nWidth) {
                        pstCmd->u32MaxPicWidth = pstVdecCtx->stBitStreamInfo.nWidth;
                    }

                    if (pstCmd->u32MaxPicHeight < pstVdecCtx->stBitStreamInfo.nHeight) {
                        pstCmd->u32MaxPicHeight = pstVdecCtx->stBitStreamInfo.nHeight;
                    }
                } else {
                    for (ci = 0; ci < AX_DEC_MAX_CHN_NUM; ci++) {
                        if (pstCmd->tChnCfg[ci].u32PicWidth == 0) {
                            pstCmd->tChnCfg[ci].u32PicWidth = pstCmd->u32MaxPicWidth;
                        }

                        if (pstCmd->tChnCfg[ci].u32PicHeight == 0) {
                            pstCmd->tChnCfg[ci].u32PicHeight = pstCmd->u32MaxPicHeight;
                        }
                    }
                }

                break;
            }
        }
    } else {
        for (ci = 0; ci < AX_DEC_MAX_CHN_NUM; ci++) {
            if (pstCmd->tChnCfg[ci].u32PicWidth == 0) {
                pstCmd->tChnCfg[ci].u32PicWidth = pstCmd->u32MaxPicWidth;
            }

            if (pstCmd->tChnCfg[ci].u32PicHeight == 0) {
                pstCmd->tChnCfg[ci].u32PicHeight = pstCmd->u32MaxPicHeight;
            }
        }
    }

    for (ci = 0; ci < AX_DEC_MAX_CHN_NUM; ci++) {
        if (!pstCmd->tChnCfg[ci].bChnEnable) continue;
        if (pstCmd->tChnCfg[ci].u32PicWidth == 0) {
            SAMPLE_CRIT_LOG("VdGrp=%d, pstCmd->tChnCfg[%d].u32PicWidth == 0\n",
                            VdGrp, ci);
        }

        if (pstCmd->tChnCfg[ci].u32PicHeight == 0) {
            SAMPLE_CRIT_LOG("VdGrp=%d, pstCmd->tChnCfg[%d].u32PicHeight == 0\n",
                            VdGrp, ci);
        }
    }

    pstVdGrpAttr = &pstFuncArgs->tVdGrpAttr;

    if (pstCmd->bQuitWait == AX_TRUE) {
        pstVdecCtx->bGrpQuitWait[VdGrp] = AX_TRUE;
    }

    sRet = __VdecGrpCreate(VdGrp, &VdGrpApi, pstVdGrpAttr, pstCmd, tStreamBuf.uBufSize);
    if (sRet != AX_SUCCESS) {
        SAMPLE_CRIT_LOG("VdGrp=%d, VdChn=%d, __VdecGrpCreate FAILED! ret:0x%x\n",
                        VdGrp, VdChn, sRet);
        goto ERR_RET_FFMPEG_DEINIT;
    }

    VdGrpApiMgr[VdGrp] = VdGrpApi;
    VdGrpUTMgr[VdGrpApi] = VdGrp;
    pstVdecCtx->GrpStatus[VdGrp] = AX_VDEC_GRP_CREATED;
    SAMPLE_LOG("VdGrp=%d, AX_VDEC_CreateGrp done!", VdGrp);

    tmp_size = sizeof(AX_VDEC_CHN_ATTR_T) * AX_DEC_MAX_CHN_NUM;

    AX_VDEC_CHN_ATTR_T *pstChnSet = calloc(1, tmp_size);
    if (pstChnSet == NULL) {
        SAMPLE_CRIT_LOG("calloc FAILED! size:0x%x\n", tmp_size);
        goto ERR_RET_DESTROY_GRP;
    }

    sRet = SampleVdecChnAttrEnable(pstFuncArgs, pstChnSet);
    if (sRet != AX_SUCCESS) {
        SAMPLE_CRIT_LOG("VdGrp=%d, VdChn=%d, SampleVdecChnAttrEnable FAILED! ret:0x%x\n",
                        VdGrp, VdChn, sRet);
        goto ERR_RET_CHNATTR_FREE;
    }

    if (pstVdGrpAttr->bSdkAutoFramePool == AX_FALSE) {
        sRet = __VdecCreateUserPool(pstFuncArgs, pstChnSet);
        if (sRet != AX_SUCCESS) {
            SAMPLE_CRIT_LOG("VdGrp=%d, VdChn=%d, __VdecCreateUserPool FAILED! ret:0x%x\n",
                            VdGrp, VdChn, sRet);
            goto ERR_RET_CHNATTR_FREE;
        }
        bCreateUserPool = AX_TRUE;
    }

    AX_VDEC_GRP_PARAM_T stGrpParam;
    memset(&stGrpParam, 0, sizeof(stGrpParam));
    stGrpParam.stVdecVideoParam.enOutputOrder = pstCmd->enOutputOrder;
    stGrpParam.stVdecVideoParam.enVdecMode = pstCmd->enVideoMode;
    stGrpParam.f32SrcFrmRate = pstCmd->f32SrcFrmRate;
    sRet = AXCL_VDEC_SetGrpParam(VD_GRP_API_MGR_ID(VdGrp), &stGrpParam);
    if (0 != sRet) {
        SAMPLE_CRIT_LOG("VdGrp=%d, AXCL_VDEC_SetGrpParam fail, ret = 0x%x", VdGrp, sRet);
        goto ERR_RET_DETACH_POOL;
    }

    sRet = __VdecInitUsrPicArgs(pstFuncArgs, &pstVdecCtx->stVdecUserPic);
    if (sRet != AX_SUCCESS) {
        SAMPLE_CRIT_LOG("VdGrp=%d, __VdecInitUsrPicArgs failed. ret:0x%x %s\n",
                         VdGrp, sRet, AX_VdecRetStr(sRet));
        goto ERR_RET_DETACH_POOL;
    }

    sRet = __VdecUsrPicCreat(&pstFuncArgs->tUsrPicArgs, &pstVdecCtx->stVdecUserPic);
    if (sRet != AX_SUCCESS) {
        SAMPLE_CRIT_LOG("VdGrp=%d, __VdecUsrPicCreat failed. ret:0x%x %s\n",
                         VdGrp, sRet, AX_VdecRetStr(sRet));
        goto ERR_RET_DETACH_POOL;
    }

    s_groupSuccess = AX_TRUE;


    if (pstCmd->bChnSwitchTest) {
        SAMPLE_LOG_N("VdGrp=%d, pstCmd->sTestCnt:%d\n", VdGrp, pstCmd->sTestCnt);
        sRet = VdecGrpChnSwitchTest(pstFuncArgs, &tStreamBuf);
        if (sRet != AX_SUCCESS) {
            SAMPLE_CRIT_LOG("VdGrp=%d, VdecGrpChnSwitchTest failed. ret:0x%x %s\n",
                            VdGrp, sRet, AX_VdecRetStr(sRet));
            goto ERR_RET_DISABLE_USER_PIC;
        }
    } else {
        SAMPLE_LOG_N("VdGrp=%d, pstCmd->sTestCnt:%d\n", VdGrp, pstCmd->sTestCnt);
        sRet = __VdecGrpSendStream(pstFuncArgs, &tStreamBuf);
        if (sRet != AX_SUCCESS) {
            SAMPLE_CRIT_LOG("VdGrp=%d, __VdecGrpSendStream failed. ret:0x%x %s\n",
                            VdGrp, sRet, AX_VdecRetStr(sRet));
            goto ERR_RET_DISABLE_USER_PIC;
        }
    }

    SAMPLE_LOG("VdGrp=%d, bQuitWait:%d, s_ThreadExit:%d, bRecvFlowEnd:%d bGrpQuitWait:%d\n",
               VdGrp, pstCmd->bQuitWait, s_ThreadExit,
               pstVdecCtx->bRecvFlowEnd, pstVdecCtx->bGrpQuitWait[VdGrp]);

    if (pstCmd->bQuitWait == AX_TRUE) {
        SAMPLE_LOG("VdecGrpSendStream Finished! Now waiting RecvFlowEnd");
    }

    while (1) {
        if (s_ThreadExit > 0) {
            SAMPLE_LOG("VdGrp=%d, s_ThreadExit:%d, break while(1)!", VdGrp, s_ThreadExit);
            break;
        }

        sleep(2);

        if ((AX_TRUE == pstVdecCtx->bRecvFlowEnd) && (AX_FALSE == pstVdecCtx->bGrpQuitWait[VdGrp])) {
            SAMPLE_LOG("VdGrp=%d, bRecvFlowEnd break while(1)! \n", VdGrp);
            break;
        }
        SAMPLE_LOG("VdGrp=%d, bRecvFlowEnd:%d bGrpQuitWait:%d, s_ThreadExit:%d\n",
                   VdGrp, pstVdecCtx->bRecvFlowEnd, pstVdecCtx->bGrpQuitWait[VdGrp], s_ThreadExit);
    }

    if (pstVdGrpAttr->bSdkAutoFramePool == AX_FALSE) {
        for (VdChn = 0; VdChn < AX_DEC_MAX_CHN_NUM; VdChn++) {
            if (pstFuncArgs->bChnEnable[VdChn] == AX_FALSE) continue;

            sRet = AXCL_VDEC_DetachPool(VD_GRP_API_MGR_ID(VdGrp), VdChn);
            if (sRet) {
                SAMPLE_CRIT_LOG("VdGrp=%d, VdChn=%d, PoolId[VdChn]:%d, AXCL_VDEC_DetachPool FAILED! ret:0x%x %s",
                                VdGrp, VdChn, pstFuncArgs->PoolId[VdChn], sRet, AX_VdecRetStr(sRet));
            }
        }
    }

    if (s_ThreadExit == 0) {
        s_ThreadExit += 1;
        SAMPLE_LOG("++s_ThreadExit:%d\n", s_ThreadExit);
    }

    SAMPLE_LOG("VdGrp=%d, s_ThreadExit:%d AX_VDEC_DestroyGrp enter ++++", VdGrp, s_ThreadExit);
    while (1) {
        sRet = AXCL_VDEC_DestroyGrp(VD_GRP_API_MGR_ID(VdGrp));
        if (sRet == AX_ERR_VDEC_BUSY) {
            SAMPLE_WARN_LOG("VdGrp=%d, AXCL_VDEC_DestroyGrp FAILED! ret:0x%x %s",
                           VdGrp, sRet, AX_VdecRetStr(sRet));
            usleep(10000);

            continue;
        }

        if (sRet == AX_SUCCESS) {
            pstVdecCtx->GrpStatus[VdGrp] = AX_VDEC_GRP_DESTROYED;
            if (pstCmd->bGrpUnExist) {
                AX_VIDEO_FRAME_INFO_T stFrame;
                AX_S32 s32GrpUnExistCnt= 0;
                for (int i = 0; i < 5; i++) {
                    for (VdChn = 0; VdChn < AX_DEC_MAX_CHN_NUM; VdChn++) {
                        if (pstFuncArgs->bChnEnable[VdChn] == AX_FALSE) continue;
                        s32Ret = AXCL_VDEC_GetChnFrame(VD_GRP_API_MGR_ID(VdGrp), VdChn, &stFrame, -1);
                        if (s32Ret == AX_ERR_VDEC_UNEXIST) {
                            s32GrpUnExistCnt++;
                            SAMPLE_LOG("VdGrp=%d, VdChn:%d, AXCL_VDEC_GetChnFrame ret %s",
                                         VdGrp, VdChn, AX_VdecRetStr(s32Ret));
                        } else if (s32Ret == AX_ERR_VDEC_FLOW_END) {
                            SAMPLE_LOG("VdGrp=%d, VdChn:%d, AXCL_VDEC_GetChnFrame ret %s",
                                         VdGrp, VdChn, AX_VdecRetStr(s32Ret));
                        } else {
                            SAMPLE_CRIT_LOG("VdGrp=%d, VdChn:%d, AXCL_VDEC_GetChnFrame FAILED! ret:0x%x %s",
                                            VdGrp, VdChn, s32Ret, AX_VdecRetStr(s32Ret));
                        }
                    }
                }
                SAMPLE_INFO_LOG("VdGrp=%d, s32GrpUnExistCnt=%d", VdGrp, s32GrpUnExistCnt);
            }
        } else {
            SAMPLE_CRIT_LOG("VdGrp=%d, AXCL_VDEC_DestroyGrp FAILED! ret:0x%x %s",
                            VdGrp, sRet, AX_VdecRetStr(sRet));
            ErrorCode |= sRet;
        }

        break;
    }

    SAMPLE_LOG("VdGrp=%d, AXCL_VDEC_DestroyGrp exit ---- ret:0x%x", VdGrp, sRet);

    if (pstVdGrpAttr->bSdkAutoFramePool == AX_FALSE) {
        for (VdChn = 0; VdChn < AX_DEC_MAX_CHN_NUM; VdChn++) {
            if (pstFuncArgs->bChnEnable[VdChn] == AX_FALSE) continue;

            if (pstFuncArgs->PoolId[VdChn] == AX_INVALID_POOLID) continue;

            sRet = AXCL_POOL_DestroyPool(pstFuncArgs->PoolId[VdChn]);
            if (sRet) {
                SAMPLE_CRIT_LOG("VdGrp=%d, VdChn=%d, PoolId[VdChn]:%d, AXCL_POOL_DestroyPool FAILED! ret:0x%x %s",
                                VdGrp, VdChn, pstFuncArgs->PoolId[VdChn], sRet, AX_VdecRetStr(sRet));
            }
        }
    }

    if (pstCmd->bFfmpegEnable) {
        if ((pstCmd->enInputMode != AX_VDEC_INPUT_MODE_STREAM)
                && ((pstCmd->enDecType == PT_H264) || (pstCmd->enDecType == PT_H265))) {
            s32Ret = SampleVdecFfmpegDeinit(&pstVdecCtx->stFfmpeg, pstBitStreamInfo->VdGrp);
            if (s32Ret != AX_SUCCESS) {
                SAMPLE_CRIT_LOG("VdGrp=%d, SampleVdecFfmpegDeinit FAILED! ret:0x%x\n",
                            VdGrp, s32Ret);
                goto ERR_RET_FCLOSE;
            }
        }
    }

    axclrtFreeHost(pHostVirAddr);
    if (stStrmBuf.pu8Addr != NULL) {
        sRet = AXCL_SYS_MemFree(stStrmBuf.u64PhyAddr, stStrmBuf.pu8Addr);
        if(sRet != AX_SUCCESS) {
            SAMPLE_CRIT_LOG("AX_SYS_MemFree streamPhyAddr FAILED! sRet:0x%x\n", sRet);
        }
    }

    if (pstChnSet) {
        free(pstChnSet);
        pstChnSet = NULL;
    }

    for (VdChn = 0; VdChn < AX_DEC_MAX_CHN_NUM; VdChn++) {
        pstFuncArgs->pstVdChnAttr[VdChn] = NULL;
    }

    if (fInput) {
        fclose(fInput);
        fInput = NULL;
    }

    /* step04: destory thread context */
    axclrtDestroyContext(context);

    return NULL;


ERR_RET_DISABLE_USER_PIC:
    VdecUserPicDestroy(VdGrp, pstFuncArgs->tUsrPicArgs.pstVdecUserPic);
ERR_RET_DETACH_POOL:
    if (pstVdGrpAttr->bSdkAutoFramePool == AX_FALSE) {
        for (VdChn = 0; VdChn < AX_DEC_MAX_CHN_NUM; VdChn++) {
            if (pstFuncArgs->bChnEnable[VdChn] == AX_FALSE) continue;

            sRet = AXCL_VDEC_DetachPool(VD_GRP_API_MGR_ID(VdGrp), VdChn);
            if (sRet) {
                SAMPLE_CRIT_LOG("VdGrp=%d, VdChn=%d, PoolId[VdChn]:%d, AXCL_VDEC_DetachPool FAILED! ret:0x%x %s",
                                VdGrp, VdChn, pstFuncArgs->PoolId[VdChn], sRet, AX_VdecRetStr(sRet));
            }
        }
    }
ERR_RET_CHNATTR_FREE:
    if (pstChnSet) {
        free(pstChnSet);
        pstChnSet = NULL;
    }

    for (VdChn = 0; VdChn < AX_DEC_MAX_CHN_NUM; VdChn++) {
        pstFuncArgs->pstVdChnAttr[VdChn] = NULL;
    }
ERR_RET_DESTROY_GRP:
    while (1) {
        sRet = AXCL_VDEC_DestroyGrp(VD_GRP_API_MGR_ID(VdGrp));
        if (sRet == AX_ERR_VDEC_BUSY) {
            SAMPLE_WARN_LOG("VdGrp=%d, AXCL_VDEC_DestroyGrp FAILED! ret:0x%x %s",
                           VdGrp, sRet, AX_VdecRetStr(sRet));
            usleep(10000);
            continue;
        }

        if (sRet == AX_SUCCESS) {
            pstVdecCtx->GrpStatus[VdGrp] = AX_VDEC_GRP_DESTROYED;
        } else {
            SAMPLE_CRIT_LOG("VdGrp=%d, AXCL_VDEC_DestroyGrp FAILED! ret:0x%x %s",
                            VdGrp, sRet, AX_VdecRetStr(sRet));
            ErrorCode |= sRet;
        }

        break;
    }

    if ((bCreateUserPool) && pstVdGrpAttr->bSdkAutoFramePool == AX_FALSE) {
        for (VdChn = 0; VdChn < AX_DEC_MAX_CHN_NUM; VdChn++) {
            if (pstFuncArgs->bChnEnable[VdChn] == AX_FALSE) continue;

            if (pstFuncArgs->PoolId[VdChn] == AX_INVALID_POOLID) continue;

            sRet = AXCL_POOL_DestroyPool(pstFuncArgs->PoolId[VdChn]);
            if (sRet) {
                SAMPLE_CRIT_LOG("VdGrp=%d, VdChn=%d, PoolId[VdChn]:%d, AXCL_POOL_DestroyPool FAILED! ret:0x%x %s",
                                VdGrp, VdChn, pstFuncArgs->PoolId[VdChn], sRet, AX_VdecRetStr(sRet));
            }
        }
    }
ERR_RET_FREE_STREAM:
    if (stStrmBuf.pu8Addr != NULL) {
        sRet = AXCL_SYS_MemFree(stStrmBuf.u64PhyAddr, stStrmBuf.pu8Addr);
        if(sRet != AX_SUCCESS) {
            SAMPLE_CRIT_LOG("AX_SYS_MemFree streamPhyAddr FAILED! sRet:0x%x\n", sRet);
        }
    }
ERR_RET_FFMPEG_DEINIT:
    if (pstCmd->bFfmpegEnable) {
        if ((pstCmd->enInputMode !=AX_VDEC_INPUT_MODE_STREAM)
                && ((pstCmd->enDecType == PT_H264) || (pstCmd->enDecType == PT_H265))) {
            s32Ret = SampleVdecFfmpegDeinit(&pstVdecCtx->stFfmpeg, pstBitStreamInfo->VdGrp);
            if (s32Ret != AX_SUCCESS) {
                SAMPLE_CRIT_LOG("VdGrp=%d, SampleVdecFfmpegDeinit FAILED! ret:0x%x\n",
                            VdGrp, s32Ret);
            }
        }
    }
    if (pHostVirAddr)
        axclrtFreeHost(pHostVirAddr);

ERR_RET_FCLOSE:
    if (fInput) {
        fclose(fInput);
        fInput = NULL;
    }

    /* step04: destory thread context */
    axclrtDestroyContext(context);

    return NULL;
}

static int _VdecRecvProcess(SAMPLE_VDEC_CONTEXT_T *pstVdecCtx, SAMPLE_VDEC_GRP_SET_ARGS_T *pstGrpSetArgs)
{
    SAMPLE_VDEC_CMD_PARAM_T *pstCmd = NULL;
    AX_S32 s32Ret = 0;
    int gi;
    AX_VDEC_GRP VdGrp = 0;

    SAMPLE_VDEC_GRP_CHN_RECV_ARGS_T *pStRecvArgs = NULL;
    SAMPLE_VDEC_GRP_CHN_RECV_ARGS_T *pStGrpRecvArgs = NULL;
    SAMPLE_VDEC_ARGS_T *pstFuncArgs = NULL;

    if (pstVdecCtx == NULL) {
        SAMPLE_CRIT_LOG("pstSampleCtx == NULL\n");
        return -1;
    }

    pstCmd = &pstVdecCtx->tCmdParam;

    if (pstCmd->enSelectMode == AX_VDEC_SELECT_MODE_DISABLE) {
        AX_U32 tmp_size = sizeof(SAMPLE_VDEC_GRP_CHN_RECV_ARGS_T) * pstCmd->uGrpCount * AX_DEC_MAX_CHN_NUM;

        pStRecvArgs = (SAMPLE_VDEC_GRP_CHN_RECV_ARGS_T *)calloc(1, tmp_size);
        if (NULL == pStRecvArgs) {
            SAMPLE_CRIT_LOG("calloc FAILED! size:0x%x\n", tmp_size);
            s32Ret = AX_ERR_VDEC_NOMEM;
            goto ERR_RET;
        }

        SAMPLE_LOG_N("tmp_size:%d, pstCmd->uGrpCount:%d", tmp_size, pstCmd->uGrpCount);

        for (gi = 0; gi < pstCmd->uGrpCount; gi++) {

            VdGrp = gi;
            pstFuncArgs = &pstGrpSetArgs->stVdecGrpArgs[gi];

            for (int VdChn = 0; VdChn < AX_DEC_MAX_CHN_NUM; VdChn++) {
                if (pstFuncArgs->bChnEnable[VdChn] == AX_FALSE) continue;

                pStGrpRecvArgs = pStRecvArgs + (gi * AX_DEC_MAX_CHN_NUM) + VdChn;
                pStGrpRecvArgs->VdGrp = VdGrp;
                pStGrpRecvArgs->pstCtx = pstFuncArgs->pstCtx;
                pStGrpRecvArgs->VdChn = VdChn;
                pStGrpRecvArgs->device_id = pstVdecCtx->device_id;

                SAMPLE_LOG_N("VdGrp=%d, VdChn=%d", VdGrp, VdChn);
                /* create thread for get chn frame */
                if (pthread_create(&pstVdecCtx->GrpChnRecvTid[VdGrp][VdChn],
                                    NULL, __VdecGrpChnRecvThread, pStGrpRecvArgs) != 0) {
                    SAMPLE_CRIT_LOG("VdGrp=%d, VdChn=%d, pthread_create __VdecGrpChnRecvThread FAILED!\n",
                                    VdGrp, VdChn);
                    goto ERR_RET_FREE_RECV_ARGS;
                }

                SAMPLE_LOG_N("VdGrp=%d, VdChn=%d, pthread_create __VdecGrpChnRecvThread done! pthread_id:0x%lx!\n",
                            VdGrp, VdChn, pstVdecCtx->GrpChnRecvTid[VdGrp][VdChn]);
            }
        }

        if (pStRecvArgs != NULL) {
            free(pStRecvArgs);
            pStRecvArgs = NULL;
        }

    }
    else if (pstCmd->enSelectMode == AX_VDEC_SELECT_MODE_PRIVATE) {
        pstGrpSetArgs->device_id = pstVdecCtx->device_id;
        if (pthread_create(&pstVdecCtx->RecvTid, NULL, _VdecRecvThread, pstGrpSetArgs) != 0) {
            SAMPLE_CRIT_LOG("pthread_create VdecRecvThread FAILED!\n");
            s32Ret = AX_ERR_VDEC_RUN_ERROR;
            goto ERR_RET;
        }
    }
    else {
        SAMPLE_CRIT_LOG("VdGrp=%d, Unsupport enSelectMode:%d!\n",
                        VdGrp, pstCmd->enSelectMode);
        s32Ret = AX_ERR_VDEC_NOT_SUPPORT;
    }

    return s32Ret;

ERR_RET_FREE_RECV_ARGS:
    if (pStRecvArgs != NULL) {
        free(pStRecvArgs);
        pStRecvArgs = NULL;
    }
ERR_RET:
    return s32Ret;
}

static int _VdecTestMain(SAMPLE_VDEC_CONTEXT_T *pstVdecCtx, SAMPLE_VDEC_GRP_SET_ARGS_T *pstGrpSetArgs)
{
    int i;
    int ret = 0;
    AX_U32 GrpNum = 0;
    AX_S32 s32Ret = 0;
    SAMPLE_VDEC_CMD_PARAM_T *pstCmd = NULL;
    SAMPLE_VDEC_ARGS_T *pstFuncArgs = NULL;
    AX_VDEC_GRP VdGrp = 0;

    if (pstVdecCtx == NULL) {
        SAMPLE_CRIT_LOG("pstSampleCtx == NULL\n");
        return -1;
    }

    pstCmd = &pstVdecCtx->tCmdParam;
    SAMPLE_LOG("_VdecTestMain pstCmd->sTestCnt:%d, pstCmd->sLoopDecNum:%d", pstCmd->sTestCnt, pstCmd->sLoopDecNum);
    bCreatGrpEx = pstCmd->bCreatGrpEx;
    if (pstGrpSetArgs->stVdecGrpArgs[0].pstCtx) {
        SAMPLE_LOG_N("stVdecGrpArgs[0].pstCtx->tCmdParam.sTestCnt:%d, ->sLoopDecNum:%d",
                    pstGrpSetArgs->stVdecGrpArgs[0].pstCtx->tCmdParam.sTestCnt,
                    pstGrpSetArgs->stVdecGrpArgs[0].pstCtx->tCmdParam.sLoopDecNum);
    }

    if (AX_VDEC_RECV_MODE_AFTER_SEND != pstCmd->enSelectMode) {
        s32Ret = _VdecRecvProcess(pstVdecCtx, pstGrpSetArgs);
        if (s32Ret != AX_SUCCESS) {
            SAMPLE_CRIT_LOG("AX_SUCCESS FAILED! ret:0x%x\n", s32Ret);
            goto ERR_RET;
        }
    }

    for (i = 0; i < pstCmd->uGrpCount; i++) {
        GrpNum = i;
        pstFuncArgs = &pstGrpSetArgs->stVdecGrpArgs[i];
        ret = pthread_create(&pstVdecCtx->GrpTid[i], NULL, _VdecGroupThreadMain,
                             (void *)pstFuncArgs);
        if (ret) {
            SAMPLE_CRIT_LOG("VdGrp=%d, pthread_create _VdecGroupThreadMain FAILED! ret:%d, %s, \n",
                            i, ret, strerror(ret));
            s32Ret = AX_ERR_VDEC_UNKNOWN;
            goto ERR_RET_CANCEL_PTHREAD_RECV;
        }
        SAMPLE_LOG("VdGrp=%d, pthread_create _VdecGroupThread done! pthread_id:0x%lx \n",
                   i, pstVdecCtx->GrpTid[i]);
    }

    for (i = 0; i < pstCmd->uGrpCount; i++) {
        SAMPLE_LOG("VdGrp=%d, before pthread_join _VdecGroupThread pthread_id:0x%lx, PID:%d\n",
                   i, pstVdecCtx->GrpTid[i], pstVdecCtx->GrpPID[i]);

        ret = pthread_join(pstVdecCtx->GrpTid[i], NULL);
        if (ret) {
            SAMPLE_CRIT_LOG("VdGrp=%d, pthread_join FAILED! ret:%d, %s, \n",
                            i, ret, strerror(ret));
            s32Ret = AX_ERR_VDEC_UNKNOWN;
            goto ERR_RET_CANCEL_PTHREAD_RECV;
        }
        SAMPLE_LOG("VdGrp=%d, pthread_join _VdecGroupThread done! pthread_id:0x%lx, PID:%d \n",
                   i, pstVdecCtx->GrpTid[i], pstVdecCtx->GrpPID[i]);
        pstVdecCtx->GrpTid[i] = 0;
    }


    if (pstCmd->enSelectMode == AX_VDEC_SELECT_MODE_DISABLE) {
        for (int gi = 0; gi < pstCmd->uGrpCount; gi++) {
            for (int VdChn = 0; VdChn < AX_DEC_MAX_CHN_NUM; VdChn++) {
                if (pstFuncArgs->bChnEnable[VdChn] == AX_FALSE) continue;

                SAMPLE_LOG("VdGrp=%d, VdChn=%d, before pthread_join pthread_id:0x%lx",
                            VdGrp, VdChn, pstVdecCtx->GrpChnRecvTid[VdGrp][VdChn]);

                ret = pthread_join(pstVdecCtx->GrpChnRecvTid[VdGrp][VdChn], NULL);
                if (ret) {
                    SAMPLE_CRIT_LOG("VdGrp=%d, VdChn=%d, pthread_join __VdecGrpChnRecvThread FAILED! ret:%d, %s, \n",
                                    VdGrp, VdChn, ret, strerror(ret));
                    s32Ret = AX_ERR_VDEC_UNKNOWN;
                    goto ERR_RET_CANCEL_PTHREAD_DEC;
                } else {
                    SAMPLE_LOG("VdGrp=%d, VdChn=%d, pthread_join __VdecGrpChnRecvThread done! pthread_id:0x%lx",
                                VdGrp, VdChn, pstVdecCtx->GrpChnRecvTid[VdGrp][VdChn]);
                    pstVdecCtx->GrpChnRecvTid[VdGrp][VdChn] = 0;
                }
            }
        }
    }
    else if (pstCmd->enSelectMode == AX_VDEC_SELECT_MODE_PRIVATE) {
        /* At least one group was successfully created and configured */
        if (s_groupSuccess) {
            ret = pthread_join(pstVdecCtx->RecvTid, NULL);
            if (ret) {
                SAMPLE_CRIT_LOG("pthread_join FAILED! ret:%d, %s, \n",
                                ret, strerror(ret));
                s32Ret = AX_ERR_VDEC_UNKNOWN;
                goto ERR_RET;
            }
        } else {
            /* If no group is successfully created, cancel the thread directly */
            ret = pthread_cancel(pstVdecCtx->RecvTid);
            if (ret) {
                SAMPLE_LOG("pthread has been released! ret:%d, %s, \n",
                           ret, strerror(ret));
                s32Ret = AX_ERR_VDEC_UNKNOWN;
                goto ERR_RET;
            }
        }
    }

    return s32Ret;

ERR_RET_CANCEL_PTHREAD_RECV:
    if (pstVdecCtx->tCmdParam.enSelectMode == AX_VDEC_SELECT_MODE_DISABLE) {
        for (int gi = 0; gi < pstCmd->uGrpCount; gi++) {
            for (int VdChn = 0; VdChn < AX_DEC_MAX_CHN_NUM; VdChn++) {
                if (pstFuncArgs->bChnEnable[VdChn] == AX_FALSE) continue;

                VdGrp = gi;
                ret = pthread_cancel(pstVdecCtx->GrpChnRecvTid[VdGrp][VdChn]);
                if (ret) {
                    SAMPLE_CRIT_LOG("VdGrp=%d, VdChn=%d, pthread_cancel __VdecGrpChnRecvThread FAILED! ret:%d, %s, \n",
                                    VdGrp, VdChn, ret, strerror(ret));
                    s32Ret = AX_ERR_VDEC_UNKNOWN;
                    goto ERR_RET;
                }

                ret = pthread_join(pstVdecCtx->GrpChnRecvTid[VdGrp][VdChn], NULL);
                if (ret) {
                    SAMPLE_CRIT_LOG("VdGrp=%d, VdChn=%d, pthread_join __VdecGrpChnRecvThread FAILED! ret:%d, %s, \n",
                                    VdGrp, VdChn, ret, strerror(ret));
                    s32Ret = AX_ERR_VDEC_UNKNOWN;
                    goto ERR_RET;
                }
                SAMPLE_LOG("VdGrp=%d, VdChn=%d, pthread_join __VdecGrpChnRecvThread done! pthread_id:0x%lx",
                            VdGrp, VdChn, pstVdecCtx->GrpChnRecvTid[VdGrp][VdChn]);
                pstVdecCtx->GrpChnRecvTid[VdGrp][VdChn] = 0;
            }
        }
    }
    else if (pstVdecCtx->tCmdParam.enSelectMode == AX_VDEC_SELECT_MODE_PRIVATE) {
        ret = pthread_cancel(pstVdecCtx->RecvTid);
        if (ret) {
            SAMPLE_CRIT_LOG("pthread_cancel FAILED! ret:%d, %s, \n",
                            ret, strerror(ret));
            s32Ret = AX_ERR_VDEC_UNKNOWN;
            goto ERR_RET;
        }

        ret = pthread_join(pstVdecCtx->RecvTid, NULL);
        if (ret) {
            SAMPLE_CRIT_LOG("pthread_join FAILED! ret:%d, %s, \n",
                            ret, strerror(ret));
            s32Ret = AX_ERR_VDEC_UNKNOWN;
            goto ERR_RET;
        }
    }

ERR_RET_CANCEL_PTHREAD_DEC:
    for (i = 0; i < GrpNum; i++) {
        ret = pthread_cancel(pstVdecCtx->GrpTid[i]);
        if (ret) {
            SAMPLE_CRIT_LOG("VdGrp=%d, pthread_cancel FAILED! ret:%d, %s, \n",
                            i, ret, strerror(ret));
            goto ERR_RET;
        }

        ret = pthread_join(pstVdecCtx->GrpTid[i], NULL);
        if (ret) {
            SAMPLE_CRIT_LOG("VdGrp=%d, pthread_join FAILED! ret:%d, %s, \n",
                            i, ret, strerror(ret));
            goto ERR_RET;
        }
    }
ERR_RET:
    return s32Ret;
}

int Sample_VdecTestBenchMain(SAMPLE_VDEC_CONTEXT_T **ppstSampleCtx)
{
    int i;
    AX_S32 s32Ret = 0;
    SAMPLE_VDEC_ARGS_T *pstVdecGrpArgs = NULL;
    SAMPLE_VDEC_CMD_PARAM_T *pstCmd = NULL;
    AX_U32 tmp_size = 0;
    AX_U32 uStreamCount = 0;
    SAMPLE_VDEC_CONTEXT_T *pstVdecCtx = NULL;
    AX_VDEC_GRP uStartGrpId = 0;

    if (ppstSampleCtx == NULL) {
        SAMPLE_CRIT_LOG("ppstSampleCtx == NULL\n");
        return -1;
    }

    pstVdecCtx = *ppstSampleCtx;
    if (pstVdecCtx == NULL) {
        SAMPLE_CRIT_LOG("pstSampleCtx == NULL\n");
        return -1;
    }

    pstCmd = &pstVdecCtx->tCmdParam;

    uStreamCount = pstCmd->uStreamCount;
    uStartGrpId = pstCmd->uStartGrpId;
    memset(&stGrpSetArgs, 0x0, sizeof(SAMPLE_VDEC_GRP_SET_ARGS_T));
    for (i = 0; i < pstCmd->uGrpCount; i++) {
        pstVdecGrpArgs = &stGrpSetArgs.stVdecGrpArgs[i];
        pstVdecGrpArgs->VdGrp = i + uStartGrpId;

        if (i == 0) {
            pstVdecGrpArgs->pstCtx = pstVdecCtx;
        } else {
            tmp_size = sizeof(SAMPLE_VDEC_CONTEXT_T);
            pstVdecGrpArgs->pstCtx = (SAMPLE_VDEC_CONTEXT_T *)calloc(1, tmp_size);
            if (NULL == pstVdecGrpArgs->pstCtx) {
                SAMPLE_CRIT_LOG("calloc FAILED! size:0x%x\n", tmp_size);
                s32Ret = AX_ERR_VDEC_NOMEM;
                goto ERR_RET_ARGS;
            }

            if (uStreamCount > 0) {
                s32Ret = utVdecDefaultParamsSet(&pstVdecGrpArgs->pstCtx->tCmdParam);
                if (s32Ret) {
                    SAMPLE_CRIT_LOG("VdecDefaultParamsSet FAILED! ret:0x%x\n", s32Ret);
                    goto ERR_RET_ARGS;
                }

                uStreamCount--;
            } else {
                memcpy(pstVdecGrpArgs->pstCtx, pstVdecCtx, sizeof(SAMPLE_VDEC_CONTEXT_T));
                pstVdecGrpArgs->pstCtx->tCmdParam.tChnCfg[0].pOutputFilePath = NULL;
                pstVdecGrpArgs->pstCtx->tCmdParam.pTbCfgFilePath = NULL;
            }
        }

        if (pstCmd->pGrpCmdlFile[i] == NULL) {
            pstCmd->pGrpCmdlFile[i] = pstCmd->pGrpCmdlFile[0];
        }

        s32Ret = utVdecParseStreamCfg(pstCmd->pGrpCmdlFile[i], pstVdecGrpArgs->pstCtx, i);
        if (s32Ret) {
            SAMPLE_CRIT_LOG("VdGrp=%d, VdecParseStreamCfg FAILED! ret:%d, \n", i, s32Ret);
            goto ERR_RET_ARGS;
        }

        s32Ret = utOpenTestHooks(pstVdecGrpArgs->pstCtx);
        if (s32Ret) {
            SAMPLE_CRIT_LOG("VdGrp=%d, OpenTestHooks FAILED! ret:%d, \n", i, s32Ret);
            goto ERR_RET_ARGS;
        }

        pstVdecGrpArgs->tVdGrpAttr.enCodecType = pstVdecGrpArgs->pstCtx->tCmdParam.enDecType;

        for (int ci = 0; ci < AX_VDEC_MAX_CHN_NUM; ci++) {
            pstVdecGrpArgs->bChnEnable[ci] = pstCmd->tChnCfg[ci].bChnEnable;
            // SAMPLE_LOG("VdGrp=%d, VdChn=%d, bChnEnable=%d", i, ci, pstCmd->tChnCfg[ci].bChnEnable);
        }
    }

    SAMPLE_LOG_N("pstVdecCtx:%p, .stVdecGrpArgs[0].pstCtx:%p, "
                "pstVdecGrpArgs->tVdGrpAttr.enCodecType:%d",
                pstVdecCtx, stGrpSetArgs.stVdecGrpArgs[0].pstCtx,
                pstVdecGrpArgs->tVdGrpAttr.enCodecType);

    if (s_ThreadExit) {
        SAMPLE_LOG("s_ThreadExit:%d, so goto exit", s_ThreadExit);
        goto ERR_RET_ARGS;
    }

    s32Ret = _VdecTestMain(pstVdecCtx, &stGrpSetArgs);
    if (s32Ret) {
        SAMPLE_CRIT_LOG("VdGrp=%d, _VdecTestMain FAILED! ret:0x%x, %s\n",
                        i, s32Ret, AX_VdecRetStr(s32Ret));
        goto ERR_RET_ARGS;
    }

    return s32Ret;

ERR_RET_ARGS:
    uStreamCount = pstCmd->uGrpCount;
    for (i = 0; i < uStreamCount; i++) {
        pstVdecGrpArgs = &stGrpSetArgs.stVdecGrpArgs[i];

        if (NULL != pstVdecGrpArgs) {
            pstVdecCtx = pstVdecGrpArgs->pstCtx;
            if (pstVdecCtx != NULL) {
                if (pstVdecCtx->bArgvAlloc == AX_TRUE) {
                    if (pstVdecCtx->argv != NULL) {
                        if (pstVdecCtx->argv[0] != NULL) {
                            free(pstVdecCtx->argv[0]);
                            pstVdecCtx->argv[0] = NULL;
                        }

                        free(pstVdecCtx->argv);
                        pstVdecCtx->argv = NULL;
                    }

                    pstVdecCtx->bArgvAlloc = AX_FALSE;
                }

                free(pstVdecCtx);
                pstVdecCtx = NULL;
            }

            pstVdecGrpArgs = NULL;
        }
    }


    *ppstSampleCtx = NULL;
    return s32Ret;
}

AX_S32 Sample_VdecTestBenchDeinit(SAMPLE_VDEC_CONTEXT_T *pstVdecCtx)
{
    AX_S32 s32Ret = 0;
    SAMPLE_VDEC_CMD_PARAM_T *pstCmd = NULL;
    int i = 0;
    SAMPLE_VDEC_ARGS_T *pstVdecGrpArgs = NULL;
    SAMPLE_VDEC_CONTEXT_T *pstCtx = NULL;
    AX_U32 uGrpCount = 0;

    pstCmd = &pstVdecCtx->tCmdParam;

    for (int ci = 0; ci < AX_VDEC_MAX_CHN_NUM; ci++) {
        if (!pstCmd->tChnCfg[ci].bChnEnable) continue;
        if (NULL != pstCmd->tChnCfg[ci].pOutputFilePath) {
            SAMPLE_LOG_NN("pstCmd->tChnCfg[%d].pOutputFilePath:%p",
                          ci, pstCmd->tChnCfg[ci].pOutputFilePath);

            free(pstCmd->tChnCfg[ci].pOutputFilePath);
            pstCmd->tChnCfg[ci].pOutputFilePath = NULL;
        }

        if (NULL != pstCmd->tChnCfg[ci].pUsrPicFilePath) {
            SAMPLE_LOG_NN("pstCmd->tChnCfg[%d].pUsrPicFilePath:%p",
                          ci, pstCmd->tChnCfg[ci].pUsrPicFilePath);
            free(pstCmd->tChnCfg[ci].pUsrPicFilePath);
            pstCmd->tChnCfg[ci].pUsrPicFilePath = NULL;
        }
    }

    if (NULL != pstCmd->pInputFilePath) {
        SAMPLE_LOG_N("pstCmd->pInputFilePath:%p", pstCmd->pInputFilePath);
        free(pstCmd->pInputFilePath);
        pstCmd->pInputFilePath = NULL;
    }

    if (NULL != pstCmd->pNewInputFilePath) {
        SAMPLE_LOG_N("pstCmd->pNewInputFilePath:%p", pstCmd->pNewInputFilePath);
        free(pstCmd->pNewInputFilePath);
        pstCmd->pNewInputFilePath = NULL;
    }


    uGrpCount = pstCmd->uGrpCount;
    for (i = 0; i < uGrpCount; i++) {
        pstVdecGrpArgs = &stGrpSetArgs.stVdecGrpArgs[i];

        if (NULL != pstVdecGrpArgs) {
            pstCtx = pstVdecGrpArgs->pstCtx;
            if (pstCtx != NULL) {
                if (pstCtx->bArgvAlloc == AX_TRUE) {
                    if (pstCtx->argv != NULL) {
                        if (pstCtx->argv[0] != NULL) {
                            free(pstCtx->argv[0]);
                            pstCtx->argv[0] = NULL;
                        }

                        free(pstCtx->argv);
                        pstCtx->argv = NULL;
                    }

                    pstCtx->bArgvAlloc = AX_FALSE;
                }

                free(pstCtx);
                pstVdecGrpArgs->pstCtx = NULL;
            }

            pstVdecGrpArgs = NULL;
        }
    }

    return s32Ret;
}

AX_S32 Sample_VdecTestBenchInit(int argc, char *argv[], SAMPLE_VDEC_CONTEXT_T **ppstVdecCtx)
{
    AX_S32 s32Ret = 0;
    //pid_t pid, ppid;
    struct timeval current_tv;
    SAMPLE_VDEC_CONTEXT_T *pstVdecCtx = NULL;
    int gi = 0;
    int ci = 0;

    signal(SIGINT, _VdecSigInt); /* ctrl + c */
    signal(SIGQUIT, _VdecSigInt); /* ctrl + \ */
    signal(SIGTSTP, _VdecSigInt); /* ctrl + z */

    gettimeofday(&current_tv, NULL);
    //pid = getpid();
    //ppid = getppid();

    AX_CHAR cPthreadName[16];
    snprintf(cPthreadName, 16, "%s", argv[0]);
    prctl(PR_SET_NAME, cPthreadName);

    SAMPLE_LOG("Start! pid:%d, ppid:%d, date:%s, time:%s, current_tv.tv_sec:%ld",
                  getpid(), getppid(), __DATE__, __TIME__, current_tv.tv_sec);

    AX_CHAR cTmp[1024] = {0};
    AX_CHAR *pcTmp = cTmp;
    int len = 0;
    for (int ii = 0; ii < argc; ii++) {
        len = strlen(argv[ii]);
        strcat(pcTmp, argv[ii]);
        pcTmp += len;
        strcat(pcTmp, " ");
        pcTmp += 1;
    }

    SAMPLE_LOG("cmd:%s \n", cTmp);

    pstVdecCtx = (SAMPLE_VDEC_CONTEXT_T *)calloc(1, sizeof(SAMPLE_VDEC_CONTEXT_T));
    if (NULL == pstVdecCtx) {
        SAMPLE_CRIT_LOG("calloc FAILED! size:0x%lx\n", sizeof(SAMPLE_VDEC_CONTEXT_T));
        return -1;
    }

    SAMPLE_LOG_NN("pstVdecCtx:%p", pstVdecCtx);

    pstVdecCtx->argc = argc;
    pstVdecCtx->argv = argv;

#ifdef AX_VDEC_POOL_REFCNT_TEST
    int bi;
    for (gi = 0; gi < AX_VDEC_MAX_GRP_NUM; gi++) {
        for (bi = 0; bi < SAMPLE_VDEC_REF_BLK_CNT; bi++) {
            pstVdecCtx->blkRef[gi][bi] = AX_INVALID_BLOCKID;
        }
    }
#endif

    for (gi = 0; gi < AX_VDEC_MAX_GRP_NUM; gi++) {
        for (ci = 0; ci < AX_DEC_MAX_CHN_NUM; ci++) {
            s_u64GetFrameNum[gi][ci] = 0;
        }
    }

    *ppstVdecCtx = pstVdecCtx;
    s_pstVdecCtx = pstVdecCtx;

    return s32Ret;
}

int VdecJpegDecodeOneFrame(SAMPLE_VDEC_CMD_PARAM_T *pstCmd)
{
    int res = 0;
    AX_S32 s32Ret = 0;
    AX_S32 sRet = 0;
    AX_U64 outPhyAddrDst = 0;
    AX_VOID *outVirAddrDst = NULL;
    AX_S32 heightAlign = 0;
    AX_S32 frmStride = 0;
    AX_U64 streamPhyAddr = 0;
    AX_VOID *pStreamVirAddr = NULL;
    SAMPLE_STREAM_BUF_T stStreamBuf;
    FILE *fInput = NULL;
    AX_CHAR *streamFile = NULL;
    off_t inputFileSize = 0;

    AX_U32 uBufSize = 0;
    AX_VDEC_DEC_ONE_FRM_T decOneFrmParam;
    AX_U32 uPixBits = 0;
    AX_U32 uStrmSize = 1024 * 1024;
    AX_VOID *pHostVirAddr = NULL;
    AX_VOID *pHostFrmAddr = NULL;
    size_t sReadLen = 0;

    if (NULL == pstCmd) {
        SAMPLE_CRIT_LOG("NULL == pstCmd\n");
        s32Ret = AX_ERR_VDEC_UNKNOWN;
        goto ERR_RET;
    }

    if (pstCmd->enDecType != PT_JPEG) {
        SAMPLE_CRIT_LOG("only support for jpeg\n");
        s32Ret = AX_ERR_VDEC_UNKNOWN;
        goto ERR_RET;
    }

    if (NULL == pstCmd->pInputFilePath) {
        SAMPLE_CRIT_LOG("NULL == streamFile\n");
        s32Ret = AX_ERR_VDEC_UNKNOWN;
        goto ERR_RET;
    }

    streamFile = (AX_CHAR *)pstCmd->pInputFilePath;
    if (NULL == streamFile) {
        SAMPLE_CRIT_LOG("NULL == streamFile\n");
        s32Ret = AX_ERR_VDEC_UNKNOWN;
        goto ERR_RET;
    }

    memset(&decOneFrmParam, 0x0, sizeof(AX_VDEC_DEC_ONE_FRM_T));
    memset(&stStreamBuf, 0x0, sizeof(SAMPLE_STREAM_BUF_T));

    /* Reading input file */
    fInput = fopen(streamFile, "rb");
    if (fInput == NULL) {
        SAMPLE_CRIT_LOG("Unable to open input stream file:%s\n", streamFile);
        s32Ret = AX_ERR_VDEC_UNKNOWN;
        goto ERR_RET;
    }

    /* file i/o pointer to full */
    res = fseek(fInput, 0L, SEEK_END);
    if (res) {
        SAMPLE_CRIT_LOG("fseek FAILED! ret:%d\n", res);
        s32Ret = AX_ERR_VDEC_UNKNOWN;
        goto ERR_RET_CLOSE_IN;
    }

    inputFileSize = ftello(fInput);
    rewind(fInput);

    if (pstCmd->highRes)
        stStreamBuf.uBufSize = STREAM_BUFFER_MAX_SIZE_HIGH_RES;
    else
        stStreamBuf.uBufSize = inputFileSize > STREAM_BUFFER_MAX_SIZE ? STREAM_BUFFER_MAX_SIZE : inputFileSize;

    s32Ret = AXCL_SYS_MemAlloc(&streamPhyAddr, (AX_VOID **)&pStreamVirAddr,
                             stStreamBuf.uBufSize, 0x100, (AX_S8 *)"vdec_input_stream");
    if (s32Ret != AX_SUCCESS) {
        SAMPLE_CRIT_LOG("AX_SYS_MemAlloc FAILED! uBufSize:0x%x ret:0x%x\n",
                        stStreamBuf.uBufSize, s32Ret);
        goto ERR_RET_CLOSE_IN;
    }

    s32Ret = axclrtMallocHost(&pHostVirAddr, uStrmSize);
    if (AXCL_SUCC != s32Ret) {
        SAMPLE_CRIT_LOG("axclrtMallocHost FAILED! uStrmSize:0x%x s32Ret:0x%x\n",
                         uStrmSize, s32Ret);
        goto ERR_RET_FREE_STREAM;
    }

    sReadLen = fread(pHostVirAddr, sizeof(AX_U8), inputFileSize, fInput);

    s32Ret = axclrtMemcpy((AX_VOID *)streamPhyAddr, pHostVirAddr, inputFileSize, AXCL_MEMCPY_HOST_TO_DEVICE);
    if (AXCL_SUCC != s32Ret) {
        SAMPLE_CRIT_LOG("axclrtMemcpy FAILED! ret:0x%x\n", s32Ret);
        goto ERR_RET_HOST_FREE;
    }

    stStreamBuf.tBufAddr.pVirAddr = pStreamVirAddr;
    stStreamBuf.tBufAddr.u64PhyAddr = streamPhyAddr;

    uPixBits = 8;
    frmStride = AX_COMM_ALIGN(pstCmd->tChnCfg[0].u32PicWidth * uPixBits, AX_JDEC_WIDTH_ALIGN * 8) / 8;
    heightAlign = ALIGN_UP(pstCmd->tChnCfg[0].u32PicHeight, 2);
    uBufSize = AX_VDEC_GetPicBufferSize(heightAlign, frmStride, AX_FORMAT_YUV420_PLANAR, NULL,
                                        pstCmd->enDecType);
    s32Ret = AXCL_SYS_MemAlloc(&outPhyAddrDst, (AX_VOID **)&outVirAddrDst,
                             uBufSize, 0x1000, (AX_S8 *)"vdec_output_YUV");
    if (s32Ret != 0) {
        SAMPLE_CRIT_LOG("AX_SYS_MemAlloc FAILED! uBufSize:0x%x ret:0x%x, w:%d, h:%d\n",
                       uBufSize, s32Ret, frmStride, heightAlign);
        goto ERR_RET_HOST_FREE;
    }

    decOneFrmParam.stStream.pu8Addr = stStreamBuf.tBufAddr.pVirAddr;
    decOneFrmParam.stStream.u64PhyAddr = stStreamBuf.tBufAddr.u64PhyAddr;
    decOneFrmParam.stStream.u32StreamPackLen = (AX_U32)sReadLen;

    decOneFrmParam.stFrame.u64VirAddr[0] = (AX_U64)outVirAddrDst;
    decOneFrmParam.stFrame.u64VirAddr[1] = (AX_U64)outVirAddrDst + frmStride * heightAlign;
    decOneFrmParam.stFrame.u64PhyAddr[0] = outPhyAddrDst;
    decOneFrmParam.stFrame.u64PhyAddr[1] = outPhyAddrDst + frmStride * heightAlign;
    decOneFrmParam.enImgFormat = AX_FORMAT_YUV420_SEMIPLANAR;
    decOneFrmParam.enOutputMode = AX_VDEC_OUTPUT_ORIGINAL;
    if (AX_VDEC_OUTPUT_CROP == decOneFrmParam.enOutputMode) {
        decOneFrmParam.stFrame.s16CropX = pstCmd->tChnCfg[0].u32CropX;
        decOneFrmParam.stFrame.s16CropY = pstCmd->tChnCfg[0].u32CropY;
        decOneFrmParam.stFrame.s16CropWidth = pstCmd->tChnCfg[0].u32PicWidth;
        decOneFrmParam.stFrame.s16CropHeight = pstCmd->tChnCfg[0].u32PicHeight;
    }
    s32Ret = AXCL_VDEC_JpegDecodeOneFrame(&decOneFrmParam);
    if (s32Ret != AX_SUCCESS) {
        SAMPLE_CRIT_LOG("AXCL_VDEC_JpegDecodeOneFrame FAILED! ret:0x%x %s\n",
                        s32Ret, AX_VdecRetStr(s32Ret));
        goto ERR_RET_FREE_OUT;
    }

    if (pstCmd->DestMD5) {
        AX_VOID *p_lu = NULL;
        AX_VOID *p_ch = NULL;
        AX_U32 coded_width = decOneFrmParam.stFrame.u32Width;
        AX_U32 coded_height = decOneFrmParam.stFrame.u32Height;
        AX_U32 pic_stride = decOneFrmParam.stFrame.u32PicStride[0];
        AX_U32 coded_width_ch = decOneFrmParam.stFrame.u32Width;
        AX_U32 coded_h_ch = decOneFrmParam.stFrame.u32Height / 2;
        AX_U32 pic_stride_ch = decOneFrmParam.stFrame.u32PicStride[1];
        AX_CHAR md5_str[33];
        char str[33] = "4d39945e7631b5414e062ba096a0f8c4";

        /* need memcpy frame from device to host */
        s32Ret = axclrtMallocHost(&pHostFrmAddr, uBufSize);
        if (AXCL_SUCC != s32Ret) {
            SAMPLE_CRIT_LOG("axclrtMallocHost FAILED! uStrmSize:0x%x s32Ret:0x%x\n",
                            uStrmSize, s32Ret);
            goto ERR_RET_FREE_OUT;
        }
        s32Ret = axclrtMemcpy(pHostFrmAddr, (AX_VOID *)outPhyAddrDst, uBufSize, AXCL_MEMCPY_DEVICE_TO_HOST);
        if (AXCL_SUCC != s32Ret) {
            SAMPLE_CRIT_LOG("axclrtMemcpy FAILED! ret:0x%x\n", s32Ret);
            goto ERR_FRM_HOST_FREE;
        }

        p_lu = pHostFrmAddr;
        p_ch = pHostFrmAddr + frmStride * heightAlign;

        s32Ret = __MD5SumValidOnly(p_lu, p_ch, coded_width, coded_height, coded_width_ch, coded_h_ch,
                                   pic_stride, pic_stride_ch, 0, 0, md5_str);
        if (!s32Ret) {
            if (strcmp(md5_str, str)) {
                SAMPLE_CRIT_LOG("JpegDecodeOneFrame MD5 Check FAILED!\n");
                s32Ret = AX_ERR_VDEC_NOT_MATCH;
                goto ERR_FRM_HOST_FREE;
            }
        }
        SAMPLE_INFO_LOG("MD5Sum Check Successfully! \n\n");
    }

    s32Ret = AXCL_SYS_MemFree(outPhyAddrDst, (AX_VOID *)outVirAddrDst);
    if (s32Ret != AX_SUCCESS) {
        SAMPLE_CRIT_LOG("AXCL_SYS_MemFree outPhyAddrDst FAILED! s32Ret:0x%x\n", s32Ret);
    }

    if (pStreamVirAddr != NULL) {
        sRet = AXCL_SYS_MemFree(streamPhyAddr, pStreamVirAddr);
        if(sRet != AX_SUCCESS) {
            SAMPLE_CRIT_LOG("AX_SYS_MemFree streamPhyAddr FAILED! sRet:0x%x\n", sRet);
        }
    }

    if (s32Ret || sRet) {
        goto ERR_RET;
    }

    if (fInput != NULL) {
        res = fclose(fInput);
        if (res) {
            SAMPLE_CRIT_LOG("fclose FAILED! ret:%d\n", res);
            sRet = AX_ERR_VDEC_UNKNOWN;
        }
        fInput = NULL;
    }

    axclrtFreeHost(pHostVirAddr);
    axclrtFreeHost(pHostFrmAddr);

    g_u64GetFrmTag += 1;
    s_u64GetFrameNum[0][0] = 1;

    return 0;

ERR_FRM_HOST_FREE:
    axclrtFreeHost(pHostFrmAddr);
ERR_RET_FREE_OUT:
    if (outVirAddrDst != NULL) {
        sRet = AXCL_SYS_MemFree(outPhyAddrDst, (AX_VOID *)outVirAddrDst);
        if(sRet != AX_SUCCESS) {
            SAMPLE_CRIT_LOG("AXCL_SYS_MemFree outPhyAddrDst FAILED! sRet:0x%x\n", sRet);
        }
    }
ERR_RET_HOST_FREE:
	axclrtFreeHost(pHostVirAddr);

ERR_RET_FREE_STREAM:
    if (pStreamVirAddr != NULL) {
        sRet = AXCL_SYS_MemFree(streamPhyAddr, pStreamVirAddr);
        if(sRet != AX_SUCCESS) {
            SAMPLE_CRIT_LOG("AX_SYS_MemFree streamPhyAddr FAILED! sRet:0x%x\n", sRet);
        }
    }
ERR_RET_CLOSE_IN:
    if (fInput) {
        res = fclose(fInput);
        fInput = NULL;
    }
ERR_RET:
    SAMPLE_CRIT_LOG("s32Ret:0x%x, sRet:0x%x", s32Ret, sRet);

    return s32Ret;
}

