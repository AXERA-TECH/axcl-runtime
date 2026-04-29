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


#define AX_VDEC_RESET_MAX_SAVE_FRM_NUM 10

typedef struct axUT_VDEC_RESET_PARAM_T {
    SAMPLE_VDEC_CONTEXT_T *pstCtx;
    AX_U32 uSendPicNum;
    AX_S32 resetExeTimes;
    AX_VDEC_RESET_CASE_E enResetCase;
    AX_BOOL *bStartRecvFrm;
} UT_VDEC_RESET_PARAM_T;

typedef struct axUT_VDEC_RESET_FRAME_T {
    AX_VIDEO_FRAME_INFO_T stFrameInfo[AX_VDEC_RESET_MAX_SAVE_FRM_NUM];
    AX_U32 frmCnt;
} UT_VDEC_RESET_FRAME_T;


AX_S32 ut_vdecReset(AX_VDEC_GRP VdGrp, UT_VDEC_RESET_PARAM_T *utVdecResetParam);
AX_VOID ut_vdecResetSaveFrm(AX_VDEC_GRP VdGrp, AX_VDEC_CHN VdChn, AX_VIDEO_FRAME_INFO_T *pstFrameInfo);
AX_VOID ut_vdecResetReleaseFrm(AX_VDEC_GRP VdGrp, AX_VDEC_CHN VdChn);