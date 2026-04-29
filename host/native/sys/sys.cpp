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
#include <string.h>

AX_S32 sys::AX_SYS_Init() {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_SYS_Init);
    AX_S32 ret;

    if (!m_req.pack(api)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 sys::AX_SYS_Deinit() {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_SYS_Deinit);
    AX_S32 ret;

    if (!m_req.pack(api)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 sys::AX_SYS_MemAlloc(AX_U64 *phyaddr, AX_VOID **pviraddr, AX_U32 size, AX_U32 align, const AX_S8 *token) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_SYS_MemAlloc);
    AX_S32 ret;

    AX_U64 viraddr = (pviraddr) ? reinterpret_cast<AX_U64>(pviraddr) : 0;
    axcl::pkg::uint8_array name;
    if (token) {
        name.data = reinterpret_cast<uint8_t *>(const_cast<AX_S8 *>(token));
        name.size = strlen(reinterpret_cast<const char *>(token));
    }

    if (!m_req.pack(api, viraddr, size, align, name)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, phyaddr, &viraddr)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_DECODE);
    }

    if (pviraddr) {
        *pviraddr = reinterpret_cast<AX_VOID *>(viraddr);
    }
    return ret;
}

AX_S32 sys::AX_SYS_MemAllocCached(AX_U64 *phyaddr, AX_VOID **pviraddr, AX_U32 size, AX_U32 align, const AX_S8 *token) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_SYS_MemAllocCached);
    AX_S32 ret;

    AX_U64 viraddr = 0;
    axcl::pkg::uint8_array name;
    if (token) {
        name.data = reinterpret_cast<uint8_t *>(const_cast<AX_S8 *>(token));
        name.size = strlen(reinterpret_cast<const char *>(token));
    }

    if (!m_req.pack(api, size, align, name)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, phyaddr, &viraddr)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_DECODE);
    }

    *pviraddr = reinterpret_cast<AX_VOID *>(viraddr);
    return ret;
}

AX_S32 sys::AX_SYS_MemFree(AX_U64 phyaddr, AX_VOID *pviraddr) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_SYS_MemFree);
    AX_S32 ret;

    AX_U64 viraddr = reinterpret_cast<AX_U64>(pviraddr);
    if (!m_req.pack(api, phyaddr, viraddr)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_VOID *sys::AX_SYS_Mmap(AX_U64 phyaddr, AX_U32 size) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_SYS_Mmap);
    AX_S32 ret;

    if (!m_req.pack(api, phyaddr, size)) {
        return AX_NULL;
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return AX_NULL;
    }

    AX_U64 viraddr;
    if (!m_rsp.unpack(api, &viraddr)) {
        return AX_NULL;
    }

    return reinterpret_cast<AX_VOID *>(viraddr);
}

AX_VOID *sys::AX_SYS_MmapCache(AX_U64 phyaddr, AX_U32 size) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_SYS_MmapCache);
    AX_S32 ret;

    if (!m_req.pack(api, phyaddr, size)) {
        return AX_NULL;
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return AX_NULL;
    }

    AX_U64 viraddr;
    if (!m_rsp.unpack(api, &viraddr)) {
        return AX_NULL;
    }

    return reinterpret_cast<AX_VOID *>(viraddr);
}

AX_VOID *sys::AX_SYS_MmapFast(AX_U64 phyaddr, AX_U32 size) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_SYS_MmapFast);
    AX_S32 ret;

    if (!m_req.pack(api, phyaddr, size)) {
        return AX_NULL;
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return AX_NULL;
    }

    AX_U64 viraddr;
    if (!m_rsp.unpack(api, &viraddr)) {
        return AX_NULL;
    }

    return reinterpret_cast<AX_VOID *>(viraddr);
}

AX_VOID *sys::AX_SYS_MmapCacheFast(AX_U64 phyaddr, AX_U32 size) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_SYS_MmapCacheFast);
    AX_S32 ret;

    if (!m_req.pack(api, phyaddr, size)) {
        return AX_NULL;
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return AX_NULL;
    }

    AX_U64 viraddr;
    if (!m_rsp.unpack(api, &viraddr)) {
        return AX_NULL;
    }

    return reinterpret_cast<AX_VOID *>(viraddr);
}

AX_S32 sys::AX_SYS_Munmap(AX_VOID *pviraddr, AX_U32 size) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_SYS_Munmap);
    AX_S32 ret;

    AX_U64 viraddr = reinterpret_cast<AX_U64>(pviraddr);
    if (!m_req.pack(api, viraddr, size)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 sys::AX_SYS_MflushCache(AX_U64 phyaddr, AX_VOID *pviraddr, AX_U32 size) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_SYS_MflushCache);
    AX_S32 ret;

    AX_U64 viraddr = reinterpret_cast<AX_U64>(pviraddr);
    if (!m_req.pack(api, phyaddr, viraddr, size)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 sys::AX_SYS_MinvalidateCache(AX_U64 phyaddr, AX_VOID *pviraddr, AX_U32 size) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_SYS_MinvalidateCache);
    AX_S32 ret;

    AX_U64 viraddr = reinterpret_cast<AX_U64>(pviraddr);
    if (!m_req.pack(api, phyaddr, viraddr, size)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 sys::AX_SYS_MemGetBlockInfoByPhy(AX_U64 phyaddr, AX_S32 *pmemType, AX_VOID **pviraddr, AX_U32 *pblockSize) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_SYS_MemGetBlockInfoByPhy);
    AX_S32 ret;

    if (!m_req.pack(api, phyaddr)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    AX_U64 viraddr;
    if (!m_rsp.unpack(api, &ret, pmemType, &viraddr, pblockSize)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_DECODE);
    }

    *pviraddr = reinterpret_cast<AX_VOID *>(viraddr);
    return ret;
}

AX_S32 sys::AX_SYS_MemGetBlockInfoByVirt(AX_VOID *pviraddr, AX_U64 *pphyaddr, AX_S32 *pmemType) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_SYS_MemGetBlockInfoByVirt);
    AX_S32 ret;

    AX_U64 viraddr = reinterpret_cast<AX_U64>(pviraddr);
    if (!m_req.pack(api, viraddr)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pphyaddr, pmemType)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 sys::AX_SYS_MemGetPartitionInfo(AX_CMM_PARTITION_INFO_T *pCmmPartitionInfo) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_SYS_MemGetPartitionInfo);
    AX_S32 ret;

    if (!m_req.pack(api)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pCmmPartitionInfo)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 sys::AX_SYS_MemSetConfig(const AX_MOD_INFO_T *pModInfo, const AX_S8 *pPartitionName) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_SYS_MemSetConfig);
    AX_S32 ret;

    axcl::pkg::uint8_array name;

    name.data = reinterpret_cast<uint8_t *>(const_cast<AX_S8 *>(pPartitionName));
    name.size = strlen(reinterpret_cast<const char *>(pPartitionName));

    if (!m_req.pack(api, pModInfo, name)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 sys::AX_SYS_MemGetConfig(const AX_MOD_INFO_T *pModInfo, AX_S8 *pPartitionName) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_SYS_MemGetConfig);
    AX_S32 ret;

    if (!m_req.pack(api, pModInfo)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    axcl::pkg::uint8_array name;
    if (!m_rsp.unpack(api, &ret, &name)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_DECODE);
    }

    strcpy(reinterpret_cast<char *>(pPartitionName), reinterpret_cast<const char *>(name.data));
    return ret;
}

AX_S32 sys::AX_SYS_MemQueryStatus(AX_CMM_STATUS_T *pCmmStatus) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_SYS_MemQueryStatus);
    AX_S32 ret;

    if (!m_req.pack(api)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pCmmStatus)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 sys::AX_SYS_Link(const AX_MOD_INFO_T *pSrc, const AX_MOD_INFO_T *pDest) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_SYS_Link);
    AX_S32 ret;

    if (!m_req.pack(api, pSrc, pDest)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 sys::AX_SYS_UnLink(const AX_MOD_INFO_T *pSrc, const AX_MOD_INFO_T *pDest) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_SYS_UnLink);
    AX_S32 ret;

    if (!m_req.pack(api, pSrc, pDest)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 sys::AX_SYS_GetLinkByDest(const AX_MOD_INFO_T *pDest, AX_MOD_INFO_T *pSrc) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_SYS_GetLinkByDest);
    AX_S32 ret;

    if (!m_req.pack(api, pDest)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pSrc)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 sys::AX_SYS_GetLinkBySrc(const AX_MOD_INFO_T *pSrc, AX_LINK_DEST_T *pLinkDest) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_SYS_GetLinkBySrc);
    AX_S32 ret;

    if (!m_req.pack(api, pSrc)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pLinkDest)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 sys::AX_POOL_SetConfig(const AX_POOL_FLOORPLAN_T *pPoolFloorPlan) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_POOL_SetConfig);
    AX_S32 ret;

    if (!m_req.pack(api, pPoolFloorPlan)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 sys::AX_POOL_GetConfig(AX_POOL_FLOORPLAN_T *pPoolFloorPlan) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_POOL_GetConfig);
    AX_S32 ret;

    if (!m_req.pack(api)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pPoolFloorPlan)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 sys::AX_POOL_Init() {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_POOL_Init);
    AX_S32 ret;

    if (!m_req.pack(api)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 sys::AX_POOL_Exit(AX_VOID) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_POOL_Exit);
    AX_S32 ret;

    if (!m_req.pack(api)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_POOL sys::AX_POOL_CreatePool(AX_POOL_CONFIG_T *pPoolConfig) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_POOL_CreatePool);
    AX_S32 ret;

    if (!pPoolConfig) {
        return AX_INVALID_POOLID;
    }

    if (!m_req.pack(api, pPoolConfig)) {
        return AX_INVALID_POOLID;
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return AX_INVALID_POOLID;
    }

    AX_POOL PoolId;
    if (!m_rsp.unpack(api, &PoolId)) {
        return AX_INVALID_POOLID;
    }

    return PoolId;
}

AX_S32 sys::AX_POOL_DestroyPool(AX_POOL PoolId) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_POOL_DestroyPool);
    AX_S32 ret;

    if (!m_req.pack(api, PoolId)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_BLK sys::AX_POOL_GetBlock(AX_POOL PoolId, AX_U64 BlkSize, const AX_S8 *pPartitionName) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_POOL_GetBlock);
    AX_S32 ret;

    axcl::pkg::uint8_array name;
    if (pPartitionName) {
        name.data = reinterpret_cast<uint8_t *>(const_cast<AX_S8 *>(pPartitionName));
        name.size = strlen(reinterpret_cast<const char *>(pPartitionName));
    }

    if (!m_req.pack(api, PoolId, BlkSize, name)) {
        return AX_INVALID_BLOCKID;
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return AX_INVALID_BLOCKID;
    }

    AX_POOL BlockId;
    if (!m_rsp.unpack(api, &BlockId)) {
        return AX_INVALID_BLOCKID;
    }

    return BlockId;
}

AX_S32 sys::AX_POOL_ReleaseBlock(AX_BLK BlockId) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_POOL_ReleaseBlock);
    AX_S32 ret;

    if (!m_req.pack(api, BlockId)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_BLK sys::AX_POOL_PhysAddr2Handle(AX_U64 PhysAddr) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_POOL_PhysAddr2Handle);
    AX_S32 ret;

    if (!m_req.pack(api, PhysAddr)) {
        return AX_INVALID_BLOCKID;
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return AX_INVALID_BLOCKID;
    }

    AX_POOL BlockId;
    if (!m_rsp.unpack(api, &BlockId)) {
        return AX_INVALID_BLOCKID;
    }

    return BlockId;
}

AX_U64 sys::AX_POOL_Handle2PhysAddr(AX_BLK BlockId) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_POOL_Handle2PhysAddr);
    AX_S32 ret;

    if (!m_req.pack(api, BlockId)) {
        return 0;
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return 0;
    }

    AX_U64 phyaddr;
    if (!m_rsp.unpack(api, &phyaddr)) {
        return 0;
    }

    return phyaddr;
}

AX_U64 sys::AX_POOL_Handle2MetaPhysAddr(AX_BLK BlockId) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_POOL_Handle2MetaPhysAddr);
    AX_S32 ret;

    if (!m_req.pack(api, BlockId)) {
        return 0;
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return 0;
    }

    AX_U64 phyaddr;
    if (!m_rsp.unpack(api, &phyaddr)) {
        return 0;
    }

    return phyaddr;
}

AX_POOL sys::AX_POOL_Handle2PoolId(AX_BLK BlockId) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_POOL_Handle2PoolId);
    AX_S32 ret;

    if (!m_req.pack(api, BlockId)) {
        return AX_INVALID_POOLID;
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return AX_INVALID_POOLID;
    }

    AX_POOL PoolId;
    if (!m_rsp.unpack(api, &PoolId)) {
        return AX_INVALID_POOLID;
    }

    return PoolId;
}

AX_U64 sys::AX_POOL_Handle2BlkSize(AX_BLK BlockId) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_POOL_Handle2BlkSize);
    AX_S32 ret;

    if (!m_req.pack(api, BlockId)) {
        return 0;
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return 0;
    }

    AX_U64 blkSize;
    if (!m_rsp.unpack(api, &blkSize)) {
        return 0;
    }

    return blkSize;
}

AX_S32 sys::AX_POOL_MmapPool(AX_POOL PoolId) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_POOL_MmapPool);
    AX_S32 ret;

    if (!m_req.pack(api, PoolId)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 sys::AX_POOL_MunmapPool(AX_POOL PoolId) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_POOL_MunmapPool);
    AX_S32 ret;

    if (!m_req.pack(api, PoolId)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_VOID *sys::AX_POOL_GetBlockVirAddr(AX_BLK BlockId) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_POOL_GetBlockVirAddr);
    AX_S32 ret;

    if (!m_req.pack(api, BlockId)) {
        return AX_NULL;
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return AX_NULL;
    }

    AX_U64 viraddr;
    if (!m_rsp.unpack(api, &viraddr)) {
        return AX_NULL;
    }

    return reinterpret_cast<AX_VOID *>(viraddr);
}

AX_VOID *sys::AX_POOL_GetMetaVirAddr(AX_BLK BlockId) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_POOL_GetMetaVirAddr);
    AX_S32 ret;

    if (!m_req.pack(api, BlockId)) {
        return AX_NULL;
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return AX_NULL;
    }

    AX_U64 viraddr;
    if (!m_rsp.unpack(api, &viraddr)) {
        return AX_NULL;
    }

    return reinterpret_cast<AX_VOID *>(viraddr);
}

AX_S32 sys::AX_POOL_IncreaseRefCnt(AX_BLK BlockId) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_POOL_IncreaseRefCnt);
    AX_S32 ret;

    if (!m_req.pack(api, BlockId)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 sys::AX_POOL_DecreaseRefCnt(AX_BLK BlockId) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_POOL_DecreaseRefCnt);
    AX_S32 ret;

    if (!m_req.pack(api, BlockId)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 sys::AX_SYS_GetCurPTS(AX_U64 *pu64CurPTS) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_SYS_GetCurPTS);
    AX_S32 ret;

    if (!m_req.pack(api)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret, pu64CurPTS)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 sys::AX_SYS_InitPTSBase(AX_U64 u64PTSBase) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_SYS_InitPTSBase);
    AX_S32 ret;

    if (!m_req.pack(api, u64PTSBase)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 sys::AX_SYS_SyncPTS(AX_U64 u64PTSBase) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_SYS_SyncPTS);
    AX_S32 ret;

    if (!m_req.pack(api, u64PTSBase)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_CHIP_TYPE_E sys::AX_SYS_GetChipType() {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_SYS_GetChipType);
    AX_S32 ret;

    if (!m_req.pack(api)) {
        return NONE_CHIP_TYPE;
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return NONE_CHIP_TYPE;
    }

    AX_U32 chipType;
    if (!m_rsp.unpack(api, &chipType)) {
        return NONE_CHIP_TYPE;
    }

    return static_cast<AX_CHIP_TYPE_E>(chipType);
}

AX_S32 sys::AX_SYS_SetLogLevel(AX_LOG_LEVEL_E level) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_SYS_SetLogLevel);
    AX_S32 ret;

    if (!m_req.pack(api, level)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 sys::AX_SYS_SetLogTarget(AX_LOG_TARGET_E target) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_SYS_SetLogTarget);
    AX_S32 ret;

    if (!m_req.pack(api, target)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 sys::AX_SYS_EnableTimestamp(AX_BOOL enable) {
    axcl::pkg::native::SYS_API api(axcl::pkg::native::SYS_API::AX_SYS_EnableTimestamp);
    AX_S32 ret;

    if (!m_req.pack(api, enable)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_ENCODE);
    }

    if (ret = invoke(NO_TIMEOUT); AXCL_SUCC != ret) {
        return ret;
    }

    if (!m_rsp.unpack(api, &ret)) {
        return AXCL_DEF_SYS_ERR(AXCL_ERR_DECODE);
    }

    return ret;
}

AX_S32 sys::AX_SYS_Sleep() {
    return AXCL_DEF_SYS_ERR(AXCL_ERR_UNSUPPORT);
}

AX_S32 sys::AX_SYS_WakeLock(const AX_MOD_ID_E ModId) {
    return AXCL_DEF_SYS_ERR(AXCL_ERR_UNSUPPORT);
}

AX_S32 sys::AX_SYS_WakeUnlock(const AX_MOD_ID_E ModId) {
    return AXCL_DEF_SYS_ERR(AXCL_ERR_UNSUPPORT);
}

AX_S32 sys::AX_SYS_RegisterEventCb(const AX_MOD_ID_E ModId, NotifyEventCallback pFunction, AX_VOID *pData) {
    return AXCL_DEF_SYS_ERR(AXCL_ERR_UNSUPPORT);
}

AX_S32 sys::AX_SYS_UnregisterEventCb(const AX_MOD_ID_E ModId) {
    return AXCL_DEF_SYS_ERR(AXCL_ERR_UNSUPPORT);
}
