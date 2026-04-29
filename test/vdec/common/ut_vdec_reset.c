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

UT_VDEC_RESET_FRAME_T s_utResetFrmInfo[AX_VDEC_MAX_GRP_NUM][AX_DEC_MAX_CHN_NUM] = {0};

AX_S32 ut_vdecReset(AX_VDEC_GRP VdGrp, UT_VDEC_RESET_PARAM_T *utVdecResetParam)
{
    AX_S32 resetExeTimes = 0;
    AX_S32 sRet = 0;
    AX_VDEC_RECV_PIC_PARAM_T tRecvParam;
    SAMPLE_VDEC_CMD_PARAM_T *pstCmd = NULL;
    AX_U32 uSendPicNum = 0;
    AX_BOOL bReset = AX_FALSE;
    SAMPLE_VDEC_CONTEXT_T *pstCtx = NULL;

    pstCtx = utVdecResetParam->pstCtx;
    pstCmd = &pstCtx->tCmdParam;
    uSendPicNum = utVdecResetParam->uSendPicNum;
    resetExeTimes = utVdecResetParam->resetExeTimes;

    switch (utVdecResetParam->enResetCase) {
    case AX_VDEC_RESET_CASE_NORMAL:
        if (uSendPicNum % pstCmd->sResetPerFrmNum == 0) bReset = AX_TRUE;
        break;
    case AX_VDEC_RESET_CASE_FORCE_BLOCK:
        if ((pstCmd->sResetTimes > 0) && (resetExeTimes < pstCmd->sResetTimes) &&
            (uSendPicNum % pstCmd->sResetPerFrmNum == 0)) {
            bReset = AX_TRUE;
        }
        break;
    default:
        SAMPLE_CRIT_LOG("VdGrp=%d, invalid case:%d", VdGrp, utVdecResetParam->enResetCase);
        sRet = -1;
        goto ERR_RET;
    }

    if (bReset) {
        SAMPLE_LOG("VdGrp=%d, reset start! reset case:%d, resetExeTimes:%d, sResetTimes:%d ",
                     VdGrp, utVdecResetParam->enResetCase, resetExeTimes, pstCmd->sResetTimes);
        sRet = AXCL_VDEC_StopRecvStream(VD_GRP_API_MGR_ID(VdGrp));
        if (sRet != AX_SUCCESS){
            SAMPLE_CRIT_LOG("VdGrp=%d, AXCL_VDEC_StopRecvStream FAILED! ret:0x%x %s",
                            VdGrp, sRet, AX_VdecRetStr(sRet));
            goto ERR_RET;
        }

        while (1) {
            sRet = AXCL_VDEC_ResetGrp(VD_GRP_API_MGR_ID(VdGrp));
            if (sRet == AX_SUCCESS) break;

            if (sRet == AX_ERR_VDEC_BUSY) {
                usleep(10000);
                continue;
            }

            SAMPLE_CRIT_LOG("VdGrp=%d, AXCL_VDEC_ResetGrp FAILED! ret:0x%x %s",
                            VdGrp, sRet, AX_VdecRetStr(sRet));
            goto ERR_RET;
        }
        memset(&tRecvParam, 0, sizeof(tRecvParam));
        tRecvParam.s32RecvPicNum = pstCmd->sRecvPicNum;
        sRet = AXCL_VDEC_StartRecvStream(VD_GRP_API_MGR_ID(VdGrp), &tRecvParam);
        if (sRet != AX_SUCCESS){
            SAMPLE_CRIT_LOG("VdGrp=%d, AXCL_VDEC_StartRecvStream FAILED! ret:0x%x %s",
                            VdGrp, sRet, AX_VdecRetStr(sRet));
            goto ERR_RET;
        }

        *(utVdecResetParam->bStartRecvFrm) = AX_TRUE;
        resetExeTimes++;
        utVdecResetParam->resetExeTimes = resetExeTimes;
        SAMPLE_LOG("VdGrp=%d, reset end!", VdGrp);
    } else {
        usleep(30 * 1000);
    }

ERR_RET:
    return sRet;
}

AX_VOID ut_vdecResetSaveFrm(AX_VDEC_GRP VdGrp, AX_VDEC_CHN VdChn, AX_VIDEO_FRAME_INFO_T *pstFrameInfo) {
    memcpy(&s_utResetFrmInfo[VdGrp][VdChn].stFrameInfo[s_utResetFrmInfo[VdGrp][VdChn].frmCnt], pstFrameInfo, sizeof(AX_VIDEO_FRAME_INFO_T));
    s_utResetFrmInfo[VdGrp][VdChn].frmCnt++;

    SAMPLE_LOG("VdGrp=%d, VdChn:%d, frmCnt:%d "
                ".u64PhyAddr[0]:0x%llx, BlkId[0]:0x%x, u64SeqNum:%lld\n",
                VdGrp, VdChn, s_utResetFrmInfo[VdGrp][VdChn].frmCnt, pstFrameInfo->stVFrame.u64PhyAddr[0],
                pstFrameInfo->stVFrame.u32BlkId[0], pstFrameInfo->stVFrame.u64SeqNum);
}

AX_VOID ut_vdecResetReleaseFrm(AX_VDEC_GRP VdGrp, AX_VDEC_CHN VdChn) {
    AX_S32 sRet = 0;
    int i = 0;

    AX_VIDEO_FRAME_INFO_T *pstFrameInfo = NULL;

    while(s_utResetFrmInfo[VdGrp][VdChn].frmCnt > 0) {
        pstFrameInfo = &s_utResetFrmInfo[VdGrp][VdChn].stFrameInfo[i];
        SAMPLE_LOG("VdGrp=%d, VdChn:%d, frmCnt:%d "
                    ".u64PhyAddr[0]:0x%llx, BlkId[0]:0x%x, u64SeqNum:%lld\n",
                    VdGrp, VdChn, s_utResetFrmInfo[VdGrp][VdChn].frmCnt, pstFrameInfo->stVFrame.u64PhyAddr[0],
                    pstFrameInfo->stVFrame.u32BlkId[0], pstFrameInfo->stVFrame.u64SeqNum);
        sRet = AXCL_VDEC_ReleaseChnFrame(VD_GRP_API_MGR_ID(VdGrp), VdChn, pstFrameInfo);
        if (sRet) {
            SAMPLE_CRIT_LOG("VdGrp=%d, VdChn:%d, AXCL_VDEC_ReleaseChnFrame FAILED! res:0x%x %s \n"
                            "u64PhyAddr[0]:0x%llX, BlkId[0]:0x%x, BlkId[1]:0x%x\n",
                            VdGrp, VdChn, sRet, AX_VdecRetStr(sRet), pstFrameInfo->stVFrame.u64PhyAddr[0],
                            pstFrameInfo->stVFrame.u32BlkId[0], pstFrameInfo->stVFrame.u32BlkId[1]);
        }

        s_utResetFrmInfo[VdGrp][VdChn].frmCnt--;
        i++;
        usleep(1000 * 1000);
    }

}
