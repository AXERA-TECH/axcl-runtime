/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "sys.hpp"
#include "dispatcher.hpp"
#include "axcl_logger.hpp"

#define TAG "sys"

namespace axcl::worker {

sys::sys(dispatcher *dispatcher) : base(dispatcher), m_api(TAG) {
    m_api.append({
        NATIVE_SYS_API_APPEND(AX_SYS_Init),
        NATIVE_SYS_API_APPEND(AX_SYS_Deinit),
        NATIVE_SYS_API_APPEND(AX_SYS_MemAlloc),
        NATIVE_SYS_API_APPEND(AX_SYS_MemAllocCached),
        NATIVE_SYS_API_APPEND(AX_SYS_MemFree),
        NATIVE_SYS_API_APPEND(AX_SYS_Mmap),
        NATIVE_SYS_API_APPEND(AX_SYS_MmapCache),
        NATIVE_SYS_API_APPEND(AX_SYS_MmapFast),
        NATIVE_SYS_API_APPEND(AX_SYS_MmapCacheFast),
        NATIVE_SYS_API_APPEND(AX_SYS_Munmap),
        NATIVE_SYS_API_APPEND(AX_SYS_MflushCache),
        NATIVE_SYS_API_APPEND(AX_SYS_MinvalidateCache),
        NATIVE_SYS_API_APPEND(AX_SYS_MemGetBlockInfoByPhy),
        NATIVE_SYS_API_APPEND(AX_SYS_MemGetBlockInfoByVirt),
        NATIVE_SYS_API_APPEND(AX_SYS_MemGetPartitionInfo),
        NATIVE_SYS_API_APPEND(AX_SYS_MemSetConfig),
        NATIVE_SYS_API_APPEND(AX_SYS_MemGetConfig),
        NATIVE_SYS_API_APPEND(AX_SYS_MemQueryStatus),
        NATIVE_SYS_API_APPEND(AX_SYS_Link),
        NATIVE_SYS_API_APPEND(AX_SYS_UnLink),
        NATIVE_SYS_API_APPEND(AX_SYS_GetLinkByDest),
        NATIVE_SYS_API_APPEND(AX_SYS_GetLinkBySrc),
        NATIVE_SYS_API_APPEND(AX_POOL_SetConfig),
        NATIVE_SYS_API_APPEND(AX_POOL_GetConfig),
        NATIVE_SYS_API_APPEND(AX_POOL_Init),
        NATIVE_SYS_API_APPEND(AX_POOL_Exit),
        NATIVE_SYS_API_APPEND(AX_POOL_CreatePool),
        NATIVE_SYS_API_APPEND(AX_POOL_DestroyPool),
        NATIVE_SYS_API_APPEND(AX_POOL_GetBlock),
        NATIVE_SYS_API_APPEND(AX_POOL_ReleaseBlock),
        NATIVE_SYS_API_APPEND(AX_POOL_PhysAddr2Handle),
        NATIVE_SYS_API_APPEND(AX_POOL_Handle2PhysAddr),
        NATIVE_SYS_API_APPEND(AX_POOL_Handle2MetaPhysAddr),
        NATIVE_SYS_API_APPEND(AX_POOL_Handle2PoolId),
        NATIVE_SYS_API_APPEND(AX_POOL_Handle2BlkSize),
        NATIVE_SYS_API_APPEND(AX_POOL_MmapPool),
        NATIVE_SYS_API_APPEND(AX_POOL_MunmapPool),
        NATIVE_SYS_API_APPEND(AX_POOL_GetBlockVirAddr),
        NATIVE_SYS_API_APPEND(AX_POOL_GetMetaVirAddr),
        NATIVE_SYS_API_APPEND(AX_POOL_IncreaseRefCnt),
        NATIVE_SYS_API_APPEND(AX_POOL_DecreaseRefCnt),
        NATIVE_SYS_API_APPEND(AX_SYS_GetCurPTS),
        NATIVE_SYS_API_APPEND(AX_SYS_InitPTSBase),
        NATIVE_SYS_API_APPEND(AX_SYS_SyncPTS),
        NATIVE_SYS_API_APPEND(AX_SYS_GetChipType),
        NATIVE_SYS_API_APPEND(AX_SYS_SetLogLevel),
        NATIVE_SYS_API_APPEND(AX_SYS_SetLogTarget),
        NATIVE_SYS_API_APPEND(AX_SYS_EnableTimestamp),
        NATIVE_SYS_API_APPEND(AX_SYS_Sleep),
        NATIVE_SYS_API_APPEND(AX_SYS_WakeLock),
        NATIVE_SYS_API_APPEND(AX_SYS_WakeUnlock),
        NATIVE_SYS_API_APPEND(AX_SYS_RegisterEventCb),
        NATIVE_SYS_API_APPEND(AX_SYS_UnregisterEventCb),
    });
}

void sys::run(const void *data, const size_t& size) {
    m_api.run(get_dispatcher(), data, size);
}

NATIVE_SYS_API_IMPL(AX_SYS_Init) {
    if (!response->unpack(response->get_api())) {
        return false;
    }

    AX_S32 ret = ::AX_SYS_Init();

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_SYS_Deinit) {
    if (!response->unpack(response->get_api())) {
        return false;
    }

    AX_S32 ret = ::AX_SYS_Deinit();

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_SYS_MemAlloc) {
    AX_U64 phyaddr;
    AX_U64 viraddr;
    AX_U32 size;
    AX_U32 align;
    AX_S32 ret;
    axcl::pkg::uint8_array name;

    if (!response->unpack(response->get_api(), &viraddr, &size, &align, &name)) {
        return false;
    }

    AX_VOID *pviraddr;
    if (name.size == 0) {
        ret = ::AX_SYS_MemAlloc(&phyaddr, (0 == viraddr) ? NULL : &pviraddr, size, align, NULL);
    } else {
        ret = ::AX_SYS_MemAlloc(&phyaddr, (0 == viraddr) ? NULL : &pviraddr, size, align, (AX_S8 *)name.data);
    }

    viraddr = reinterpret_cast<AX_U64>(pviraddr);

    if (!request->pack(response->get_api(), ret, phyaddr, viraddr)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_SYS_MemAllocCached) {
    AX_U64 phyaddr;
    AX_U64 viraddr;
    AX_U32 size;
    AX_U32 align;
    AX_S32 ret;
    axcl::pkg::uint8_array name;

    if (!response->unpack(response->get_api(), &size, &align, &name)) {
        return false;
    }

    AX_VOID *pviraddr;
    if (name.size == 0) {
        ret = ::AX_SYS_MemAllocCached(&phyaddr, &pviraddr, size, align, NULL);
    } else {
        ret = ::AX_SYS_MemAllocCached(&phyaddr, &pviraddr, size, align, (AX_S8 *)name.data);
    }

    viraddr = reinterpret_cast<AX_U64>(pviraddr);

    if (!request->pack(response->get_api(), ret, phyaddr, viraddr)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_SYS_MemFree) {
    AX_U64 phyaddr;
    AX_U64 viraddr;

    if (!response->unpack(response->get_api(), &phyaddr, &viraddr)) {
        return false;
    }

    AX_VOID *pviraddr = reinterpret_cast<AX_VOID *>(viraddr);
    AX_S32 ret = ::AX_SYS_MemFree(phyaddr, pviraddr);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_SYS_Mmap) {
    AX_U64 phyaddr;
    AX_U32 size;

    if (!response->unpack(response->get_api(), &phyaddr, &size)) {
        return false;
    }

    AX_VOID *pviraddr = ::AX_SYS_Mmap(phyaddr, size);
    AX_U64 viraddr = reinterpret_cast<AX_U64>(pviraddr);

    if (!request->pack(response->get_api(), viraddr)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_SYS_MmapCache) {
    AX_U64 phyaddr;
    AX_U32 size;

    if (!response->unpack(response->get_api(), &phyaddr, &size)) {
        return false;
    }

    AX_VOID *pviraddr = ::AX_SYS_MmapCache(phyaddr, size);
    AX_U64 viraddr = reinterpret_cast<AX_U64>(pviraddr);

    if (!request->pack(response->get_api(), viraddr)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_SYS_MmapFast) {
    AX_U64 phyaddr;
    AX_U32 size;

    if (!response->unpack(response->get_api(), &phyaddr, &size)) {
        return false;
    }

    AX_VOID *pviraddr = ::AX_SYS_MmapFast(phyaddr, size);
    AX_U64 viraddr = reinterpret_cast<AX_U64>(pviraddr);

    if (!request->pack(response->get_api(), viraddr)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_SYS_MmapCacheFast) {
    AX_U64 phyaddr;
    AX_U32 size;

    if (!response->unpack(response->get_api(), &phyaddr, &size)) {
        return false;
    }

    AX_VOID *pviraddr = ::AX_SYS_MmapCacheFast(phyaddr, size);
    AX_U64 viraddr = reinterpret_cast<AX_U64>(pviraddr);

    if (!request->pack(response->get_api(), viraddr)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_SYS_Munmap) {
    AX_U64 viraddr;
    AX_U32 size;

    if (!response->unpack(response->get_api(), &viraddr, &size)) {
        return false;
    }

    AX_VOID *pviraddr = reinterpret_cast<AX_VOID *>(viraddr);
    AX_S32 ret = ::AX_SYS_Munmap(pviraddr, size);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_SYS_MflushCache) {
    AX_U64 phyaddr;
    AX_U64 viraddr;
    AX_U32 size;

    if (!response->unpack(response->get_api(), &phyaddr, &viraddr, &size)) {
        return false;
    }

    AX_VOID *pviraddr = reinterpret_cast<AX_VOID *>(viraddr);
    AX_S32 ret = ::AX_SYS_MflushCache(phyaddr, pviraddr, size);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_SYS_MinvalidateCache) {
    AX_U64 phyaddr;
    AX_U64 viraddr;
    AX_U32 size;

    if (!response->unpack(response->get_api(), &phyaddr, &viraddr, &size)) {
        return false;
    }

    AX_VOID *pviraddr = reinterpret_cast<AX_VOID *>(viraddr);
    AX_S32 ret = ::AX_SYS_MinvalidateCache(phyaddr, pviraddr, size);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_SYS_MemGetBlockInfoByPhy) {
    AX_U64 phyaddr;
    AX_S32 memType;
    AX_U64 viraddr;
    AX_U32 blockSize;

    if (!response->unpack(response->get_api(), &phyaddr)) {
        return false;
    }

    AX_VOID *pviraddr;
    AX_S32 ret = ::AX_SYS_MemGetBlockInfoByPhy(phyaddr, &memType, &pviraddr, &blockSize);

    viraddr = reinterpret_cast<AX_U64>(pviraddr);

    if (!request->pack(response->get_api(), ret, memType, viraddr, blockSize)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_SYS_MemGetBlockInfoByVirt) {
    AX_U64 viraddr;
    AX_U64 phyaddr;
    AX_S32 memType;

    if (!response->unpack(response->get_api(), &viraddr)) {
        return false;
    }

    AX_VOID *pviraddr = reinterpret_cast<AX_VOID *>(viraddr);
    AX_S32 ret = ::AX_SYS_MemGetBlockInfoByVirt(pviraddr, &phyaddr, &memType);

    if (!request->pack(response->get_api(), ret, phyaddr, memType)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_SYS_MemGetPartitionInfo) {
    AX_CMM_PARTITION_INFO_T CmmPartitionInfo;

    if (!response->unpack(response->get_api())) {
        return false;
    }

    AX_S32 ret = ::AX_SYS_MemGetPartitionInfo(&CmmPartitionInfo);

    if (!request->pack(response->get_api(), ret, &CmmPartitionInfo)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_SYS_MemSetConfig) {
    AX_MOD_INFO_T ModInfo;
    axcl::pkg::uint8_array name;
    AX_S32 ret;

    if (!response->unpack(response->get_api(), &ModInfo, &name)) {
        return false;
    }

    ret = ::AX_SYS_MemSetConfig(&ModInfo, (const AX_S8 *)name.data);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_SYS_MemGetConfig) {
    AX_MOD_INFO_T ModInfo;

    if (!response->unpack(response->get_api(), &ModInfo)) {
        return false;
    }

    AX_S8 PartitionName[AX_MAX_PARTITION_NAME_LEN] = {0};
    AX_S32 ret = ::AX_SYS_MemGetConfig(&ModInfo, (AX_S8 *)PartitionName);

    axcl::pkg::uint8_array name;
    name.data = reinterpret_cast<uint8_t *>(const_cast<AX_S8 *>(PartitionName));
    name.size = strlen(reinterpret_cast<const char *>(PartitionName));

    if (!request->pack(response->get_api(), ret, name)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_SYS_MemQueryStatus) {
    AX_CMM_STATUS_T CmmStatus;

    if (!response->unpack(response->get_api())) {
        return false;
    }

    AX_S32 ret = ::AX_SYS_MemQueryStatus(&CmmStatus);

    if (!request->pack(response->get_api(), ret, &CmmStatus)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_SYS_Link) {
    AX_MOD_INFO_T Src;
    AX_MOD_INFO_T Dest;

    if (!response->unpack(response->get_api(), &Src, &Dest)) {
        return false;
    }

    AX_S32 ret = ::AX_SYS_Link(&Src, &Dest);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_SYS_UnLink) {
    AX_MOD_INFO_T Src;
    AX_MOD_INFO_T Dest;

    if (!response->unpack(response->get_api(), &Src, &Dest)) {
        return false;
    }

    AX_S32 ret = ::AX_SYS_UnLink(&Src, &Dest);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_SYS_GetLinkByDest) {
    AX_MOD_INFO_T Dest;
    AX_MOD_INFO_T Src;

    if (!response->unpack(response->get_api(), &Dest)) {
        return false;
    }

    AX_S32 ret = ::AX_SYS_GetLinkByDest(&Dest, &Src);

    if (!request->pack(response->get_api(), ret, &Src)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_SYS_GetLinkBySrc) {
    AX_MOD_INFO_T Src;
    AX_LINK_DEST_T LinkDest;

    if (!response->unpack(response->get_api(), &Src)) {
        return false;
    }

    AX_S32 ret = ::AX_SYS_GetLinkBySrc(&Src, &LinkDest);

    if (!request->pack(response->get_api(), ret, &LinkDest)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_POOL_SetConfig) {
    AX_POOL_FLOORPLAN_T PoolFloorPlan;

    if (!response->unpack(response->get_api(), &PoolFloorPlan)) {
        return false;
    }

    AX_S32 ret = ::AX_POOL_SetConfig(&PoolFloorPlan);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_POOL_GetConfig) {
    AX_POOL_FLOORPLAN_T PoolFloorPlan;

    if (!response->unpack(response->get_api())) {
        return false;
    }

    AX_S32 ret = ::AX_POOL_GetConfig(&PoolFloorPlan);

    if (!request->pack(response->get_api(), ret, &PoolFloorPlan)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_POOL_Init) {
    if (!response->unpack(response->get_api())) {
        return false;
    }

    AX_S32 ret = ::AX_POOL_Init();

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_POOL_Exit) {
    if (!response->unpack(response->get_api())) {
        return false;
    }

    AX_S32 ret = ::AX_POOL_Exit();

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_POOL_CreatePool) {
    AX_POOL_CONFIG_T PoolConfig;

    if (!response->unpack(response->get_api(), &PoolConfig)) {
        return false;
    }

    AX_POOL PoolId = ::AX_POOL_CreatePool(&PoolConfig);

    if (!request->pack(response->get_api(), PoolId)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_POOL_DestroyPool) {
    AX_POOL PoolId;

    if (!response->unpack(response->get_api(), &PoolId)) {
        return false;
    }

    AX_S32 ret = ::AX_POOL_DestroyPool(PoolId);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_POOL_GetBlock) {
    AX_POOL PoolId;
    AX_U64 BlkSize;
    AX_BLK BlockId;
    axcl::pkg::uint8_array name;

    if (!response->unpack(response->get_api(), &PoolId, &BlkSize, &name)) {
        return false;
    }

    if (name.size == 0) {
        BlockId = ::AX_POOL_GetBlock(PoolId, BlkSize, NULL);
    } else {
        BlockId = ::AX_POOL_GetBlock(PoolId, BlkSize, (const AX_S8 *)name.data);
    }

    if (!request->pack(response->get_api(), BlockId)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_POOL_ReleaseBlock) {
    AX_BLK BlockId;

    if (!response->unpack(response->get_api(), &BlockId)) {
        return false;
    }

    AX_S32 ret = ::AX_POOL_ReleaseBlock(BlockId);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_POOL_PhysAddr2Handle) {
    AX_U64 PhysAddr;

    if (!response->unpack(response->get_api(), &PhysAddr)) {
        return false;
    }

    AX_BLK BlockId = ::AX_POOL_PhysAddr2Handle(PhysAddr);

    if (!request->pack(response->get_api(), BlockId)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_POOL_Handle2PhysAddr) {
    AX_BLK BlockId;

    if (!response->unpack(response->get_api(), &BlockId)) {
        return false;
    }

    AX_U64 PhysAddr = ::AX_POOL_Handle2PhysAddr(BlockId);

    if (!request->pack(response->get_api(), PhysAddr)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_POOL_Handle2MetaPhysAddr) {
    AX_BLK BlockId;

    if (!response->unpack(response->get_api(), &BlockId)) {
        return false;
    }

    AX_U64 PhysAddr = ::AX_POOL_Handle2MetaPhysAddr(BlockId);

    if (!request->pack(response->get_api(), PhysAddr)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_POOL_Handle2PoolId) {
    AX_BLK BlockId;

    if (!response->unpack(response->get_api(), &BlockId)) {
        return false;
    }

    AX_POOL PoolId = ::AX_POOL_Handle2PoolId(BlockId);

    if (!request->pack(response->get_api(), PoolId)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_POOL_Handle2BlkSize) {
    AX_BLK BlockId;

    if (!response->unpack(response->get_api(), &BlockId)) {
        return false;
    }

    AX_U64 blkSize = ::AX_POOL_Handle2BlkSize(BlockId);

    if (!request->pack(response->get_api(), blkSize)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_POOL_MmapPool) {
    AX_BLK PoolId;

    if (!response->unpack(response->get_api(), &PoolId)) {
        return false;
    }

    AX_S32 ret = ::AX_POOL_MmapPool(PoolId);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_POOL_MunmapPool) {
    AX_BLK PoolId;

    if (!response->unpack(response->get_api(), &PoolId)) {
        return false;
    }

    AX_S32 ret = ::AX_POOL_MunmapPool(PoolId);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_POOL_GetBlockVirAddr) {
    AX_BLK BlockId;

    if (!response->unpack(response->get_api(), &BlockId)) {
        return false;
    }

    AX_VOID *pviraddr = ::AX_POOL_GetBlockVirAddr(BlockId);

    AX_U64 viraddr = reinterpret_cast<AX_U64>(pviraddr);

    if (!request->pack(response->get_api(), viraddr)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_POOL_GetMetaVirAddr) {
    AX_BLK BlockId;

    if (!response->unpack(response->get_api(), &BlockId)) {
        return false;
    }

    AX_VOID *pviraddr = ::AX_POOL_GetMetaVirAddr(BlockId);

    AX_U64 viraddr = reinterpret_cast<AX_U64>(pviraddr);

    if (!request->pack(response->get_api(), viraddr)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_POOL_IncreaseRefCnt) {
    AX_BLK BlockId;

    if (!response->unpack(response->get_api(), &BlockId)) {
        return false;
    }

    AX_S32 ret = ::AX_POOL_IncreaseRefCnt(BlockId);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_POOL_DecreaseRefCnt) {
    AX_BLK BlockId;

    if (!response->unpack(response->get_api(), &BlockId)) {
        return false;
    }

    AX_S32 ret = ::AX_POOL_DecreaseRefCnt(BlockId);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_SYS_GetCurPTS) {
    AX_U64 u64CurPTS;

    if (!response->unpack(response->get_api())) {
        return false;
    }

    AX_S32 ret = ::AX_SYS_GetCurPTS(&u64CurPTS);

    if (!request->pack(response->get_api(), ret, u64CurPTS)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_SYS_InitPTSBase) {
    AX_U64 u64PTSBase;

    if (!response->unpack(response->get_api(), &u64PTSBase)) {
        return false;
    }

    AX_S32 ret = ::AX_SYS_InitPTSBase(u64PTSBase);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_SYS_SyncPTS) {
    AX_U64 u64PTSBase;

    if (!response->unpack(response->get_api(), &u64PTSBase)) {
        return false;
    }

    AX_S32 ret = ::AX_SYS_SyncPTS(u64PTSBase);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_SYS_GetChipType) {
    AX_U64 u64PTSBase;

    if (!response->unpack(response->get_api(), &u64PTSBase)) {
        return false;
    }

    AX_U32 chipType = ::AX_SYS_GetChipType();

    if (!request->pack(response->get_api(), chipType)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_SYS_SetLogLevel) {
    AX_LOG_LEVEL_E level;

    if (!response->unpack(response->get_api(), &level)) {
        return false;
    }

    AX_S32 ret = ::AX_SYS_SetLogLevel(level);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_SYS_SetLogTarget) {
    AX_LOG_TARGET_E target;

    if (!response->unpack(response->get_api(), &target)) {
        return false;
    }

    AX_S32 ret = ::AX_SYS_SetLogTarget(target);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_SYS_EnableTimestamp) {
    AX_BOOL enable;

    if (!response->unpack(response->get_api(), &enable)) {
        return false;
    }

    AX_S32 ret = ::AX_SYS_EnableTimestamp(enable);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_SYS_Sleep) {
    if (!response->unpack(response->get_api())) {
        return false;
    }

    AX_S32 ret = ::AX_SYS_Sleep();

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_SYS_WakeLock) {
    AX_MOD_ID_E ModId;

    if (!response->unpack(response->get_api(), &ModId)) {
        return false;
    }

    AX_S32 ret = ::AX_SYS_WakeLock(ModId);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_SYS_WakeUnlock) {
    AX_MOD_ID_E ModId;

    if (!response->unpack(response->get_api(), &ModId)) {
        return false;
    }

    AX_S32 ret = ::AX_SYS_WakeUnlock(ModId);

    if (!request->pack(response->get_api(), ret)) {
        return false;
    }

    return true;
}

NATIVE_SYS_API_IMPL(AX_SYS_RegisterEventCb) {
    return false;
}

NATIVE_SYS_API_IMPL(AX_SYS_UnregisterEventCb) {
    return false;
}

}  // namespace axcl::worker
