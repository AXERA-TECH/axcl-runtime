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
#include <cstdint>
#include "axcl.h"
#include "cmdline.h"

static int axcl_cmm_test_0001();
static int axcl_cmm_test_0002();
static int axcl_cmm_test_0003();
static int axcl_cmm_test_0004();
static int axcl_cmm_test_0005();
static int axcl_cmm_test_0006();
static int axcl_cmm_test_0007(int32_t device);

int main(int argc, char *argv[]) {
    cmdline::parser a;
    a.add<uint32_t>("device", 'd', "device index from 0 to connected device num - 1", false, 0, cmdline::range(0, AXCL_MAX_DEVICE_COUNT - 1));
    a.add<int32_t>("case", 't', "case number", true);
    a.parse_check(argc, argv);
    const uint32_t device_index = a.get<uint32_t>("device");
    const int32_t case_num = a.get<int32_t>("case");

    printf("axcl_ut_cmm test begin\n");

    axclError ret;
    ret = axclInit(NULL);
    if (AXCL_SUCC != ret) {
        printf("axcl_ut_cmm test failed!\n");
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
        printf("axcl_ut_cmm test failed!\n");
        return 1;
    }

    switch(case_num)
    {
    case 1:
        ret = axcl_cmm_test_0001();
        break;
    case 2:
        ret = axcl_cmm_test_0002();
        break;
    case 3:
        ret = axcl_cmm_test_0003();
        break;
    case 4:
        ret = axcl_cmm_test_0004();
        break;
    case 5:
        ret = axcl_cmm_test_0005();
        break;
    case 6:
        ret = axcl_cmm_test_0006();
        break;
    case 7:
        ret = axcl_cmm_test_0007(device);
        break;
    default:
        printf("axcl_ut_cmm test failed!Invalid case number!\n");
        ret = -1;
    }

    if (ret) {
        printf("axcl_ut_cmm test failed!\n");
    } else {
        printf("axcl_ut_cmm test success end\n");
    }

    axclrtResetDevice(device);
    axclFinalize();
    return 0;
}

/* AXCL_SYS_MemAlloc/AXCL_SYS_MemFree */
static int axcl_cmm_test_0001() {
    AX_S32 ret, i;
    AX_U64 PhyAddr[10]= { 0 };
    AX_VOID *pVirAddr[10] = { 0 };
    AX_U32 BlockSize = 1 * 1024 * 1024;
    AX_U32 align = 0x1000;
    AX_S8 blockname[20];

    printf("axcl_cmm_test_0001 begin\n");

    AXCL_SYS_Init();

    for (i = 0; i < 10; i++) {
        sprintf((char *)blockname, "noncache_block_%d", i);
        ret = AXCL_SYS_MemAlloc(&PhyAddr[i], (AX_VOID **)&pVirAddr[i], BlockSize, align, blockname);
        if (AXCL_SUCC != ret) {
            printf("AXCL_SYS_MemAlloc failed\n");
            return -1;
        }
        printf("alloc PhyAddr= 0x%llx,pVirAddr=%p\n", PhyAddr[i], pVirAddr[i]);
    }

    for (i = 0; i < 10; i++) {
        ret = AXCL_SYS_MemFree(PhyAddr[i], (AX_VOID *)pVirAddr[i]);
        if (AXCL_SUCC != ret) {
            printf("AXCL_SYS_MemFree failed\n");
            return -1;
        }
        printf("free PhyAddr= 0x%llx,pVirAddr=%p\n", PhyAddr[i], pVirAddr[i]);
    }

    AXCL_SYS_Deinit();

    printf("axcl_cmm_test_0001 end success\n\n");
    return 0;
}

/* AXCL_SYS_MemAllocCached/AXCL_SYS_MemFree */
static int axcl_cmm_test_0002() {
    AX_S32 ret, i;
    AX_U64 PhyAddr[10]= { 0 };
    AX_VOID *pVirAddr[10] = { 0 };
    AX_U32 BlockSize = 1 * 1024 * 1024;
    AX_U32 align = 0x1000;
    AX_S8 blockname[20];

    printf("axcl_cmm_test_0002 begin\n");

    AXCL_SYS_Init();

    for (i = 0; i < 10; i++) {
        sprintf((char *)blockname, "cache_block_%d", i);
        ret = AXCL_SYS_MemAllocCached(&PhyAddr[i], (AX_VOID **)&pVirAddr[i], BlockSize, align, blockname);
        if (AXCL_SUCC != ret) {
            printf("AXCL_SYS_MemAllocCached failed\n");
            return -1;
        }
        printf("alloc PhyAddr= 0x%llx,pVirAddr=%p\n", PhyAddr[i], pVirAddr[i]);
    }

    for (i = 0; i < 10; i++) {
        ret = AXCL_SYS_MemFree(PhyAddr[i], (AX_VOID *)pVirAddr[i]);
        if (AXCL_SUCC != ret) {
            printf("AXCL_SYS_MemFree failed\n");
            return -1;
        }
        printf("free PhyAddr= 0x%llx,pVirAddr=%p\n", PhyAddr[i], pVirAddr[i]);
    }

    AXCL_SYS_Deinit();

    printf("axcl_cmm_test_0002 end success\n\n");
    return 0;
}

/* AXCL_SYS_MemAlloc/AXCL_SYS_MemFree without virtaddr */
static int axcl_cmm_test_0003() {
    AX_S32 ret, i;
    AX_U64 PhyAddr[10]= { 0 };
    AX_U32 BlockSize = 1 * 1024 * 1024;
    AX_U32 align = 0x1000;
    AX_S8 blockname[20];

    printf("axcl_cmm_test_0003 begin\n");

    AXCL_SYS_Init();

    for (i = 0; i < 10; i++) {
        sprintf((char *)blockname, "noncache_block_%d", i);
        ret = AXCL_SYS_MemAlloc(&PhyAddr[i], NULL, BlockSize, align, blockname);
        if (AXCL_SUCC != ret) {
            printf("AXCL_SYS_MemAlloc failed\n");
            return -1;
        }
        printf("alloc PhyAddr= 0x%llx\n", PhyAddr[i]);
    }

    for (i = 0; i < 10; i++) {
        ret = AXCL_SYS_MemFree(PhyAddr[i], NULL);
        if (AXCL_SUCC != ret) {
            printf("AXCL_SYS_MemFree failed\n");
            return -1;
        }
        printf("free PhyAddr= 0x%llx\n", PhyAddr[i]);
    }

    AXCL_SYS_Deinit();

    printf("axcl_cmm_test_0003 end success\n\n");
    return 0;
}

/* AXCL_SYS_Mmap/AXCL_SYS_Munmap */
static int axcl_cmm_test_0004(void)
{
    AX_U64 PhyAddr;
    AX_VOID *pVirAddr;
    AX_VOID *pVirAddr2;
    AX_U32 BlockSize = 1 * 1024 * 1024;
    AX_S32 ret;

    printf("axcl_cmm_test_0004 begin\n");

    AXCL_SYS_Init();

    ret = AXCL_SYS_MemAlloc(&PhyAddr, (AX_VOID **)&pVirAddr, BlockSize, 0x1000, (AX_S8 *)"mem_test");
    if (ret != 0) {
        printf("AXCL_SYS_MemAlloc failed\n");
        return -1;
    }

    printf("PhyAddr= 0x%llx,pVirAddr=%p\n", PhyAddr, pVirAddr);

    pVirAddr2 = AXCL_SYS_Mmap(PhyAddr, BlockSize);
    if (pVirAddr2 == NULL) {
        printf("AXCL_SYS_Mmap failed\n");
        return -1;
    } else {
        printf("PhyAddr= 0x%llx,new pVirAddr2=%p\n", PhyAddr, pVirAddr2);
    }

    ret = AXCL_SYS_Munmap(pVirAddr2, BlockSize);
    if (ret != 0) {
        printf("AXCL_SYS_Munmap failed\n");
        return -1;
    }

    ret = AXCL_SYS_MemFree(PhyAddr, (AX_VOID *)pVirAddr);
    if (ret != 0) {
        printf("AXCL_SYS_MemFree failed\n");
        return -1;
    }

    AXCL_SYS_Deinit();

    printf("axcl_cmm_test_0004 end success\n\n");
    return 0;
}

/* AXCL_SYS_MmapCache/AXCL_SYS_Munmap */
static int axcl_cmm_test_0005(void)
{
    AX_U64 PhyAddr;
    AX_VOID *pVirAddr;
    AX_VOID *pVirAddr2;
    AX_U32 BlockSize = 1 * 1024 * 1024;
    AX_S32 ret;

    printf("axcl_cmm_test_0005 begin\n");

    AXCL_SYS_Init();

    ret = AXCL_SYS_MemAlloc(&PhyAddr, (AX_VOID **)&pVirAddr, BlockSize, 0x1000, (AX_S8 *)"mem_test");
    if (ret != 0) {
        printf("AXCL_SYS_MemAlloc failed\n");
        return -1;
    }

    printf("PhyAddr= 0x%llx,pVirAddr=%p\n", PhyAddr, pVirAddr);

    pVirAddr2 = AXCL_SYS_MmapCache(PhyAddr, BlockSize);
    if (pVirAddr2 == NULL) {
        printf("AXCL_SYS_MmapCache failed\n");
        return -1;
    } else {
        printf("PhyAddr= 0x%llx,new cache pVirAddr2=%p\n", PhyAddr, pVirAddr2);
    }

    ret = AXCL_SYS_Munmap(pVirAddr2, BlockSize);
    if (ret != 0) {
        printf("AXCL_SYS_Munmap failed\n");
        return -1;
    }

    ret = AXCL_SYS_MemFree(PhyAddr, (AX_VOID *)pVirAddr);
    if (ret != 0) {
        printf("AXCL_SYS_MemFree failed\n");
        return -1;
    }

    AXCL_SYS_Deinit();

    printf("axcl_cmm_test_0005 end success\n");
    return 0;
}

/* AXCL_SYS_MemGetBlockInfoByVirt/AXCL_SYS_MemGetBlockInfoByPhy */
static int axcl_cmm_test_0006(void)
{
    AX_U64 PhyAddr;
    AX_VOID *pVirAddr;
    AX_U64 PhyAddr2;
    AX_VOID *pVirAddr2;
    AX_U32 BlockSize;
    AX_S32 CacheType;
    AX_S32 ret;

    printf("axcl_cmm_test_0006 begin\n");

    AXCL_SYS_Init();

    /* non-cached type */
    ret = AXCL_SYS_MemAlloc(&PhyAddr, (AX_VOID **)&pVirAddr, 1 * 1024 * 1024, 0x1000, (AX_S8 *)"non-cached");
    if (ret != 0) {
        printf("AXCL_SYS_MemAlloc failed\n");
        return -1;
    }

    printf("PhyAddr= 0x%llx,non-cached pVirAddr=%p\n", PhyAddr, pVirAddr);

    ret = AXCL_SYS_MemGetBlockInfoByVirt(pVirAddr, &PhyAddr2, &CacheType);
    if (ret != 0) {
        printf("AXCL_SYS_MemGetBlockInfoByVirt failed\n");
        goto FAIL;
    } else {
        printf("non-cached pVirAddr:%p --> PhyAddr2:0x%llx ,CacheType:%d\n", pVirAddr, PhyAddr2, CacheType);
    }

    if(CacheType != AX_MEM_NONCACHED) {
        printf("non-cache virtaddr,but get a wrong type!\n");
        goto FAIL;
    }

    if ((PhyAddr2 - PhyAddr) != 0) {
        printf("PhyAddr2 got wrong offset!\n");
        goto FAIL;
    }

    ret = AXCL_SYS_MemGetBlockInfoByVirt((AX_VOID *)((char *)pVirAddr + 0x20ff), &PhyAddr2, &CacheType);
    if (ret != 0) {
        printf("AXCL_SYS_MemGetBlockInfoByVirt failed\n");
        goto FAIL;
    } else {
        printf("non-cached pVirAddr:%p --> PhyAddr2:0x%llx ,CacheType:%d\n", (char *)pVirAddr + 0x20ff, PhyAddr2,CacheType);
    }

    if(CacheType != AX_MEM_NONCACHED) {
        printf("non-cached virtaddr,but get a wrong type!\n");
        goto FAIL;
    }

    if ((PhyAddr2 - PhyAddr) != 0x20ff) {
        printf("PhyAddr2 got wrong offset!\n");
        goto FAIL;
    }

    ret = AXCL_SYS_MemGetBlockInfoByPhy(PhyAddr, &CacheType, &pVirAddr2, &BlockSize);
    if (ret != 0) {
        printf("AXCL_SYS_MemGetBlockInfoByPhy failed\n");
        goto FAIL;
    } else {
        printf("PhyAddr:0x%llx --> pVirAddr2:%p ,CacheType:%d,BlockSize=%d\n", PhyAddr, pVirAddr2, CacheType, BlockSize);
    }

    if(CacheType != AX_MEM_NONCACHED) {
        printf("non-cached virtaddr,but get a wrong type!\n");
        goto FAIL;
    }

    if (((uintptr_t)pVirAddr2 - (uintptr_t)pVirAddr) != 0) {
        printf("pVirAddr2 got wrong offset!\n");
        goto FAIL;
    }

    ret = AXCL_SYS_MemGetBlockInfoByPhy(PhyAddr + 0x20ff, &CacheType, &pVirAddr2, &BlockSize);
    if (ret != 0) {
        printf("AXCL_SYS_MemGetBlockInfoByPhy failed\n");
        goto FAIL;
    } else {
        printf("PhyAddr:0x%llx --> pVirAddr2:%p ,CacheType:%d,BlockSize=%d\n", PhyAddr + 0x20ff, pVirAddr2, CacheType, BlockSize);
    }

    if(CacheType != AX_MEM_NONCACHED) {
        printf("non-cached virtaddr,but get a wrong type!\n");
        goto FAIL;
    }

    if (((uintptr_t)pVirAddr2 - (uintptr_t)pVirAddr) != 0x20ff) {
        printf("pVirAddr2 got wrong offset!\n");
        goto FAIL;
    }

    ret = AXCL_SYS_MemFree(PhyAddr, (AX_VOID *)pVirAddr);
    if (ret != 0) {
        printf("AXCL_SYS_MemFree failed\n");
        goto FAIL;
    }

    /* cached type */
    ret = AXCL_SYS_MemAllocCached(&PhyAddr, (AX_VOID **)&pVirAddr, 1 * 1024 * 1024, 0x1000, (AX_S8 *)"cached");
    if (ret != 0) {
        printf("AXCL_SYS_MemAllocCached failed\n");
        return -1;
    }

    printf("PhyAddr= 0x%llx,cached pVirAddr=%p\n", PhyAddr, pVirAddr);

    ret = AXCL_SYS_MemGetBlockInfoByVirt(pVirAddr, &PhyAddr2, &CacheType);
    if (ret != 0) {
        printf("AXCL_SYS_MemGetBlockInfoByVirt failed\n");
        goto FAIL;
    } else {
        printf("cached pVirAddr:%p --> PhyAddr2:0x%llx ,CacheType:%d\n", pVirAddr, PhyAddr2, CacheType);
    }

    if(CacheType != AX_MEM_CACHED) {
        printf("cached virtaddr,but get a wrong type!\n");
        goto FAIL;
    }

    if ((PhyAddr2 - PhyAddr) != 0) {
        printf("PhyAddr2 got wrong offset!\n");
        goto FAIL;
    }

    ret = AXCL_SYS_MemGetBlockInfoByVirt((AX_VOID *)((char *)pVirAddr + 0x22dd), &PhyAddr2, &CacheType);
    if (ret != 0) {
        printf("AXCL_SYS_MemGetBlockInfoByVirt failed\n");
        goto FAIL;
    } else {
        printf("cached pVirAddr:%p --> PhyAddr2:0x%llx ,CacheType:%d\n", (char *)pVirAddr + 0x22dd, PhyAddr2,CacheType);
    }

    if(CacheType != AX_MEM_CACHED) {
        printf("cached virtaddr,but get a wrong type!\n");
        goto FAIL;
    }

    if ((PhyAddr2 - PhyAddr) != 0x22dd) {
        printf("PhyAddr2 got wrong offset!\n");
        goto FAIL;
    }

    ret = AXCL_SYS_MemGetBlockInfoByPhy(PhyAddr, &CacheType, &pVirAddr2, &BlockSize);
    if (ret != 0) {
        printf("AXCL_SYS_MemGetBlockInfoByPhy failed\n");
        goto FAIL;
    } else {
        printf("PhyAddr:0x%llx --> pVirAddr2:%p ,CacheType:%d,BlockSize=%d\n", PhyAddr, pVirAddr2, CacheType, BlockSize);
    }

    if(CacheType != AX_MEM_CACHED) {
        printf("cached virtaddr,but get a wrong type!\n");
        goto FAIL;
    }

    if (((uintptr_t)pVirAddr2 - (uintptr_t)pVirAddr) != 0) {
        printf("pVirAddr2 got wrong offset!\n");
        goto FAIL;
    }

    ret = AXCL_SYS_MemGetBlockInfoByPhy(PhyAddr + 0x2ffd, &CacheType, &pVirAddr2, &BlockSize);
    if (ret != 0) {
        printf("AXCL_SYS_MemGetBlockInfoByPhy failed\n");
        goto FAIL;
    } else {
        printf("PhyAddr:0x%llx --> pVirAddr2:%p ,CacheType:%d,BlockSize=%d\n", PhyAddr + 0x2ffd, pVirAddr2, CacheType, BlockSize);
    }

    if(CacheType != AX_MEM_CACHED) {
        printf("non-cached virtaddr,but get a wrong type!\n");
        goto FAIL;
    }

    if (((uintptr_t)pVirAddr2 - (uintptr_t)pVirAddr) != 0x2ffd) {
        printf("pVirAddr2 got wrong offset!\n");
        goto FAIL;
    }

    ret = AXCL_SYS_MemFree(PhyAddr, (AX_VOID *)pVirAddr);
    if (ret != 0) {
        printf("AXCL_SYS_MemFree failed\n");
        goto FAIL;
    }

    AXCL_SYS_Deinit();

    printf("axcl_cmm_test_0006 end success\n");

    return 0;

FAIL:
    ret = AXCL_SYS_MemFree(PhyAddr, (AX_VOID *)pVirAddr);
    if (ret != 0) {
        printf("AXCL_SYS_MemFree failed\n");
        return -1;
    }

    AXCL_SYS_Deinit();
    printf("axcl_cmm_test_0006 end failed\n");

    return -1;
}

/* CMM multi-thread */
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
    AX_U64 PhyAddr = 0;
    AX_VOID *pVirAddr = NULL;
    AX_VOID *pVirAddr2 = NULL;
    AX_U32 BlockSize = 1*1024*1024;
    AX_S32 nRet;
    SAMPLE_THREAD_PARAM_T *pThreadParam = (SAMPLE_THREAD_PARAM_T *)arg;
    axclrtContext context;

    nRet = axclrtCreateContext(&context, pThreadParam->deviceId);
    if (AXCL_SUCC != nRet) {
        printf("axcl create cxt err, nRet=0x%x.\n", nRet);
        pThreadParam->s32ThreadResult = -1;
        return NULL;
    }

    AXCL_SYS_Init();

    while (pThreadParam->bThreadStart) {

        nRet = AXCL_SYS_MemAlloc(&PhyAddr, (AX_VOID **)&pVirAddr, BlockSize, 0x1000, (AX_S8 *)"cmm_test");
        if (nRet != 0) {
            printf("AXCL_SYS_MemAlloc failed\n");
            goto ERROR;
        }

        pVirAddr2 = AXCL_SYS_Mmap(PhyAddr, BlockSize);
        if (pVirAddr2 == NULL) {
            printf("AXCL_SYS_Mmap failed\n");
            goto ERROR;
        }

        nRet = AXCL_SYS_Munmap(pVirAddr2, BlockSize);
        if (nRet != 0) {
            printf("AXCL_SYS_Munmap failed\n");
            goto ERROR;
        }

        nRet = AXCL_SYS_MemFree(PhyAddr, (AX_VOID *)pVirAddr);
        if (nRet != 0) {
            printf("AXCL_SYS_MemFree failed\n");
            goto ERROR;
        }

        std::this_thread::sleep_for(std::chrono::microseconds(1000*100));
    }

    AXCL_SYS_Deinit();

    axclrtDestroyContext(context);
    pThreadParam->s32ThreadResult = 0;
    return NULL;

ERROR:
    if (pVirAddr) {
        nRet = AXCL_SYS_MemFree(PhyAddr, (AX_VOID *)pVirAddr);
        if (nRet != 0) {
            printf("AXCL_SYS_MemFree failed\n");
        }
    }

    AXCL_SYS_Deinit();
    axclrtDestroyContext(context);

    pThreadParam->s32ThreadResult = -1;
    printf("Error occurred,thread_func exit!\n");
    return NULL;
}

static AX_S32 axcl_cmm_test_0007(int32_t device)
{
    AX_S32 i = 0, result = 0;
    std::vector<std::thread> thread_array(MAX_THREAD_NUM);

    printf("axcl_cmm_test_0007 begin\n");

    gLoopExit = AX_FALSE;

    std::signal(SIGINT, SigInt);

    printf("device id: %d\n", device);

    for (i = 0; i < MAX_THREAD_NUM; i++) {

        gThreadParam[i].bThreadStart = AX_TRUE;
        gThreadParam[i].s32ThreadResult = 0;
        gThreadParam[i].deviceId = device;
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

    printf("axcl_cmm_test_0007 end\n");
    return result;
}