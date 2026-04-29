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

#include <sys/prctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mutex>
#include <queue>
#include <thread>
#include "SkelFrame.hpp"
#include "utils.hpp"
#include "elapsed.hpp"
#include "TimeUtil.hpp"
#include <condition_variable>

namespace axcl::skel {
using namespace std;

typedef enum {
    AXCL_SKEL_STAGE_STAT_FINALIZE,
    AXCL_SKEL_STAGE_STAT_FAILURE,
    AXCL_SKEL_STAGE_STAT_CONTINUE,
    AXCL_SKEL_STAGE_STAT_BUTT
} AXCL_SKEL_STAGE_STAT_E;

#define SET_ELAPSE_STATUS(status) m_elapsed.SetStatus(status)
#define START_RECORD_TIME m_elapsed.Start()
#define PRINT_ELAPSE_TIME m_elapsed.Stop(__func__, __PRINT_SKEL_TIME__)
#define PRINT_ELAPSE_TIME_WITH_FUNC(fn) m_elapsed.Stop(fn, __PRINT_SKEL_TIME__)
#define PRINT_ELAPSE_INFO m_elapsed.PrintElapsedInfo()
#define RESET_ELAPSE_INFO m_elapsed.Reset()
#define ADD_ELAPSE_TIME(fn, begin) m_elapsed.Add(fn, begin, __PRINT_SKEL_TIME__)

class CStage {
public:
    CStage(const string& strName, AX_S32 nDeviceId);
    virtual ~CStage();

public:
    virtual CStage* BindNextStage(CStage* pNext);
    virtual CStage* GetNextStage();

    virtual AX_BOOL Init() {
        return AX_TRUE;
    };
    virtual AX_VOID DeInit(){};

    virtual AX_BOOL Start();
    virtual AX_VOID Stop();

    virtual AX_BOOL IsDataPrepared();
    virtual AX_BOOL EnqueueFrame(CSkelFrame* pFrame);
    virtual AXCL_SKEL_STAGE_STAT_E ProcessFrame(const CSkelFrame* pFrame) {
        return AXCL_SKEL_STAGE_STAT_FINALIZE;
    };
    virtual AX_BOOL QueueEmpty();
    virtual AX_VOID WaitQueueEmpty();
    virtual AX_VOID ProcessFrameThreadFunc();
    virtual string GetStageName() {
        return m_strStageName;
    };
    AX_BOOL SetCapacity(AX_U32 nCapacity) {
        std::unique_lock<std::mutex> lck(m_mtxFrameQueue);
        if (nCapacity > 0) {
            m_nQueueCapacity = nCapacity;
        }
        return AX_TRUE;
    };
    virtual AX_VOID DebugThreadFunc(AX_VOID);

public:
    queue<CSkelFrame*> m_qFrame;
    mutex m_mtxFrameQueue;
    condition_variable m_cvFrameCome;
    AX_BOOL m_bRecvFrameWorking{AX_FALSE};
    AX_BOOL m_bProcessFrameWorking{AX_FALSE};
    CElapsed m_elapsed;
    THREAD_ATTR_T m_stThreadAttr;

protected:
    string m_strStageName;
    CStage* m_pNextStage{nullptr};
    thread* m_pProcFrameThread{nullptr};
    AX_U32 m_nQueueCapacity{0};
    AX_BOOL m_bDebugEnable{AX_FALSE};
    AX_BOOL m_bDebugWorking{AX_FALSE};
    thread* m_pDebugThread{nullptr};
    AX_S32 m_nDeviceId{-1};
};
}

using namespace axcl::skel;
