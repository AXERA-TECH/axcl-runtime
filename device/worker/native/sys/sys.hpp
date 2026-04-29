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

#include "package/device/native/module/sys_request.hpp"
#include "package/device/native/module/sys_response.hpp"
#include "ax_sys_api.h"
#include "base.hpp"
#include "native_api.hpp"

#define NATIVE_SYS_API_DECL(_API_) \
            NATIVE_API_DECL(_API_, NATIVE_API_PARAM(sys))

#define NATIVE_SYS_API_IMPL(_API_) \
            NATIVE_API_IMPL(sys, _API_, NATIVE_API_PARAM(sys))

#define NATIVE_SYS_API_APPEND(_API_) \
            NATIVE_API_APPEND(SYS, _API_, NATIVE_API_PARAM(sys))

namespace axcl::worker {

class dispatcher;
class sys : public base {
public:
    sys(dispatcher *dispatcher);
    ~sys() = default;

    virtual void run(const void *data, const size_t& size) override;

private:
    NATIVE_SYS_API_DECL(AX_SYS_Init);
    NATIVE_SYS_API_DECL(AX_SYS_Deinit);
    NATIVE_SYS_API_DECL(AX_SYS_MemAlloc);
    NATIVE_SYS_API_DECL(AX_SYS_MemAllocCached);
    NATIVE_SYS_API_DECL(AX_SYS_MemFree);
    NATIVE_SYS_API_DECL(AX_SYS_Mmap);
    NATIVE_SYS_API_DECL(AX_SYS_MmapCache);
    NATIVE_SYS_API_DECL(AX_SYS_MmapFast);
    NATIVE_SYS_API_DECL(AX_SYS_MmapCacheFast);
    NATIVE_SYS_API_DECL(AX_SYS_Munmap);
    NATIVE_SYS_API_DECL(AX_SYS_MflushCache);
    NATIVE_SYS_API_DECL(AX_SYS_MinvalidateCache);
    NATIVE_SYS_API_DECL(AX_SYS_MemGetBlockInfoByPhy);
    NATIVE_SYS_API_DECL(AX_SYS_MemGetBlockInfoByVirt);
    NATIVE_SYS_API_DECL(AX_SYS_MemGetPartitionInfo);
    NATIVE_SYS_API_DECL(AX_SYS_MemSetConfig);
    NATIVE_SYS_API_DECL(AX_SYS_MemGetConfig);
    NATIVE_SYS_API_DECL(AX_SYS_MemQueryStatus);
    NATIVE_SYS_API_DECL(AX_SYS_Link);
    NATIVE_SYS_API_DECL(AX_SYS_UnLink);
    NATIVE_SYS_API_DECL(AX_SYS_GetLinkByDest);
    NATIVE_SYS_API_DECL(AX_SYS_GetLinkBySrc);
    NATIVE_SYS_API_DECL(AX_POOL_SetConfig);
    NATIVE_SYS_API_DECL(AX_POOL_GetConfig);
    NATIVE_SYS_API_DECL(AX_POOL_Init);
    NATIVE_SYS_API_DECL(AX_POOL_Exit);
    NATIVE_SYS_API_DECL(AX_POOL_CreatePool);
    NATIVE_SYS_API_DECL(AX_POOL_DestroyPool);
    NATIVE_SYS_API_DECL(AX_POOL_GetBlock);
    NATIVE_SYS_API_DECL(AX_POOL_ReleaseBlock);
    NATIVE_SYS_API_DECL(AX_POOL_PhysAddr2Handle);
    NATIVE_SYS_API_DECL(AX_POOL_Handle2PhysAddr);
    NATIVE_SYS_API_DECL(AX_POOL_Handle2MetaPhysAddr);
    NATIVE_SYS_API_DECL(AX_POOL_Handle2PoolId);
    NATIVE_SYS_API_DECL(AX_POOL_Handle2BlkSize);
    NATIVE_SYS_API_DECL(AX_POOL_MmapPool);
    NATIVE_SYS_API_DECL(AX_POOL_MunmapPool);
    NATIVE_SYS_API_DECL(AX_POOL_GetBlockVirAddr);
    NATIVE_SYS_API_DECL(AX_POOL_GetMetaVirAddr);
    NATIVE_SYS_API_DECL(AX_POOL_IncreaseRefCnt);
    NATIVE_SYS_API_DECL(AX_POOL_DecreaseRefCnt);
    NATIVE_SYS_API_DECL(AX_SYS_GetCurPTS);
    NATIVE_SYS_API_DECL(AX_SYS_InitPTSBase);
    NATIVE_SYS_API_DECL(AX_SYS_SyncPTS);
    NATIVE_SYS_API_DECL(AX_SYS_GetChipType);
    NATIVE_SYS_API_DECL(AX_SYS_SetLogLevel);
    NATIVE_SYS_API_DECL(AX_SYS_SetLogTarget);
    NATIVE_SYS_API_DECL(AX_SYS_EnableTimestamp);
    NATIVE_SYS_API_DECL(AX_SYS_Sleep);
    NATIVE_SYS_API_DECL(AX_SYS_WakeLock);
    NATIVE_SYS_API_DECL(AX_SYS_WakeUnlock);
    NATIVE_SYS_API_DECL(AX_SYS_RegisterEventCb);
    NATIVE_SYS_API_DECL(AX_SYS_UnregisterEventCb);

private:
    native_api<axcl::pkg::native::SYS_API, axcl::pkg::device::sys_response, axcl::pkg::device::sys_request> m_api;
};

}  // namespace axcl::worker
