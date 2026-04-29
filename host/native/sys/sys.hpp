/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#pragma once

#include "axcl_sys_type.h"
#include "native.hpp"
#include "package/host/native/module/sys_request.hpp"
#include "package/host/native/module/sys_response.hpp"

#define AXCL_DEF_SYS_ERR(e) AXCL_DEF_NATIVE_ERR(static_cast<uint8_t>(axcl::pkg::native::module::SYS), (e))

class sys : public native<axcl::pkg::host::sys_request, axcl::pkg::host::sys_response> {
public:
    sys() : native<axcl::pkg::host::sys_request, axcl::pkg::host::sys_response>(axcl::pkg::native::module::SYS) {
    }

    AX_S32 AX_SYS_Init();
    AX_S32 AX_SYS_Deinit();

    AX_S32 AX_SYS_MemAlloc(AX_U64 *phyaddr, AX_VOID **pviraddr, AX_U32 size, AX_U32 align, const AX_S8 *token);
    AX_S32 AX_SYS_MemAllocCached(AX_U64 *phyaddr, AX_VOID **pviraddr, AX_U32 size, AX_U32 align, const AX_S8 *token);
    AX_S32 AX_SYS_MemFree(AX_U64 phyaddr, AX_VOID *pviraddr);
    AX_VOID *AX_SYS_Mmap(AX_U64 phyaddr, AX_U32 size);
    AX_VOID *AX_SYS_MmapCache(AX_U64 phyaddr, AX_U32 size);
    AX_VOID *AX_SYS_MmapFast(AX_U64 phyaddr, AX_U32 size);
    AX_VOID *AX_SYS_MmapCacheFast(AX_U64 phyaddr, AX_U32 size);
    AX_S32 AX_SYS_Munmap(AX_VOID *pviraddr, AX_U32 size);
    AX_S32 AX_SYS_MflushCache(AX_U64 phyaddr, AX_VOID *pviraddr, AX_U32 size);
    AX_S32 AX_SYS_MinvalidateCache(AX_U64 phyaddr, AX_VOID *pviraddr, AX_U32 size);
    AX_S32 AX_SYS_MemGetBlockInfoByPhy(AX_U64 phyaddr, AX_S32 *pmemType, AX_VOID **pviraddr, AX_U32 *pblockSize);
    AX_S32 AX_SYS_MemGetBlockInfoByVirt(AX_VOID *pviraddr, AX_U64 *pphyaddr, AX_S32 *pmemType);
    AX_S32 AX_SYS_MemGetPartitionInfo(AX_CMM_PARTITION_INFO_T *pCmmPartitionInfo);
    AX_S32 AX_SYS_MemSetConfig(const AX_MOD_INFO_T *pModInfo, const AX_S8 *pPartitionName);
    AX_S32 AX_SYS_MemGetConfig(const AX_MOD_INFO_T *pModInfo, AX_S8 *pPartitionName);
    AX_S32 AX_SYS_MemQueryStatus(AX_CMM_STATUS_T *pCmmStatus);

    AX_S32 AX_SYS_Link(const AX_MOD_INFO_T *pSrc, const AX_MOD_INFO_T *pDest);
    AX_S32 AX_SYS_UnLink(const AX_MOD_INFO_T *pSrc, const AX_MOD_INFO_T *pDest);
    AX_S32 AX_SYS_GetLinkByDest(const AX_MOD_INFO_T *pDest, AX_MOD_INFO_T *pSrc);
    AX_S32 AX_SYS_GetLinkBySrc(const AX_MOD_INFO_T *pSrc, AX_LINK_DEST_T *pLinkDest);

    AX_S32 AX_POOL_SetConfig(const AX_POOL_FLOORPLAN_T *pPoolFloorPlan);
    AX_S32 AX_POOL_GetConfig(AX_POOL_FLOORPLAN_T *pPoolFloorPlan);
    AX_S32 AX_POOL_Init();
    AX_S32 AX_POOL_Exit();
    AX_POOL AX_POOL_CreatePool(AX_POOL_CONFIG_T *pPoolConfig);
    AX_S32 AX_POOL_DestroyPool(AX_POOL PoolId);
    AX_BLK AX_POOL_GetBlock(AX_POOL PoolId, AX_U64 BlkSize, const AX_S8 *pPartitionName);
    AX_S32 AX_POOL_ReleaseBlock(AX_BLK BlockId);
    AX_BLK AX_POOL_PhysAddr2Handle(AX_U64 PhysAddr);
    AX_U64 AX_POOL_Handle2PhysAddr(AX_BLK BlockId);
    AX_U64 AX_POOL_Handle2MetaPhysAddr(AX_BLK BlockId);
    AX_POOL AX_POOL_Handle2PoolId(AX_BLK BlockId);
    AX_U64 AX_POOL_Handle2BlkSize(AX_BLK BlockId);
    AX_S32 AX_POOL_MmapPool(AX_POOL PoolId);
    AX_S32 AX_POOL_MunmapPool(AX_POOL PoolId);
    AX_VOID *AX_POOL_GetBlockVirAddr(AX_BLK BlockId);
    AX_VOID *AX_POOL_GetMetaVirAddr(AX_BLK BlockId);
    AX_S32 AX_POOL_IncreaseRefCnt(AX_BLK BlockId);
    AX_S32 AX_POOL_DecreaseRefCnt(AX_BLK BlockId);

    AX_S32 AX_SYS_GetCurPTS(AX_U64 *pu64CurPTS);
    AX_S32 AX_SYS_InitPTSBase(AX_U64 u64PTSBase);
    AX_S32 AX_SYS_SyncPTS(AX_U64 u64PTSBase);

    AX_CHIP_TYPE_E AX_SYS_GetChipType();

    AX_S32 AX_SYS_SetLogLevel(AX_LOG_LEVEL_E level);
    AX_S32 AX_SYS_SetLogTarget(AX_LOG_TARGET_E target);
    AX_S32 AX_SYS_EnableTimestamp(AX_BOOL enable);

    AX_S32 AX_SYS_Sleep();
    AX_S32 AX_SYS_WakeLock(const AX_MOD_ID_E ModId);
    AX_S32 AX_SYS_WakeUnlock(const AX_MOD_ID_E ModId);
    AX_S32 AX_SYS_RegisterEventCb(const AX_MOD_ID_E ModId, NotifyEventCallback pFunction, AX_VOID *pData);
    AX_S32 AX_SYS_UnregisterEventCb(const AX_MOD_ID_E ModId);
};
