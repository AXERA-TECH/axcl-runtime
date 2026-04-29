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
#include "Singleton.hpp"
#include "skel.hpp"
#include "skelInit.hpp"
#include "hvcp.hpp"
#include "face.hpp"

namespace axcl::skel {
///
#define SKELMGROBJ CSKELMgr::GetInstance()

class CSKELMgr : public CSingleton<CSKELMgr> {
    friend class CSingleton<CSKELMgr>;

public:
    AX_S32 Init(const AXCL_SKEL_INIT_PARAM_T *pstParam);
    AX_S32 DeInit(AX_VOID);

    AX_VOID MgrMemCallback(MEMMGR_ADDR pAddr, AX_VOID *pParam);
    AX_S32 Create(const AXCL_SKEL_HANDLE_PARAM_T *pstParam, AXCL_SKEL_HANDLE *pHandle);
    AX_S32 Destroy(AXCL_SKEL_HANDLE handle);
    AX_S32 SendFrame(AXCL_SKEL_HANDLE handle, const AXCL_SKEL_FRAME_T *pstFrame, AX_S32 nTimeout);
    AX_S32 RegisterResultCallback(AXCL_SKEL_HANDLE handle, AXCL_SKEL_RESULT_CALLBACK_FUNC callback, AX_VOID *pUserData);
    AX_S32 GetResult(AXCL_SKEL_HANDLE handle, AXCL_SKEL_RESULT_T **ppstResult, AX_S32 nTimeout);
    AX_S32 GetCapability(const AXCL_SKEL_CAPABILITY_T **ppstCapability);
    AX_S32 GetVersion(const AXCL_SKEL_VERSION_INFO_T **ppstVersion);
    AX_S32 GetConfig(AXCL_SKEL_HANDLE handle, const AXCL_SKEL_CONFIG_T **ppstConfig);
    AX_S32 SetConfig(AXCL_SKEL_HANDLE handle, const AXCL_SKEL_CONFIG_T *pstConfig);
    AX_S32 Release(AX_VOID *p);

private:
    CSKELMgr(AX_VOID) = default;
    virtual ~CSKELMgr(AX_VOID) = default;

    CSKEL *Lookup(AXCL_SKEL_HANDLE pHandle);

private:
    CSkelMap<AXCL_SKEL_HANDLE, CSKEL *> m_Maps;
    AX_BOOL m_bInited{AX_FALSE};
    CSKELInit *m_pstInitInst{nullptr};
    AX_S32 m_nDeviceId{-1};
};
}

using namespace axcl::skel;
