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
#include "axcl_skel_type.h"
#include "skelInternal.hpp"
#include "def.hpp"
#include "io.hpp"
#include "byteTracker.hpp"

namespace axcl::skel {
#define SKEL_CLASS_NUM 1
#define SKEL_DEFAULT_IO_DEPTH 2
#define SKEL_MAX_IO_DEPTH 4
#define SKEL_DEFAULT_POST_THREAD_NUMS 1

#define SKEL_DEFAULT_CSC_INTF_TYPE AXCL_SKEL_CV_INTF_TYPE_VGP
#define SKEL_DEFAULT_RESIZE_INTF_TYPE AXCL_SKEL_CV_INTF_TYPE_VGP

#define SKEL_RESULT_NOTIFY_ONE() \
    do { \
        std::unique_lock<std::mutex> lck(m_mtxResultQueue); \
        m_cvResultCome.notify_one(); \
    } while(0)

#define NOTIFY_SKEL_RESULT(pstResult) \
        do { \
            std::unique_lock<std::mutex> lck(m_mtxResultQueue); \
            m_qResult.push(pstResult); \
            m_cvResultCome.notify_one(); \
        } while(0)

typedef struct axclSKEL_RUNNING_STATUS_T {
    AX_U64 nFrameTrans;
    AX_U64 nFrameProc;

    axclSKEL_RUNNING_STATUS_T() {
        nFrameTrans = 0;
        nFrameProc = 0;
    }
} AXCL_SKEL_RUNNING_STATUS_T;

typedef struct axclSKEL_CTX_T {
    AX_S32 nDeviceId;
    SKEL_INIT_PARAM_T stInitParam;
    AXCL_SKEL_HANDLE pHandle;
    AXCL_SKEL_PARAM_T stParam;
    AXCL_SKEL_CALLBACK_T stCallback;
    AXCL_SKEL_RUNNING_STATUS_T stRunningStatus;
} AXCL_SKEL_CTX_T;

typedef struct axclSKEL_PRIV_CTX_T {
    AX_ENGINE_HANDLE pHandle;
    AX_ENGINE_MODEL_TYPE_T eModelType;
    AX_ENGINE_IO_T stIO;
    AX_ENGINE_IO_INFO_T* io_info;
    std::array<int, 2> arrInputSize;  // [0]: height; [1]: width;
    std::array<std::vector<axcl::skel::GridAndStride>, 3> arrGridStride;
    AX_IMG_FORMAT_E eDtype;
    AX_VIDEO_FRAME_T *pstIOBuffer;

    // thread pool
    mutex mtxOutputBuffer;
    AX_S32 nThreadNums;
    CThreadPool *pThreadPool;
    std::vector<std::vector<AX_ENGINE_IO_BUFFER_T>> vecOutputBuffer;
    std::vector<std::vector<AXCL_SKEL_IO_BUFFER_T>> vecSkelOutput;
    std::vector<AX_BOOL> vecOutputBufferFlag;
} AXCL_SKEL_PRIV_CTX_T;

///
class CSKEL : public CStage, public IFrameRelease {
public:
    CSKEL(AXCL_SKEL_CTX_T ctx);
    CSKEL(AXCL_SKEL_CTX_T ctx, const string& strName);
    virtual ~CSKEL(AX_VOID) = default;

public:
    virtual AX_S32 CreateEngine(const std::string &strModel, std::array<int, 2> &input_size, AXCL_SKEL_PRIV_CTX_T &stCtx, const std::string& token);
    virtual AX_S32 CreateGridStride(AXCL_SKEL_PRIV_CTX_T &stCtx);
    virtual AX_S32 RunEngine(AXCL_SKEL_PRIV_CTX_T &stCtx, AX_VIDEO_FRAME_T *pstFrame, AX_U32 &nOutBufferId);
    virtual AX_S32 WarmUpEngine(AXCL_SKEL_PRIV_CTX_T &stCtx);
    virtual AX_S32 PreProcess(AXCL_SKEL_PRIV_CTX_T &stCtx, const CSkelFrame *pstFrame, AX_VIDEO_FRAME_T &stDstFrame);
    virtual AX_S32 Create(AX_VOID);
    virtual AX_S32 Destroy(AX_VOID);
    virtual AX_BOOL SkelQueueEmpty(AX_VOID);
    virtual AX_BOOL SkelQueueFull(AX_VOID);
    virtual AX_S32 SkelEnqueue(CSkelFrame *pSkelFrame, AX_S32 nTimeout/* = -1*/);
    virtual AX_VOID WaitSkelQueueEmpty(AX_S32 nTimeout/* = -1*/);
    virtual AX_VOID WaitQueueEmpty();
    virtual AX_S32 SendFrame(const AXCL_SKEL_FRAME_T *pstFrame, AX_S32 nTimeout);
    virtual AX_VOID ResultCallbackThread(AX_VOID);
    virtual AX_S32 RegisterResultCallback(AXCL_SKEL_RESULT_CALLBACK_FUNC callback, AX_VOID *pUserData);
    virtual AX_S32 GetResult(AXCL_SKEL_RESULT_T **ppstResult, AX_S32 nTimeout);
    virtual AXCL_SKEL_PARAM_T GetConfig(AX_VOID);
    virtual AX_S32 SetConfig(AXCL_SKEL_PARAM_T &stConfig);
    virtual AX_S32 SetPrivParam(AX_VOID);
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
                                                const std::array<AXCL_SKEL_FILTER_CONFIG_T, SKEL_CLASS_NUM> &stFilterArr,
                                                const vector<float> &tlwh,
                                                const AX_U32 &nObjectCount);
    virtual AX_S32 CreateThreadPool(AXCL_SKEL_PRIV_CTX_T &stCtx, AX_S32 nThreadNums, const string& threadName);
    virtual AX_U32 GetOutputBufferUnUseId(AXCL_SKEL_PRIV_CTX_T &stCtx);
    virtual AX_U32 SetOutputBufferStat(AXCL_SKEL_PRIV_CTX_T &stCtx, AX_U32 nId, AX_BOOL bStat);

public:
    mutex m_mtxSetting;
    AXCL_SKEL_CTX_T m_ctx;
    queue<AXCL_SKEL_RESULT_T *> m_qResult;
    mutex m_mtxResultQueue;
    condition_variable m_cvResultCome;
    mutex m_mtxStatus;

    AX_BOOL m_bResultThreadRunning{AX_FALSE};
    thread* m_pResultThread{nullptr};

    AX_BOOL m_bDumpSrcImage{AX_FALSE};
    AX_BOOL m_bDumpBodyImage{AX_FALSE};
    AX_BOOL m_bDumpPoseImage{AX_FALSE};

    CBYTETracker *m_pTracker{nullptr};

    AXCL_SKEL_CV_INTF_TYPE_E m_eCscIntfType{SKEL_DEFAULT_CSC_INTF_TYPE};
    AXCL_SKEL_CV_INTF_TYPE_E m_eResizeIntfType{SKEL_DEFAULT_RESIZE_INTF_TYPE};
};
}

using namespace axcl::skel;
