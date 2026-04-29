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
#include <map>
#include <mutex>
#include "def.hpp"
#include "Singleton.hpp"

namespace axcl::skel {
///
#define MEMMGROBJ CMemMgr::GetInstance()
typedef AX_VOID *MEMMGR_ADDR;
typedef AX_U64 MEMMGR_PARAM_T;

typedef AX_VOID (*MEMMGR_CALLBACK_FUNC)(MEMMGR_ADDR pAddr, AX_VOID *pUserData, AX_VOID *pParam);

typedef struct _MEMMGR_OBJECT_T {
    AX_VOID *pUserData;
    AX_VOID *pParam;
    MEMMGR_CALLBACK_FUNC callback;
} MEMMGR_OBJECT_T;

class CMemMgr : public CSingleton<CMemMgr> {
    friend class CSingleton<CMemMgr>;

public:
    AX_S32 Add(MEMMGR_ADDR addr, AX_VOID *pUserData, AX_VOID *pParam, MEMMGR_CALLBACK_FUNC callback);
    AX_S32 Rel(MEMMGR_ADDR addr);
    AX_S32 Size(AX_VOID);
    AX_S32 DeInit(AX_VOID);

protected:
    AX_VOID Release(MEMMGR_ADDR addr, MEMMGR_OBJECT_T &stObj);

private:
    CMemMgr(AX_VOID) noexcept = default;
    virtual ~CMemMgr(AX_VOID) = default;

    /* virtual function of CSingleton */
    AX_BOOL Init(AX_VOID) {
        return AX_TRUE;
    };

private:
    std::mutex m_Mutex;
    CSkelMap<MEMMGR_ADDR, MEMMGR_OBJECT_T> m_Maps;
};
}

using namespace axcl::skel;
