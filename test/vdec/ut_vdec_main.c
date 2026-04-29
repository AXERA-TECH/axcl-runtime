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

#include "ut_common_vdec_cmd.h"
#include "ut_common_vdec_api.h"
#include "axcl.h"

AX_U64 g_u64GetFrmTag = 0;


int main(int argc, char *argv[])
{
    AX_S32 s32Ret = -1;
    AX_S32 sRet = 0;
    int ret = 0;
    //int loop_num = 0;
    AX_S32 device_id = -1;


    SAMPLE_VDEC_CMD_PARAM_T *pstCmd = NULL;
    SAMPLE_VDEC_CONTEXT_T *pstVdecCtx = NULL;

    /* step02: axcl initialize */
    ret = axclInit(NULL);
    if (AX_SUCCESS != ret) {
        SAMPLE_CRIT_LOG("axcl init fail, ret:0x%x", ret);
        return ret;
    }

    axclrtDeviceList lst;
    s32Ret= axclrtGetDeviceList(&lst);
    if (AXCL_SUCC != s32Ret || 0 == lst.num) {
        SAMPLE_CRIT_LOG("no device is connected\n");
        axclFinalize();
        return s32Ret;
    }

    device_id = lst.devices[0];
    SAMPLE_INFO_LOG("device id: %d\n", device_id);

    /* step03: active device */
    ret = axclrtSetDevice(device_id);
    if (AX_SUCCESS != ret) {
        axclrtResetDevice(device_id);
        axclFinalize();
        return ret;
    }

    s32Ret = AXCL_SYS_Init();
    if (AX_SUCCESS != s32Ret) {
        SAMPLE_CRIT_LOG("AX_SYS_Init FAILED! ret:0x%x\n", s32Ret);
        goto ERR_RET;
    }

    s32Ret = Sample_VdecTestBenchInit(argc, argv, &pstVdecCtx);
    if (AX_SUCCESS != s32Ret) {
        SAMPLE_CRIT_LOG("Sample_VdecTestBenchInit FAILED! ret:0x%x\n", s32Ret);
        goto ERR_RET_SYS_DEINIT;
    }
    pstVdecCtx->device_id = device_id;

    pstCmd = &pstVdecCtx->tCmdParam;
    ret = utVdecDefaultParamsSet(pstCmd);
    if (ret) {
        SAMPLE_CRIT_LOG("VdecDefaultParamsSet FAILED! ret:0x%x\n", ret);
        goto ERR_RET_TB_DEINIT;
    }

    ret = utVdecCmdLineParseAndCheck(argc, argv, pstCmd, 0, 0);
    if (ret) {
        SAMPLE_CRIT_LOG("VdecCmdLineParseAndCheck FAILED! ret:0x%x\n", ret);
        goto ERR_RET_TB_DEINIT;
    }

    g_u64GetFrmTag = 0;

    AX_VDEC_MOD_ATTR_T stModAttr;
    stModAttr.enDecModule = pstCmd->enDecModule;
    stModAttr.u32MaxGroupCount = pstCmd->uMaxGrpCnt;
    stModAttr.bVdecMc = pstCmd->McEnable;
    stModAttr.VdecVirtChn = pstCmd->s32VdecVirtChn;

    s32Ret = AXCL_VDEC_Init(&stModAttr);
    if (AX_SUCCESS != s32Ret) {
        SAMPLE_CRIT_LOG("AX_VDEC_Init FAILED! ret:0x%x %s\n", s32Ret, AX_VdecRetStr(s32Ret));
        goto ERR_RET_TB_DEINIT;
    }

    if (pstCmd->bJpegDecOneFrm) {
        s32Ret = VdecJpegDecodeOneFrame(pstCmd);
        if(0 != s32Ret) {
            SAMPLE_CRIT_LOG("Sample_VdecJpegDecodeOneFrame FAILED! ret:0x%x\n", s32Ret);
        }

        goto ERR_RET_VDEC_DEINIT;
    }

    s32Ret = Sample_VdecTestBenchMain(&pstVdecCtx);
    if (AX_SUCCESS != s32Ret) {
        SAMPLE_CRIT_LOG("Sample_VdecTestBenchMain FAILED! ret:0x%x\n", s32Ret);
        goto ERR_RET_VDEC_DEINIT;
    }

ERR_RET_VDEC_DEINIT:
    sRet = AXCL_VDEC_Deinit();
    if (AX_SUCCESS != sRet) {
        SAMPLE_CRIT_LOG("AX_VDEC_Deinit FAILED! ret:0x%x %s\n", sRet, AX_VdecRetStr(sRet));
    }

ERR_RET_TB_DEINIT:
    if (NULL != pstVdecCtx) {
        sRet = Sample_VdecTestBenchDeinit(pstVdecCtx);
        if (AX_SUCCESS != sRet) {
            SAMPLE_CRIT_LOG("_VdecTestBenchDeinit FAILED! ret:0x%x\n", sRet);
        }

        pstVdecCtx = NULL;
    }

    if (g_u64GetFrmTag && !(s32Ret || sRet || ret)) {
        SAMPLE_INFO_LOG("axcl_vdec_ut running g_u64GetFrmTag:%lld status: Decode Finished! \n\n", g_u64GetFrmTag);
    } else {
        SAMPLE_CRIT_LOG("sample_vdec_ut running status: Decode Failed! \n"
                        "please check code, s32Ret:%d, sRet:%d, ret:%d \n\n",
                        s32Ret, sRet, ret);
    }

ERR_RET_SYS_DEINIT:
    sRet = AXCL_SYS_Deinit();
    if (AX_SUCCESS != sRet) {
        SAMPLE_CRIT_LOG("AX_SYS_Deinit FAILED! ret:0x%x\n", sRet);
    }

ERR_RET:
    axclrtResetDevice(device_id);
    axclFinalize();
    return s32Ret || sRet || ret;
}
