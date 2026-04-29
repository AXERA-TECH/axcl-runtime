/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "axcl_base.h"
#include "axcl_module_version.h"
#include "axcl_sys.h"
#include "axcl_logger.hpp"
#include "sys.hpp"

#define TAG "sys"
#define CHECK_NULL_POINTER(p)                           \
    if (!(p)) {                                         \
        LOG_MM_E(TAG, "nil pointer");                   \
        return AXCL_DEF_SYS_ERR(AXCL_ERR_NULL_POINTER); \
    }

AXCL_EXPORT AX_S32 AXCL_SYS_Init(AX_VOID) {
    /**
     *  device worker is in charge of sys init and deinit, so host return directly
     */
    // sys api;
    // return api.AX_SYS_Init();
    return AXCL_SUCC;
}

AXCL_EXPORT AX_S32 AXCL_SYS_Deinit(AX_VOID) {
    // sys api;
    // return api.AX_SYS_Deinit();
    return AXCL_SUCC;
}

AXCL_EXPORT AX_S32 AXCL_SYS_MemAlloc(AX_U64 *phyaddr, AX_VOID **pviraddr, AX_U32 size, AX_U32 align, const AX_S8 *token) {
    CHECK_NULL_POINTER(phyaddr)

    sys api;
    return api.AX_SYS_MemAlloc(phyaddr, pviraddr, size, align, token);
}

AXCL_EXPORT AX_S32 AXCL_SYS_MemAllocCached(AX_U64 *phyaddr, AX_VOID **pviraddr, AX_U32 size, AX_U32 align, const AX_S8 *token) {
    CHECK_NULL_POINTER(phyaddr)
    CHECK_NULL_POINTER(pviraddr)

    sys api;
    return api.AX_SYS_MemAllocCached(phyaddr, pviraddr, size, align, token);
}

AXCL_EXPORT AX_S32 AXCL_SYS_MemFree(AX_U64 phyaddr, AX_VOID *pviraddr) {
    sys api;
    return api.AX_SYS_MemFree(phyaddr, pviraddr);
}

AXCL_EXPORT AX_VOID *AXCL_SYS_Mmap(AX_U64 phyaddr, AX_U32 size) {
    sys api;
    return api.AX_SYS_Mmap(phyaddr, size);
}

AXCL_EXPORT AX_VOID *AXCL_SYS_MmapCache(AX_U64 phyaddr, AX_U32 size) {
    sys api;
    return api.AX_SYS_MmapCache(phyaddr, size);
}

AXCL_EXPORT AX_VOID *AXCL_SYS_MmapFast(AX_U64 phyaddr, AX_U32 size) {
    sys api;
    return api.AX_SYS_MmapFast(phyaddr, size);
}

AXCL_EXPORT AX_VOID *AXCL_SYS_MmapCacheFast(AX_U64 phyaddr, AX_U32 size) {
    sys api;
    return api.AX_SYS_MmapCacheFast(phyaddr, size);
}

AXCL_EXPORT AX_S32 AXCL_SYS_Munmap(AX_VOID *pviraddr, AX_U32 size) {
    CHECK_NULL_POINTER(pviraddr)

    sys api;
    return api.AX_SYS_Munmap(pviraddr, size);
}

AXCL_EXPORT AX_S32 AXCL_SYS_MflushCache(AX_U64 phyaddr, AX_VOID *pviraddr, AX_U32 size) {
    CHECK_NULL_POINTER(pviraddr)

    sys api;
    return api.AX_SYS_MflushCache(phyaddr, pviraddr, size);
}

AXCL_EXPORT AX_S32 AXCL_SYS_MinvalidateCache(AX_U64 phyaddr, AX_VOID *pviraddr, AX_U32 size) {
    CHECK_NULL_POINTER(pviraddr)

    sys api;
    return api.AX_SYS_MinvalidateCache(phyaddr, pviraddr, size);
}

AXCL_EXPORT AX_S32 AXCL_SYS_MemGetBlockInfoByPhy(AX_U64 phyaddr, AX_S32 *pmemType, AX_VOID **pviraddr, AX_U32 *pblockSize) {
    CHECK_NULL_POINTER(pmemType)
    CHECK_NULL_POINTER(pviraddr)
    CHECK_NULL_POINTER(pblockSize)

    sys api;
    return api.AX_SYS_MemGetBlockInfoByPhy(phyaddr, pmemType, pviraddr, pblockSize);
}

AXCL_EXPORT AX_S32 AXCL_SYS_MemGetBlockInfoByVirt(AX_VOID *pviraddr, AX_U64 *phyaddr, AX_S32 *pmemType) {
    CHECK_NULL_POINTER(pviraddr)
    CHECK_NULL_POINTER(phyaddr)
    CHECK_NULL_POINTER(pmemType)

    sys api;
    return api.AX_SYS_MemGetBlockInfoByVirt(pviraddr, phyaddr, pmemType);
}

AXCL_EXPORT AX_S32 AXCL_SYS_MemGetPartitionInfo(AX_CMM_PARTITION_INFO_T *pCmmPartitionInfo) {
    CHECK_NULL_POINTER(pCmmPartitionInfo)

    sys api;
    return api.AX_SYS_MemGetPartitionInfo(pCmmPartitionInfo);
}

AXCL_EXPORT AX_S32 AXCL_SYS_MemSetConfig(const AX_MOD_INFO_T *pModInfo, const AX_S8 *pPartitionName) {
    CHECK_NULL_POINTER(pModInfo)
    CHECK_NULL_POINTER(pPartitionName)

    sys api;
    return api.AX_SYS_MemSetConfig(pModInfo, pPartitionName);
}

AXCL_EXPORT AX_S32 AXCL_SYS_MemGetConfig(const AX_MOD_INFO_T *pModInfo, AX_S8 *pPartitionName) {
    CHECK_NULL_POINTER(pModInfo)
    CHECK_NULL_POINTER(pPartitionName)

    sys api;
    return api.AX_SYS_MemGetConfig(pModInfo, pPartitionName);
}

AXCL_EXPORT AX_S32 AXCL_SYS_MemQueryStatus(AX_CMM_STATUS_T *pCmmStatus) {
    CHECK_NULL_POINTER(pCmmStatus)

    sys api;
    return api.AX_SYS_MemQueryStatus(pCmmStatus);
}

AXCL_EXPORT AX_S32 AXCL_SYS_Link(const AX_MOD_INFO_T *pSrc, const AX_MOD_INFO_T *pDest) {
    CHECK_NULL_POINTER(pSrc)
    CHECK_NULL_POINTER(pDest)

    sys api;
    return api.AX_SYS_Link(pSrc, pDest);
}

AXCL_EXPORT AX_S32 AXCL_SYS_UnLink(const AX_MOD_INFO_T *pSrc, const AX_MOD_INFO_T *pDest) {
    CHECK_NULL_POINTER(pSrc)
    CHECK_NULL_POINTER(pDest)

    sys api;
    return api.AX_SYS_UnLink(pSrc, pDest);
}

AXCL_EXPORT AX_S32 AXCL_SYS_GetLinkByDest(const AX_MOD_INFO_T *pDest, AX_MOD_INFO_T *pSrc) {
    CHECK_NULL_POINTER(pDest)
    CHECK_NULL_POINTER(pSrc)

    sys api;
    return api.AX_SYS_GetLinkByDest(pDest, pSrc);
}

AXCL_EXPORT AX_S32 AXCL_SYS_GetLinkBySrc(const AX_MOD_INFO_T *pSrc, AX_LINK_DEST_T *pLinkDest) {
    CHECK_NULL_POINTER(pSrc)
    CHECK_NULL_POINTER(pLinkDest)

    sys api;
    return api.AX_SYS_GetLinkBySrc(pSrc, pLinkDest);
}

AXCL_EXPORT AX_S32 AXCL_POOL_SetConfig(const AX_POOL_FLOORPLAN_T *pPoolFloorPlan) {
    CHECK_NULL_POINTER(pPoolFloorPlan)

    sys api;
    return api.AX_POOL_SetConfig(pPoolFloorPlan);
}

AXCL_EXPORT AX_S32 AXCL_POOL_GetConfig(AX_POOL_FLOORPLAN_T *pPoolFloorPlan) {
    CHECK_NULL_POINTER(pPoolFloorPlan)

    sys api;
    return api.AX_POOL_GetConfig(pPoolFloorPlan);
}

AXCL_EXPORT AX_S32 AXCL_POOL_Init(AX_VOID) {
    sys api;
    return api.AX_POOL_Init();
}

AXCL_EXPORT AX_S32 AXCL_POOL_Exit(AX_VOID) {
    sys api;
    return api.AX_POOL_Exit();
}

AXCL_EXPORT AX_POOL AXCL_POOL_CreatePool(AX_POOL_CONFIG_T *pPoolConfig) {
    sys api;
    return api.AX_POOL_CreatePool(pPoolConfig);
}

AXCL_EXPORT AX_S32 AXCL_POOL_DestroyPool(AX_POOL PoolId) {
    sys api;
    return api.AX_POOL_DestroyPool(PoolId);
}

AXCL_EXPORT AX_BLK AXCL_POOL_GetBlock(AX_POOL PoolId, AX_U64 BlkSize, const AX_S8 *pPartitionName) {
    sys api;
    return api.AX_POOL_GetBlock(PoolId, BlkSize, pPartitionName);
}

AXCL_EXPORT AX_S32 AXCL_POOL_ReleaseBlock(AX_BLK BlockId) {
    sys api;
    return api.AX_POOL_ReleaseBlock(BlockId);
}

AXCL_EXPORT AX_BLK AXCL_POOL_PhysAddr2Handle(AX_U64 PhysAddr) {
    sys api;
    return api.AX_POOL_PhysAddr2Handle(PhysAddr);
}

AXCL_EXPORT AX_U64 AXCL_POOL_Handle2PhysAddr(AX_BLK BlockId) {
    sys api;
    return api.AX_POOL_Handle2PhysAddr(BlockId);
}

AXCL_EXPORT AX_U64 AXCL_POOL_Handle2MetaPhysAddr(AX_BLK BlockId) {
    sys api;
    return api.AX_POOL_Handle2MetaPhysAddr(BlockId);
}

AXCL_EXPORT AX_POOL AXCL_POOL_Handle2PoolId(AX_BLK BlockId) {
    sys api;
    return api.AX_POOL_Handle2PoolId(BlockId);
}

AXCL_EXPORT AX_U64 AXCL_POOL_Handle2BlkSize(AX_BLK BlockId) {
    sys api;
    return api.AX_POOL_Handle2BlkSize(BlockId);
}

AXCL_EXPORT AX_S32 AXCL_POOL_MmapPool(AX_POOL PoolId) {
    sys api;
    return api.AX_POOL_MmapPool(PoolId);
}

AXCL_EXPORT AX_S32 AXCL_POOL_MunmapPool(AX_POOL PoolId) {
    sys api;
    return api.AX_POOL_MunmapPool(PoolId);
}

AXCL_EXPORT AX_VOID *AXCL_POOL_GetBlockVirAddr(AX_BLK BlockId) {
    sys api;
    return api.AX_POOL_GetBlockVirAddr(BlockId);
}

AXCL_EXPORT AX_VOID *AXCL_POOL_GetMetaVirAddr(AX_BLK BlockId) {
    sys api;
    return api.AX_POOL_GetMetaVirAddr(BlockId);
}

AXCL_EXPORT AX_S32 AXCL_POOL_IncreaseRefCnt(AX_BLK BlockId) {
    sys api;
    return api.AX_POOL_IncreaseRefCnt(BlockId);
}

AXCL_EXPORT AX_S32 AXCL_POOL_DecreaseRefCnt(AX_BLK BlockId) {
    sys api;
    return api.AX_POOL_DecreaseRefCnt(BlockId);
}

AXCL_EXPORT AX_S32 AXCL_SYS_GetCurPTS(AX_U64 *pu64CurPTS) {
    CHECK_NULL_POINTER(pu64CurPTS)

    sys api;
    return api.AX_SYS_GetCurPTS(pu64CurPTS);
}

AXCL_EXPORT AX_S32 AXCL_SYS_InitPTSBase(AX_U64 u64PTSBase) {
    sys api;
    return api.AX_SYS_InitPTSBase(u64PTSBase);
}

AXCL_EXPORT AX_S32 AXCL_SYS_SyncPTS(AX_U64 u64PTSBase) {
    sys api;
    return api.AX_SYS_SyncPTS(u64PTSBase);
}

AXCL_EXPORT AX_CHIP_TYPE_E AXCL_SYS_GetChipType(AX_VOID) {
    sys api;
    return api.AX_SYS_GetChipType();
}

AXCL_EXPORT AX_S32 AXCL_SYS_SetLogLevel(AX_LOG_LEVEL_E target) {
    sys api;
    return api.AX_SYS_SetLogLevel(target);
}

AXCL_EXPORT AX_S32 AXCL_SYS_SetLogTarget(AX_LOG_TARGET_E target) {
    sys api;
    return api.AX_SYS_SetLogTarget(target);
}

AXCL_EXPORT AX_S32 AXCL_SYS_EnableTimestamp(AX_BOOL enable) {
    LOG_MM_E(TAG, "unsupported");
    return AXCL_DEF_SYS_ERR(AXCL_ERR_UNSUPPORT);
}

AXCL_EXPORT AX_S32 AXCL_SYS_Sleep(AX_VOID) {
    LOG_MM_E(TAG, "unsupported");
    return AXCL_DEF_SYS_ERR(AXCL_ERR_UNSUPPORT);
}

AXCL_EXPORT AX_S32 AXCL_SYS_WakeLock(const AX_MOD_ID_E ModId) {
    LOG_MM_E(TAG, "unsupported");
    return AXCL_DEF_SYS_ERR(AXCL_ERR_UNSUPPORT);
}

AXCL_EXPORT AX_S32 AXCL_SYS_WakeUnlock(const AX_MOD_ID_E ModId) {
    LOG_MM_E(TAG, "unsupported");
    return AXCL_DEF_SYS_ERR(AXCL_ERR_UNSUPPORT);
}

AXCL_EXPORT AX_S32 AXCL_SYS_RegisterEventCb(const AX_MOD_ID_E ModId, NotifyEventCallback pFunction, AX_VOID *pData) {
    LOG_MM_E(TAG, "unsupported");
    return AXCL_DEF_SYS_ERR(AXCL_ERR_UNSUPPORT);
}

AXCL_EXPORT AX_S32 AXCL_SYS_UnregisterEventCb(const AX_MOD_ID_E ModId) {
    LOG_MM_E(TAG, "unsupported");
    return AXCL_DEF_SYS_ERR(AXCL_ERR_UNSUPPORT);
}
