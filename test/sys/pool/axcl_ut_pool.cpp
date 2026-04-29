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
#include <csignal>
#include <thread>
#include <chrono>
#include <vector>
#include "axcl.h"
#include "cmdline.h"

static int axcl_pool_test_0001();
static int axcl_pool_test_0002();
static int axcl_pool_test_0003();
static int axcl_pool_test_0004();

int main(int argc, char *argv[]) {
    cmdline::parser a;
    a.add<uint32_t>("device", 'd', "device index from 0 to connected device num - 1", false, 0, cmdline::range(0, AXCL_MAX_DEVICE_COUNT - 1));
    a.add<int32_t>("case", 't', "case number", true);
    a.parse_check(argc, argv);
    const uint32_t device_index = a.get<uint32_t>("device");
    const int32_t case_num = a.get<int32_t>("case");

    printf("axcl_ut_pool test begin\n");

    axclError ret;
    ret = axclInit(NULL);
    if (AXCL_SUCC != ret) {
        printf("axcl_ut_pool test failed!\n");
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
        printf("axcl_ut_pool test failed!\n");
        return 1;
    }

    switch(case_num)
    {
    case 1:
        ret = axcl_pool_test_0001();
        break;
    case 2:
        ret = axcl_pool_test_0002();
        break;
    case 3:
        ret = axcl_pool_test_0003();
        break;
    case 4:
        ret = axcl_pool_test_0004();
        break;
    default:
        printf("axcl_ut_pool test failed!Invalid case number!\n");
        ret = -1;
    }

    if (ret) {
        printf("axcl_ut_pool test failed!\n");
    } else {
        printf("axcl_ut_pool test success end\n");
    }

    axclrtResetDevice(device);
    axclFinalize();

    return 0;
}

static int axcl_pool_test_0001() {
    AX_S32 ret = 0;
    AX_POOL_FLOORPLAN_T PoolFloorPlan = { 0 };
    AX_U64 BlkSize;
    AX_BLK BlkId;
    AX_POOL PoolId;
    AX_U64 PhysAddr, MetaPhysAddr;

    printf("axcl_pool_test_0001 begin\n");

    /* step 1:SYS init */
    ret = AXCL_SYS_Init();

    if (AXCL_SUCC != ret) {
        printf("AXCL_SYS_Init fail!!ret:0x%x\n", ret);
        return -1;
    } else {
        printf("AXCL_SYS_Init success!\n");
    }

    /* step 2:exit pool */
    ret = AXCL_POOL_Exit();

    if (AXCL_SUCC != ret) {
        printf("AXCL_POOL_Exit fail!!ret:0x%x\n", ret);
        goto error0;
    } else {
        printf("AXCL_POOL_Exit success!\n");
    }

    /* step 3:config common pool */
    memset(&PoolFloorPlan, 0, sizeof(AX_POOL_FLOORPLAN_T));
    PoolFloorPlan.CommPool[0].MetaSize   = 0x2000;
    PoolFloorPlan.CommPool[0].BlkSize   = 1 * 1024 * 1024;
    PoolFloorPlan.CommPool[0].BlkCnt    = 5;
    PoolFloorPlan.CommPool[0].CacheMode = POOL_CACHE_MODE_NONCACHE;
    PoolFloorPlan.CommPool[1].MetaSize   = 0x2000;
    PoolFloorPlan.CommPool[1].BlkSize   = 2 * 1024 * 1024;
    PoolFloorPlan.CommPool[1].BlkCnt    = 5;
    PoolFloorPlan.CommPool[1].CacheMode = POOL_CACHE_MODE_NONCACHE;
    PoolFloorPlan.CommPool[2].MetaSize   = 0x2000;
    PoolFloorPlan.CommPool[2].BlkSize   = 3 * 1024 * 1024;
    PoolFloorPlan.CommPool[2].BlkCnt    = 5;
    PoolFloorPlan.CommPool[2].CacheMode = POOL_CACHE_MODE_NONCACHE;

    /* The default value of PartitionName is anonymous. The partition name must exist when the cmm ko is loaded */
    memset(PoolFloorPlan.CommPool[0].PartitionName, 0, sizeof(PoolFloorPlan.CommPool[0].PartitionName));
    memset(PoolFloorPlan.CommPool[1].PartitionName, 0, sizeof(PoolFloorPlan.CommPool[1].PartitionName));
    memset(PoolFloorPlan.CommPool[2].PartitionName, 0, sizeof(PoolFloorPlan.CommPool[2].PartitionName));
    strcpy((char *)PoolFloorPlan.CommPool[0].PartitionName, "anonymous");
    strcpy((char *)PoolFloorPlan.CommPool[1].PartitionName, "anonymous");
    strcpy((char *)PoolFloorPlan.CommPool[2].PartitionName, "anonymous");

    ret = AXCL_POOL_SetConfig(&PoolFloorPlan);

    if (AXCL_SUCC != ret) {
        printf("AXCL_POOL_SetConfig fail!ret:0x%x\n", ret);
        goto error0;
    } else {
        printf("AXCL_POOL_SetConfig success!\n");
    }

    /* step 4:pool init */
    ret = AXCL_POOL_Init();
    if (AXCL_SUCC != ret) {
        printf("AXCL_POOL_Init fail!!ret:0x%x\n", ret);
        goto error0;
    } else {
        printf("AXCL_POOL_Init success!\n");
    }

    /* step 5:get block from common pool */

    BlkSize = 3 * 1024 * 1024;
    BlkId = AXCL_POOL_GetBlock(AX_INVALID_POOLID, BlkSize, NULL);

    if (BlkId == AX_INVALID_BLOCKID) {
        printf("AXCL_POOL_GetBlock fail!\n");
        goto error1;
    } else {
        printf("AXCL_POOL_GetBlock success!BlkId:0x%X\n", BlkId);
    }

    /* step 6:get poolid from blkid */
    PoolId = AXCL_POOL_Handle2PoolId(BlkId);

    if (PoolId == (AX_POOL)AX_INVALID_POOLID) {
        printf("AXCL_POOL_Handle2PoolId fail!\n");
        AXCL_POOL_ReleaseBlock(BlkId);
        goto error1;
    } else {
        printf("AXCL_POOL_Handle2PoolId success!(Blockid:0x%X --> PoolId=%d)\n", BlkId, PoolId);
    }

    /* step 7:get block phyaddr from blkid */
    PhysAddr = AXCL_POOL_Handle2PhysAddr(BlkId);

    if (!PhysAddr) {
        printf("AXCL_POOL_Handle2PhysAddr fail!\n");
        AXCL_POOL_ReleaseBlock(BlkId);
        goto error1;
    } else {
        printf("AXCL_POOL_Handle2PhysAddr success!(Blockid:0x%X --> PhyAddr=0x%llx)\n", BlkId, PhysAddr);
    }

    /* step 8:get metadata phyaddr from blkid */
    MetaPhysAddr = AXCL_POOL_Handle2MetaPhysAddr(BlkId);

    if (!MetaPhysAddr) {
        printf("AXCL_POOL_Handle2MetaPhysAddr fail!\n");
        AXCL_POOL_ReleaseBlock(BlkId);
        goto error1;
    } else {
        printf("AXCL_POOL_Handle2MetaPhysAddr success!(Blockid:0x%X --> MetaPhyAddr=0x%llx)\n", BlkId, MetaPhysAddr);
    }

    /* step 9:release block */
    ret = AXCL_POOL_ReleaseBlock(BlkId);

    if (AXCL_SUCC != ret) {
        printf("AXCL_POOL_ReleaseBlock fail!ret:0x%x\n", ret);
        goto error1;
    } else {
        printf("AXCL_POOL_ReleaseBlock success!Blockid=0x%x\n", BlkId);
    }

    /* step 10:pool exit */
    ret = AXCL_POOL_Exit();

    if (AXCL_SUCC != ret) {
        printf("AXCL_POOL_Exit fail!!ret:0x%x\n", ret);
        goto error1;
    } else {
        printf("AXCL_POOL_Exit success!\n");
    }

    /* step 11:SYS deinit */
    ret = AXCL_SYS_Deinit();

    if (AXCL_SUCC != ret) {
        printf("AXCL_SYS_Deinit fail!!ret:0x%x\n", ret);
        goto error1;
    } else {
        printf("AXCL_SYS_Deinit success!\n");
    }

    printf("axcl_pool_test_0001 end success!\n");
    return 0;

error1:
    AXCL_POOL_Exit();

error0:
    AXCL_SYS_Deinit();

    printf("axcl_pool_test_0001 end fail!\n");
    return -1;
}

static int axcl_pool_test_0002() {
    AX_S32 ret = 0;
    AX_U64 BlkSize;
    AX_BLK BlkId;
    AX_POOL PoolId;
    AX_U64 PhysAddr, MetaPhysAddr;
    AX_VOID *blockVirAddr, *metaVirAddr;
    AX_POOL_CONFIG_T PoolConfig = { 0 };
    AX_POOL UserPoolId0 , UserPoolId1;

    printf("axcl_pool_test_0002 begin\n");

    /* step 1:SYS init */
    ret = AXCL_SYS_Init();

    if (AXCL_SUCC != ret) {
        printf("AXCL_SYS_Init fail!!ret:0x%x\n", ret);
        return -1;
    } else {
        printf("AXCL_SYS_Init success!\n");
    }

    /* step 2:exit pool */
    ret = AXCL_POOL_Exit();

    if (AXCL_SUCC != ret) {
        printf("AXCL_POOL_Exit fail!!ret:0x%x\n", ret);
        goto error0;
    } else {
        printf("AXCL_POOL_Exit success!\n");
    }

    /* step 3:AX_POOL_CreatePool */

    /* user_pool_0 :blocksize=1*1024*1024,metasize=0x1000,block count =2 ,noncache */
    memset(&PoolConfig, 0, sizeof(AX_POOL_CONFIG_T));
    PoolConfig.MetaSize = 0x1000;
    PoolConfig.BlkSize = 1 * 1024 * 1024;
    PoolConfig.BlkCnt = 2;
    PoolConfig.CacheMode = POOL_CACHE_MODE_NONCACHE;
    memset(PoolConfig.PartitionName,0, sizeof(PoolConfig.PartitionName));
    strcpy((char *)PoolConfig.PartitionName, "anonymous");

    UserPoolId0 = AXCL_POOL_CreatePool(&PoolConfig);

    if ((AX_POOL)AX_INVALID_POOLID == UserPoolId0) {
        printf("AXCL_POOL_CreatePool error!!!\n");
        goto error1;
    } else {
        printf("AXCL_POOL_CreatePool[%d] success\n", UserPoolId0);
    }

    /* user_pool_1:blocksize=2*1024*1024,metasize=0x1000,block count =3 ,noncache */
    memset(&PoolConfig, 0, sizeof(AX_POOL_CONFIG_T));
    PoolConfig.MetaSize = 0x1000;
    PoolConfig.BlkSize = 2 * 1024 * 1024;
    PoolConfig.BlkCnt = 3;
    PoolConfig.CacheMode = POOL_CACHE_MODE_NONCACHE;
    memset(PoolConfig.PartitionName,0, sizeof(PoolConfig.PartitionName));
    strcpy((char *)PoolConfig.PartitionName, "anonymous");

    UserPoolId1 = AXCL_POOL_CreatePool(&PoolConfig);

    if ((AX_POOL)AX_INVALID_POOLID == UserPoolId1) {
        printf("AXCL_POOL_CreatePool error!!!\n");
        goto error1;
    } else {
        printf("AXCL_POOL_CreatePool[%d] success\n", UserPoolId1);
    }

    /* step 4:get block from user pool */
    BlkSize = 2 * 1024 * 1024;
    BlkId = AXCL_POOL_GetBlock(UserPoolId1, BlkSize, NULL);

    if (BlkId == AX_INVALID_BLOCKID) {
        printf("AXCL_POOL_GetBlock fail!\n");
        goto error1;
    } else {
        printf("AXCL_POOL_GetBlock success!BlkId:0x%X\n", BlkId);
    }

    /* step 5:get poolid from blkid */
    PoolId = AXCL_POOL_Handle2PoolId(BlkId);

    if (PoolId == (AX_POOL)AX_INVALID_POOLID) {
        printf("AXCL_POOL_Handle2PoolId fail!\n");
        AXCL_POOL_ReleaseBlock(BlkId);
        goto error1;
    } else {
        printf("AXCL_POOL_Handle2PoolId success!(Blockid:0x%X --> PoolId=%d)\n", BlkId, PoolId);
    }

    /* step 6:get block phyaddr from blkid */
    PhysAddr = AXCL_POOL_Handle2PhysAddr(BlkId);

    if (!PhysAddr) {
        printf("AXCL_POOL_Handle2PhysAddr fail!\n");
        AXCL_POOL_ReleaseBlock(BlkId);
        goto error1;
    } else {
        printf("AXCL_POOL_Handle2PhysAddr success!(Blockid:0x%X --> PhyAddr=0x%llx)\n", BlkId, PhysAddr);
    }

    /* step 7:get metadata phyaddr from blkid */
    MetaPhysAddr = AXCL_POOL_Handle2MetaPhysAddr(BlkId);

    if (!MetaPhysAddr) {
        printf("AXCL_POOL_Handle2MetaPhysAddr fail!\n");
        AXCL_POOL_ReleaseBlock(BlkId);
        goto error1;
    } else {
        printf("AXCL_POOL_Handle2MetaPhysAddr success!(Blockid:0x%X --> MetaPhyAddr=0x%llx)\n", BlkId, MetaPhysAddr);
    }

    /* step 8:get block viraddr from blkid */
    blockVirAddr = AXCL_POOL_GetBlockVirAddr(BlkId);

    if (!blockVirAddr) {
        printf("AXCL_POOL_GetBlockVirAddr fail!\n");
        AXCL_POOL_ReleaseBlock(BlkId);
        goto error1;
    } else {
        printf("AXCL_POOL_GetBlockVirAddr success!blockVirAddr=%p\n", blockVirAddr);
    }

    /* step 9:get metadata viraddr from blkid */
    metaVirAddr = AXCL_POOL_GetMetaVirAddr(BlkId);

    if (!metaVirAddr) {
        printf("AXCL_POOL_GetMetaVirAddr fail!\n");
        AXCL_POOL_ReleaseBlock(BlkId);
        goto error1;
    } else {
        printf("AXCL_POOL_GetMetaVirAddr success!metaVirAddr=%p\n" ,metaVirAddr);
    }

    /* step 10:release block */
    ret = AXCL_POOL_ReleaseBlock(BlkId);

    if (AXCL_SUCC != ret) {
        printf("AXCL_POOL_ReleaseBlock fail!ret:0x%x\n", ret);
        goto error1;
    } else {
        printf("AXCL_POOL_ReleaseBlock success!Blockid=0x%x\n", BlkId);
    }

    /* step 11:destroy pool */
    ret = AXCL_POOL_DestroyPool(UserPoolId1);
    if (AXCL_SUCC != ret) {
        printf("AXCL_POOL_DestroyPool fail!ret:0x%x\n", ret);
        goto error1;
    } else {
        printf("AXCL_POOL_DestroyPool[%d] success!\n", UserPoolId1);
    }

    ret = AXCL_POOL_DestroyPool(UserPoolId0);
    if (AXCL_SUCC != ret) {
        printf("AXCL_POOL_DestroyPool fail!ret:0x%x\n", ret);
        goto error1;
    } else {
        printf("AXCL_POOL_DestroyPool[%d] success!\n", UserPoolId0);
    }

    /* step 12:SYS deinit */
    ret = AXCL_SYS_Deinit();

    if (AXCL_SUCC != ret) {
        printf("AXCL_SYS_Deinit fail!!ret:0x%x\n", ret);
        goto error1;
    } else {
        printf("AXCL_SYS_Deinit success!\n");
    }

    printf("axcl_pool_test_0002 end success!\n");
    return 0;

error1:
    AXCL_POOL_Exit();

error0:
    AXCL_SYS_Deinit();

    printf("axcl_pool_test_0002 end fail!\n");
    return -1;
}

static AX_S32 axcl_pool_test_0003()
{
    AX_S32 ret = 0;
    AX_U64 BlkSize;
    AX_BLK BlkId;
    AX_POOL_CONFIG_T PoolConfig;
    AX_POOL UserPoolId;

    printf("axcl_pool_test_0003 begin\n");

    /* step 1:SYS init */
    ret = AXCL_SYS_Init();

    if (AXCL_SUCC != ret) {
        printf("AXCL_SYS_Init fail!!ret:0x%x\n", ret);
        return -1;
    } else {
        printf("AXCL_SYS_Init success!\n");
    }

    /* step 2:exit pool */
    ret = AXCL_POOL_Exit();

    if (AXCL_SUCC != ret) {
        printf("AXCL_POOL_Exit fail!!ret:0x%x\n", ret);
        goto error0;
    } else {
        printf("AXCL_POOL_Exit success!\n");
    }

    /* step 3:create pool with IsMergeMode=TRUE */
    memset(&PoolConfig, 0, sizeof(AX_POOL_CONFIG_T));
    PoolConfig.MetaSize = 0x1000;
    PoolConfig.BlkSize = 1 * 1024 * 1024;
    PoolConfig.BlkCnt = 2;
    PoolConfig.CacheMode = POOL_CACHE_MODE_NONCACHE;
    PoolConfig.IsMergeMode = AX_TRUE;
    memset(PoolConfig.PartitionName,0, sizeof(PoolConfig.PartitionName));
    strcpy((char *)PoolConfig.PartitionName, "anonymous");

    UserPoolId = AXCL_POOL_CreatePool(&PoolConfig);

    if ((AX_POOL)AX_INVALID_POOLID == UserPoolId) {
        printf("AXCL_POOL_CreatePool error!!!\n");
        goto error0;
    } else {
        printf("AXCL_POOL_CreatePool[%d] success\n", UserPoolId);
    }

    /* step 4:get block from common pool */
    BlkSize = 1 * 1024 * 1024;
    BlkId = AXCL_POOL_GetBlock(AX_INVALID_POOLID, BlkSize, NULL);

    if (BlkId == AX_INVALID_BLOCKID) {
        printf("AXCL_POOL_GetBlock fail!\n");
        goto error1;
    } else {
        printf("AXCL_POOL_GetBlock success!BlkId:0x%X\n", BlkId);
    }

    /* step 5:release block */
    ret = AXCL_POOL_ReleaseBlock(BlkId);

    if (AXCL_SUCC != ret) {
        printf("AXCL_POOL_ReleaseBlock fail!ret:0x%x\n", ret);
        goto error1;
    } else {
        printf("AXCL_POOL_ReleaseBlock success!Blockid=0x%X\n", BlkId);
    }

    /* step 6:destroy pool */
    ret = AXCL_POOL_DestroyPool(UserPoolId);
    if (AXCL_SUCC != ret) {
        printf("AXCL_POOL_DestroyPool fail!!ret:0x%x\n", ret);
        goto error1;
    } else {
        printf("AXCL_POOL_DestroyPool[%d] success!\n", UserPoolId);
    }

    /* step 7:SYS deinit */
    ret = AXCL_POOL_Exit();

    if (AXCL_SUCC != ret) {
        printf("AXCL_POOL_Exit fail!!ret:0x%x\n", ret);
        goto error1;
    } else {
        printf("AXCL_POOL_Exit success!\n");
    }

    printf("axcl_pool_test_0003 end success!\n");

    AXCL_SYS_Deinit();
    return 0;

error1:
    AXCL_POOL_Exit();

error0:
    AXCL_SYS_Deinit();

    printf("axcl_pool_test_0003 end fail!\n");
    return -1;
}

/* Multiple threads frequently create/operate/destroy pool */
#define MAX_THREAD_NUM (10)

typedef struct {
    AX_BOOL bThreadStart;
    AX_S32 deviceId;
    AX_S32 s32ThreadResult;
} SAMPLE_THREAD_PARAM_T;

static SAMPLE_THREAD_PARAM_T gThreadParam[MAX_THREAD_NUM];
static AX_BOOL gLoopExit;

static void SigInt(AX_S32 signal)
{
    printf("\nCatch Ctrl+C, exit thread!\n");
    gLoopExit = AX_TRUE;
}

static void *thread_func(void *arg)
{
    AX_S32 ret;
    AX_U64 BlkSize;
    AX_BLK BlkId;
    AX_VOID *blockVirAddr, *metaVirAddr;
    AX_U64 PhysAddr,MetaPhysAddr;
    AX_POOL_CONFIG_T PoolConfig;
    AX_POOL UserPoolId, PoolId;
    SAMPLE_THREAD_PARAM_T *pThreadParam = (SAMPLE_THREAD_PARAM_T *)arg;
    axclrtContext context;

    ret = axclrtCreateContext(&context, pThreadParam->deviceId);
    if (AXCL_SUCC != ret) {
        printf("axcl create cxt err, ret=0x%x.\n", ret);
        pThreadParam->s32ThreadResult = -1;
        return NULL;
    }

    AXCL_SYS_Init();

    while (pThreadParam->bThreadStart) {

        memset(&PoolConfig, 0, sizeof(AX_POOL_CONFIG_T));
        PoolConfig.MetaSize = 0x1000;
        PoolConfig.BlkSize = 1 * 1024 * 1024;
        PoolConfig.BlkCnt = 2;
        PoolConfig.CacheMode = POOL_CACHE_MODE_NONCACHE;
        memset(PoolConfig.PartitionName,0, sizeof(PoolConfig.PartitionName));
        strcpy((char *)PoolConfig.PartitionName, "anonymous");

        UserPoolId = AXCL_POOL_CreatePool(&PoolConfig);

        if ((AX_POOL)AX_INVALID_POOLID == UserPoolId) {
            printf("AXCL_POOL_CreatePool fail!!!\n");
            goto ERROR;
        }

        BlkSize = 1 * 1024 * 1024;
        BlkId = AXCL_POOL_GetBlock(UserPoolId, BlkSize, NULL);

        if (BlkId == AX_INVALID_BLOCKID) {
            printf("AXCL_POOL_GetBlock fail!\n");
            goto ERROR;
        }

        ret = AXCL_POOL_IncreaseRefCnt(BlkId);

        if (ret) {
            printf("AX_POOL_IncreaseRefCnt fail!!Error Code:0x%X\n", ret);
            goto ERROR;
        }

        ret = AXCL_POOL_DecreaseRefCnt(BlkId);

        if (ret) {
            printf("AXCL_POOL_DecreaseRefCnt fail!!Error Code:0x%X\n", ret);
            goto ERROR;
        }

        PoolId = AXCL_POOL_Handle2PoolId(BlkId);

        if (PoolId != UserPoolId) {
            printf("AXCL_POOL_Handle2PoolId fail!UserPoolId=%d, PoolId=%d\n", UserPoolId, PoolId);
            goto ERROR;
        }

        BlkSize = AXCL_POOL_Handle2BlkSize(BlkId);

        if (BlkSize == 0) {
            printf("AXCL_POOL_Handle2BlkSize fail!\n");
            goto ERROR;
        }

        PhysAddr = AXCL_POOL_Handle2PhysAddr(BlkId);

        if (!PhysAddr) {
            printf("AXCL_POOL_Handle2PhysAddr fail!\n");
            goto ERROR;
        }

        MetaPhysAddr = AXCL_POOL_Handle2MetaPhysAddr(BlkId);

        if (!MetaPhysAddr) {
            printf("AXCL_POOL_Handle2MetaPhysAddr fail!\n");
            goto ERROR;
        }

        blockVirAddr = AXCL_POOL_GetBlockVirAddr(BlkId);

        if (!blockVirAddr) {
            printf("AXCL_POOL_GetBlockVirAddr fail!\n");
            goto ERROR;
        }

        metaVirAddr = AXCL_POOL_GetMetaVirAddr(BlkId);

        if (!metaVirAddr) {
            printf("AXCL_POOL_GetMetaVirAddr fail!\n");
            goto ERROR;
        }

        ret = AXCL_POOL_ReleaseBlock(BlkId);

        if (ret) {
            printf("AXCL_POOL_ReleaseBlock fail!Error Code:0x%X\n", ret);
            goto ERROR;
        }

        ret = AXCL_POOL_DestroyPool(UserPoolId);
        if (ret) {
            printf("AXCL_POOL_DestroyPool fail!!Error Code:0x%X\n", ret);
            goto ERROR;
        }

        std::this_thread::sleep_for(std::chrono::microseconds(1000*100));
    }

    AXCL_SYS_Deinit();
    axclrtDestroyContext(context);
    pThreadParam->s32ThreadResult = 0;
    return NULL;

ERROR:
    ret = AXCL_POOL_DestroyPool(UserPoolId);
    if (ret) {
        printf("AXCL_POOL_DestroyPool fail!!Error Code:0x%X\n", ret);
    }

    AXCL_SYS_Deinit();

    axclrtDestroyContext(context);
    pThreadParam->s32ThreadResult = -1;
    printf("Error occurred,thread_func exit!\n");
    return NULL;
}

static AX_S32 axcl_pool_test_0004()
{
    AX_S32 i = 0, result = 0, ret;
    std::vector<std::thread> thread_array(MAX_THREAD_NUM);
    axclrtDeviceList lst;

    printf("axcl_pool_test_0004 begin\n");

    gLoopExit = AX_FALSE;

    std::signal(SIGINT, SigInt);

    if (ret = axclrtGetDeviceList(&lst); AXCL_SUCC != ret || 0 == lst.num) {
        printf("no device is connected\n");
        axclFinalize();
        return 1;
    }

    printf("device id: %d\n", lst.devices[0]);

    for (i = 0; i < MAX_THREAD_NUM; i++) {

        gThreadParam[i].bThreadStart = AX_TRUE;
        gThreadParam[i].s32ThreadResult = 0;
        gThreadParam[i].deviceId = lst.devices[0];
        thread_array[i] = std::thread(thread_func, (void *)&gThreadParam[i]);
    }

    while (!gLoopExit) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    for (i = 0; i < MAX_THREAD_NUM; i++) {
        if (gThreadParam[i].bThreadStart == AX_TRUE) {
            gThreadParam[i].bThreadStart = AX_FALSE;
            thread_array[i].join();
        }

        if (gThreadParam[i].s32ThreadResult == -1) {
            printf("thread_func_%d failed\n", i);
            result = -1;
        }
    }

    printf("axcl_pool_test_0004 end\n");

    return result;
}