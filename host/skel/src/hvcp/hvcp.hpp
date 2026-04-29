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
#include <mutex>
#include <queue>
#include <thread>
#include "skel.hpp"
#include "MemMgr.hpp"

namespace axcl::skel {
#define HVCP_CLASS_NUM 4
#define HVCP_DEFAULT_IO_DEPTH 4
#define HVCP_MAX_IO_DEPTH 8
#define HVCP_DEFAULT_POST_THREAD_NUMS 1

typedef struct axclSKEL_PRIV_HVCP_PARAM_T {
    std::array<AXCL_SKEL_FILTER_CONFIG_T, HVCP_CLASS_NUM> stFilterArr;

    axclSKEL_PRIV_HVCP_PARAM_T() {
        memset(&stFilterArr, 0x00, sizeof(stFilterArr));
    }
} AXCL_SKEL_PRIV_HVCP_PARAM_T;

///
class CHVCP : public CSKEL {
public:
    CHVCP(AXCL_SKEL_CTX_T ctx);
    virtual ~CHVCP(AX_VOID) = default;

public:
    virtual AX_S32 Create(AX_VOID);
    virtual AX_S32 Destroy(AX_VOID);
    virtual AX_S32 SendFrame(const AXCL_SKEL_FRAME_T *pstFrame, AX_S32 nTimeout);
    virtual AXCL_SKEL_STAGE_STAT_E ProcessFrame(const CSkelFrame *pstFrame);
    virtual AX_VOID SkelFrameRelease(CSkelFrame *pSkelFrame);
    virtual AX_VOID ImplMemCallback(MEMMGR_ADDR pAddr, AX_VOID *pParam);
    virtual AX_VOID PostProcessHandler(const CSkelFrame *pstFrame,
                                                AXCL_SKEL_RESULT_T *pstResult,
                                                AX_U32 nOutBufferId,
                                                AX_BOOL bSync);

    virtual AX_BOOL ObjectFliterProcess(const AX_CHAR *pstrObjectCategory,
                                                const AX_U32 &nClassId,
                                                const AXCL_SKEL_PARAM_T &stParam,
                                                const std::array<AXCL_SKEL_FILTER_CONFIG_T, HVCP_CLASS_NUM> &stFilterArr,
                                                const vector<float> &tlwh,
                                                const AX_U32 &nObjectCount);
    virtual AX_S32 SetPrivParam(AX_VOID);

private:
    AXCL_SKEL_PRIV_CTX_T m_stHvcpCtx{0};
    AXCL_SKEL_PRIV_HVCP_PARAM_T m_stPrivHvcpParam;
};
}

using namespace axcl::skel;
