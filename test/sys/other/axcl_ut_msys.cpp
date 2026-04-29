/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <chrono>
#include <thread>
#include "axcl.h"
#include "cmdline.h"

#if defined (WINDOWS)
#include "time_period.hpp"
#endif

static int axcl_msys_test_0001();
static int axcl_msys_test_0002();
static int axcl_msys_test_0003();
static int axcl_msys_test_0004();
static int axcl_msys_test_0005();
static int axcl_msys_test_0006();

int main(int argc, char *argv[]) {
    cmdline::parser a;
    a.add<uint32_t>("device", 'd', "device index from 0 to connected device num - 1", false, 0, cmdline::range(0, AXCL_MAX_DEVICE_COUNT - 1));
    a.add<int32_t>("case", 't', "case number", true);
    a.parse_check(argc, argv);
    const uint32_t device_index = a.get<uint32_t>("device");
    const int32_t case_num = a.get<int32_t>("case");

    printf("axcl_ut_msys test begin\n");

    axclError ret;
    ret = axclInit(NULL);
    if (AXCL_SUCC != ret) {
        printf("axcl_ut_msys test failed!\n");
        return 1;
    }

    axclrtDeviceList device_list;
    if (ret = axclrtGetDeviceList(&device_list); AXCL_SUCC != ret || 0 == device_list.num) {
        printf("no device is connected\n");
        axclFinalize();
        return ret;
    }

    if (device_index >= device_list.num) {
        printf("device index %d is out of connected device num %d\n", device_index, device_list.num);
        axclFinalize();
        return 1;
    }

    const int32_t device = device_list.devices[device_index];
    printf("device index: %d, bus number: %d\n", device_index, device);

    ret = axclrtSetDevice(device);
    if (AXCL_SUCC != ret) {
        axclFinalize();
        printf("axcl_ut_msys test failed!\n");
        return 1;
    }

    switch(case_num)
    {
    case 1:
        ret = axcl_msys_test_0001();
        break;
    case 2:
        ret = axcl_msys_test_0002();
        break;
    case 3:
        ret = axcl_msys_test_0003();
        break;
    case 4:
        ret = axcl_msys_test_0004();
        break;
    case 5:
        ret = axcl_msys_test_0005();
        break;
    case 6:
        ret = axcl_msys_test_0006();
        break;
    default:
        printf("axcl_ut_msys test failed!Invalid case number!\n");
        ret = -1;
    }

    if (ret) {
        printf("axcl_ut_msys test failed!\n");
    } else {
        printf("axcl_ut_msys test success end\n");
    }

    axclrtResetDevice(device);
    axclFinalize();

    return 0;
}

/* test AX_SYS_Link/UnLink */
static AX_S32 axcl_msys_test_0001(AX_VOID)
{
    AX_S32 ret = 0;
    AX_MOD_INFO_T srcMod;
    AX_MOD_INFO_T dstMod;
    AX_MOD_INFO_T srcMod2;
    AX_MOD_INFO_T dstMod2;
    AX_MOD_INFO_T tempMod;
    AX_LINK_DEST_T linkDest;

    printf("axcl_msys_test_0001 begin\n");

    memset(&srcMod, 0, sizeof(srcMod));
    memset(&dstMod, 0, sizeof(dstMod));
    memset(&srcMod2, 0, sizeof(srcMod2));
    memset(&dstMod2, 0, sizeof(dstMod2));
    memset(&tempMod, 0, sizeof(tempMod));
    memset(&linkDest, 0, sizeof(linkDest));

    ret = AXCL_SYS_Init();

    if (AXCL_SUCC != ret) {
        printf("AXCL_SYS_Init fail!!ret:0x%x\n", ret);
        return -1;
    } else {
        printf("AXCL_SYS_Init success!\n");
    }

    /* case 1:valid link, should success */
    srcMod.enModId = AX_ID_VIN;
    srcMod.s32GrpId = 0;
    srcMod.s32ChnId = 2;

    dstMod.enModId = AX_ID_IVPS;
    dstMod.s32GrpId = 1;
    dstMod.s32ChnId = 2;

    ret = AXCL_SYS_Link(&srcMod, &dstMod);
    if (ret) {
        printf("AXCL_SYS_Link failed 1,ret:0x%x\n", ret);
        goto error;
    }

    /* case 2:valid unlink, should success */
    ret = AXCL_SYS_UnLink(&srcMod, &dstMod);
    if (ret) {
        printf("AXCL_SYS_UnLink failed 1,ret:0x%x\n", ret);
        goto error;
    }

    /* case 3:invalid link, should fail */
    srcMod.enModId = AX_ID_IVPS;
    srcMod.s32GrpId = 0;
    srcMod.s32ChnId = 2;

    dstMod.enModId = AX_ID_VIN;
    dstMod.s32GrpId = 1;
    dstMod.s32ChnId = 2;

    ret = AXCL_SYS_Link(&srcMod, &dstMod);
    if (!ret) {
        printf("we expect AXCL_SYS_Link return error code but it not!\n");
        goto error;
    }

    /* case 4:invalid unlink, should fail */
    ret = AXCL_SYS_UnLink(&srcMod, &dstMod);
    if (!ret) {
        printf("we expect AXCL_SYS_UnLink return error code but it not!\n");
        goto error;
    }

    /* case 5:two src --> one dst, should fail */
    srcMod.enModId = AX_ID_VIN;
    srcMod.s32GrpId = 0;
    srcMod.s32ChnId = 1;

    srcMod2.enModId = AX_ID_VIN;
    srcMod2.s32GrpId = 0;
    srcMod2.s32ChnId = 2;

    dstMod.enModId = AX_ID_IVPS;
    dstMod.s32GrpId = 1;
    dstMod.s32ChnId = 2;

    ret = AXCL_SYS_Link(&srcMod, &dstMod);
    if (ret) {
        printf("AXCL_SYS_Link failed 2,ret:0x%x\n", ret);
        goto error;
    }

    /* should fail */
    ret = AXCL_SYS_Link(&srcMod2, &dstMod);
    if (!ret) {
        printf("we expect AXCL_SYS_Link return error code but it not!\n");
        goto error;
    }

    ret = AXCL_SYS_UnLink(&srcMod, &dstMod);
    if (ret) {
        printf("AXCL_SYS_UnLink failed 2,ret:0x%x\n", ret);
        goto error;
    }

    /* case 6:one src --> two dst, should success */
    srcMod.enModId = AX_ID_VIN;
    srcMod.s32GrpId = 0;
    srcMod.s32ChnId = 1;

    dstMod.enModId = AX_ID_IVPS;
    dstMod.s32GrpId = 0;
    dstMod.s32ChnId = 1;

    dstMod2.enModId = AX_ID_VENC;
    dstMod2.s32GrpId = 0;
    dstMod2.s32ChnId = 2;

    ret = AXCL_SYS_Link(&srcMod, &dstMod);
    if (ret) {
        printf("AXCL_SYS_Link failed 3,ret:0x%x\n", ret);
        goto error;
    }

    ret = AXCL_SYS_Link(&srcMod, &dstMod2);
    if (ret) {
        printf("AXCL_SYS_Link failed 4,ret:0x%x\n", ret);
        goto error;
    }

    /* AXCL_SYS_GetLinkByDest */
    ret = AXCL_SYS_GetLinkByDest(&dstMod, &tempMod);
    if (ret || (tempMod.enModId != srcMod.enModId) ||
        (tempMod.s32GrpId != srcMod.s32GrpId) ||
        (tempMod.s32ChnId != srcMod.s32ChnId)) {
        printf("AXCL_SYS_GetLinkByDest failed,ret:0x%x\n", ret);
        goto error;
    }

    /* AXCL_SYS_GetLinkBySrc */
    ret = AXCL_SYS_GetLinkBySrc(&srcMod, &linkDest);
    if (ret || (linkDest.u32DestNum != 2) ||
        (linkDest.astDestMod[0].enModId != AX_ID_IVPS) ||
        (linkDest.astDestMod[1].enModId != AX_ID_VENC)) {
        printf("AXCL_SYS_GetLinkBySrc failed,ret:0x%x\n", ret);
        goto error;
    }

    ret = AXCL_SYS_UnLink(&srcMod, &dstMod);
    if (ret) {
        printf("AXCL_SYS_UnLink failed 3,ret:0x%x\n", ret);
        goto error;
    }

    ret = AXCL_SYS_UnLink(&srcMod, &dstMod2);
    if (ret) {
        printf("AXCL_SYS_UnLink failed 4,ret:0x%x\n", ret);
        goto error;
    }

    ret = AXCL_SYS_Deinit();

    if (AXCL_SUCC != ret) {
        printf("AXCL_SYS_Deinit fail!!ret:0x%x\n", ret);
        goto error;
    } else {
        printf("AXCL_SYS_Deinit success!\n");
    }

    printf("axcl_msys_test_0001 end success!\n");
    return 0;

error:
    AXCL_SYS_Deinit();
    printf("axcl_msys_test_0001 end failed!\n");
    return -1;
}

/* test test AXCL_SYS_Link/UnLink multi-progress */
static AX_S32 axcl_msys_test_0002(AX_VOID)
{
    return 0;
}

/* test AXCL_SYS_GetCurPTS */
static AX_S32 axcl_msys_test_0003(AX_VOID)
{
    AX_S32 ret, i, fail_count = 0;
    AX_U64 start_pts;
    AX_U64 end_pts;
    AX_S32 delay_time_us = 10 * 1000;
    AX_S32 delta_us = 15000;
    AX_S32 TEST_TIME = 100;

    printf("axcl_msys_test_0003 begin\n");

    ret = AXCL_SYS_Init();

    if (AXCL_SUCC != ret) {
        printf("AXCL_SYS_Init fail!!ret:0x%x\n", ret);
        return -1;
    } else {
        printf("AXCL_SYS_Init success!\n");
    }

#if defined (WINDOWS)
    time_period tp(1);
#endif

    for (i = 0; i < TEST_TIME; i++) {

        ret = AXCL_SYS_GetCurPTS(&start_pts);
        if (ret) {
            printf("AXCL_SYS_GetCurPTS failed\n");
            fail_count++;
            continue;
        }

        std::this_thread::sleep_for(std::chrono::microseconds(delay_time_us));

        ret = AXCL_SYS_GetCurPTS(&end_pts);
        if (ret) {
            printf("AXCL_SYS_GetCurPTS failed\n");
            fail_count++;
            continue;
        }

        if (abs((AX_S32)(end_pts - start_pts - delay_time_us)) > delta_us) {
            fail_count++;
            printf("%d test failed!end_pts:%lld, start_pts:%lld, delay_time_us:%d, delta_us:%d\n",
                   i, end_pts, start_pts, delay_time_us, delta_us);
        }
    }

    ret = AXCL_SYS_Deinit();

    if (AXCL_SUCC != ret) {
        printf("AXCL_SYS_Deinit fail!!ret:0x%x\n", ret);
        return -1;
    } else {
        printf("AXCL_SYS_Deinit success!\n");
    }

    if (fail_count) {
        ret = -1;
    } else {
        ret = 0;
    }

    printf("axcl_msys_test_0003 end.Total:%d, Pass:%d, Fail:%d\n", TEST_TIME, TEST_TIME - fail_count, fail_count);

    return ret;
}

/* test AXCL_SYS_SyncPTS case 1 */
static AX_S32 axcl_msys_test_0004(AX_VOID)
{
    AX_S32 ret, i, fail_count = 0;
    AX_U64 cur_pts;
    AX_U64 new_pts_base;
    AX_S32 TEST_TIME = 50;

    printf("axcl_msys_test_0004 begin\n");

    ret = AXCL_SYS_Init();

    if (AXCL_SUCC != ret) {
        printf("AXCL_SYS_Init fail!!ret:0x%x\n", ret);
        return -1;
    } else {
        printf("AXCL_SYS_Init success!\n");
    }

    for (i = 0; i < TEST_TIME; i++) {

        ret = AXCL_SYS_GetCurPTS(&cur_pts);
        if (ret) {
            printf("AXCL_SYS_GetCurPTS failed\n");
            fail_count++;
            continue;
        }

        /* should success */
        new_pts_base = cur_pts + 1000;
        ret = AXCL_SYS_SyncPTS(new_pts_base);
        if (ret) {
            printf("AXCL_SYS_SyncPTS failed 1,ret:0x%x\n", ret);
            fail_count++;
            continue;
        }

        ret = AXCL_SYS_GetCurPTS(&cur_pts);
        if (ret) {
            printf("AXCL_SYS_GetCurPTS failed\n");
            fail_count++;
            continue;
        }

        /* should success */
        new_pts_base = cur_pts - 10;
        ret = AXCL_SYS_SyncPTS(new_pts_base);
        if (ret) {
            printf("AXCL_SYS_SyncPTS failed 2,ret:0x%x\n", ret);
            fail_count++;
        }
    }

    ret = AXCL_SYS_Deinit();

    if (AXCL_SUCC != ret) {
        printf("AXCL_SYS_Deinit fail!!ret:0x%x\n", ret);
        return -1;
    } else {
        printf("AXCL_SYS_Deinit success!\n");
    }

    if (fail_count) {
        ret = -1;
    } else {
        ret = 0;
    }

    printf("axcl_msys_test_0004 end.Total:%d, Pass:%d, Fail:%d\n", 2 * TEST_TIME, 2 * TEST_TIME - fail_count, fail_count);

    return ret;
}

/* test AXCL_SYS_SyncPTS case 2 */
static AX_S32 axcl_msys_test_0005(AX_VOID)
{
    AX_S32 ret, i, fail_count = 0;
    AX_U64 cur_pts;
    AX_U64 new_pts_base;
    AX_S32 TEST_TIME = 50;

    printf("axcl_msys_test_0005 begin\n");

    ret = AXCL_SYS_Init();

    if (AXCL_SUCC != ret) {
        printf("AXCL_SYS_Init fail!!ret:0x%x\n", ret);
        return -1;
    } else {
        printf("AXCL_SYS_Init success!\n");
    }

    for (i = 0; i < TEST_TIME; i++) {

        ret = AXCL_SYS_GetCurPTS(&cur_pts);
        if (ret) {
            printf("AXCL_SYS_GetCurPTS failed\n");
            fail_count++;
            continue;
        }

        /* should fail */
        new_pts_base = cur_pts + 5000;
        ret = AXCL_SYS_SyncPTS(new_pts_base);
        if (!ret) {
            printf("we expect AXCL_SYS_SyncPTS return error code but it not!\n");
            fail_count++;
            continue;
        }

        ret = AXCL_SYS_GetCurPTS(&cur_pts);
        if (ret) {
            printf("AXCL_SYS_GetCurPTS failed\n");
            fail_count++;
            continue;
        }

        /* should fail */
        new_pts_base = cur_pts - 5000;
        ret = AXCL_SYS_SyncPTS(new_pts_base);
        if (!ret) {
            printf("we expect AXCL_SYS_SyncPTS return error code but it not!\n");
            fail_count++;
        }
    }

    ret = AXCL_SYS_Deinit();

    if (AXCL_SUCC != ret) {
        printf("AXCL_SYS_Deinit fail!!ret:0x%x\n", ret);
        return -1;
    } else {
        printf("AXCL_SYS_Deinit success!\n");
    }

    if (fail_count) {
        ret = -1;
    } else {
        ret = 0;
    }

    printf("axcl_msys_test_0005 end.Total:%d, Pass:%d, Fail:%d\n", TEST_TIME * 2, 2 * TEST_TIME - fail_count, fail_count);

    return ret;
}

/* test AXCL_SYS_InitPTSBase */
static AX_S32 axcl_msys_test_0006(AX_VOID)
{
    AX_S32 ret, i, fail_count = 0;
    AX_U64 cur_pts;
    AX_U64 new_pts_base = 12345678;
    AX_S32 delta_us = 1000;
    AX_S32 TEST_TIME = 100;

    printf("axcl_msys_test_0006 begin\n");

    ret = AXCL_SYS_Init();

    if (AXCL_SUCC != ret) {
        printf("AXCL_SYS_Init fail!!ret:0x%x\n", ret);
        return -1;
    } else {
        printf("AXCL_SYS_Init success!\n");
    }

    for (i = 0; i < TEST_TIME; i++) {

        new_pts_base += 100*i;

        ret = AXCL_SYS_InitPTSBase(new_pts_base);
        if (ret) {
            printf("AXCL_SYS_InitPTSBase failed\n");
            fail_count++;
            continue;
        }

        ret = AXCL_SYS_GetCurPTS(&cur_pts);
        if (ret) {
            printf("AXCL_SYS_GetCurPTS failed\n");
            fail_count++;
            continue;
        }

        if (abs((AX_S32)(cur_pts - new_pts_base)) > delta_us) {
            fail_count++;
            printf("%d test failed!cur_pts:%lld, new_pts_base:%lld, delta_us:%d\n",
                   i, cur_pts, new_pts_base, delta_us);
        }
    }

    ret = AXCL_SYS_Deinit();

    if (AXCL_SUCC != ret) {
        printf("AXCL_SYS_Deinit fail!!ret:0x%x\n", ret);
        return -1;
    } else {
        printf("AXCL_SYS_Deinit success!\n");
    }

    if (fail_count) {
        ret = -1;
    } else {
        ret = 0;
    }

    printf("axcl_msys_test_0006 end.Total:%d, Pass:%d, Fail:%d\n", TEST_TIME, TEST_TIME - fail_count, fail_count);

    return ret;
}