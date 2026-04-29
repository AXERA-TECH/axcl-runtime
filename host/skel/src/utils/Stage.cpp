/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "Stage.hpp"
#include "log.hpp"
#include "axcl_rt_context.h"

#define STAGE "STAGE"

#define SKEL_THREAD_SCHED_ENV_STR "SKEL_THREAD_SCHED_SET"
#define SKEL_STAGE_SCHED_TYPE SCHED_FIFO
#define SKEL_STAGE_SCHED_PRIORITY 99

#define SKEL_DEBUG_ENABLE_ENV_STR "SKEL_DEBUG_ENABLE"
#define SKEL_DEBUG_PRINT_INTERVAL (10)
#define SKEL_DEBUG_PRINT_MSLEEP   (100)

CStage::CStage(const string& strName, AX_S32 nDeviceId) {
    m_strStageName = strName;
    m_nDeviceId = nDeviceId;

    {
        AX_CHAR *pEnv = NULL;
        pEnv = getenv(SKEL_DEBUG_ENABLE_ENV_STR);
        if (pEnv) {
            m_bDebugEnable = (AX_BOOL)atoi(pEnv);
        }

        if (m_bDebugEnable) {
            SET_ELAPSE_STATUS(AX_TRUE);
            m_pDebugThread = new thread(&CStage::DebugThreadFunc, this);
        }
        else {
            if (__ELAPSED_SKEL_TIME__) {
                SET_ELAPSE_STATUS(AX_TRUE);
            }
        }
    }

    {
        AX_CHAR *pEnv = NULL;
        AX_BOOL bSchedSet = AX_FALSE;

        pEnv = getenv(SKEL_THREAD_SCHED_ENV_STR);
        if (pEnv) {
            bSchedSet = (AX_BOOL)atoi(pEnv);
        }

        if (bSchedSet) {
            m_stThreadAttr.bExplicitSched = AX_TRUE;
            m_stThreadAttr.nSchedPolicy = SKEL_STAGE_SCHED_TYPE;
            m_stThreadAttr.nSchedPriority = SKEL_STAGE_SCHED_PRIORITY;
        }
        else {
            m_stThreadAttr.bExplicitSched = AX_FALSE;
            m_stThreadAttr.nSchedPolicy = SCHED_OTHER;
            m_stThreadAttr.nSchedPriority = 0;
        }
    }
}

CStage::~CStage() {
}

AX_BOOL CStage::Start() {
    ALOGI("{}, +++", m_strStageName.c_str());

    m_bRecvFrameWorking = AX_TRUE;
    m_bProcessFrameWorking = AX_TRUE;
    m_pProcFrameThread = new thread(&CStage::ProcessFrameThreadFunc, this);

    set_thread_setschedparam(m_pProcFrameThread, m_stThreadAttr);

    ALOGI("{} ---", m_strStageName.c_str());
    return AX_TRUE;
}

AX_VOID CStage::Stop() {
    ALOGI("{} +++", m_strStageName.c_str());

    {
        std::unique_lock<std::mutex> lck(m_mtxFrameQueue);
        m_bRecvFrameWorking = AX_FALSE;
    }

    // wait queue empty
    WaitQueueEmpty();

    {
        std::unique_lock<std::mutex> lck(m_mtxFrameQueue);
        m_bProcessFrameWorking = AX_FALSE;
        m_cvFrameCome.notify_one();
    }

    if (m_pProcFrameThread) {
        m_pProcFrameThread->join();
        delete m_pProcFrameThread;
        m_pProcFrameThread = nullptr;
    }

    m_bDebugWorking = AX_FALSE;

    if (m_pDebugThread) {
        m_pDebugThread->join();
        delete m_pDebugThread;
        m_pDebugThread = nullptr;
    }

    ALOGI("{} ---", m_strStageName.c_str());
}

AX_BOOL CStage::EnqueueFrame(CSkelFrame* pFrame) {
    std::unique_lock<std::mutex> lck(m_mtxFrameQueue);
    if (m_bRecvFrameWorking) {
        if (m_nQueueCapacity > 0) {
            if (m_qFrame.size() >= m_nQueueCapacity) {
                return AX_FALSE;
            }
        }
        m_qFrame.push(pFrame);
        m_cvFrameCome.notify_one();
        return AX_TRUE;
    }
    return AX_FALSE;
}

CStage* CStage::BindNextStage(CStage* pNext) {
    if (!pNext) {
        return nullptr;
    }
    m_pNextStage = pNext;
    return pNext;
}

CStage* CStage::GetNextStage() {
    return m_pNextStage;
}

AX_BOOL CStage::IsDataPrepared() {
    std::unique_lock<std::mutex> lck(m_mtxFrameQueue);
    return m_qFrame.empty() ? AX_FALSE : AX_TRUE;
}

AX_BOOL CStage::QueueEmpty(AX_VOID) {
    std::unique_lock<std::mutex> lck(m_mtxFrameQueue);
    return (AX_BOOL)m_qFrame.empty();
}

AX_VOID CStage::WaitQueueEmpty(AX_VOID) {
    while (1) {
        if (QueueEmpty()) {
            break;
        }

        CTimeUtils::msSleep(10);
    }
}

AX_VOID CStage::ProcessFrameThreadFunc() {
    ALOGI("{} +++", m_strStageName.c_str());

    /* create thread context */
    axclrtContext context;
    if (axclError ret = axclrtCreateContext(&context, m_nDeviceId); AXCL_SUCC != ret) {
        return;
    }

    char szThreadName[64] = {0};
    sprintf(&szThreadName[0], "%s", m_strStageName.c_str());
    prctl(PR_SET_NAME, szThreadName);
    CSkelFrame* pFrame = nullptr;
    while (m_bProcessFrameWorking) {
        pFrame = nullptr;
        {
            std::unique_lock<std::mutex> lck(m_mtxFrameQueue);
            m_cvFrameCome.wait(lck, [this]() { return (!m_qFrame.empty() || !m_bProcessFrameWorking); });

            if (!m_bProcessFrameWorking) {
                break;
            }
            if (m_qFrame.size() == 0) {
                continue;
            }
            pFrame = m_qFrame.front();
            m_qFrame.pop();
        }

        auto ret = ProcessFrame(pFrame);

        if (ret != AXCL_SKEL_STAGE_STAT_FAILURE) {
            if (GetNextStage()) {
                if (GetNextStage()->EnqueueFrame(pFrame)) {
                    continue;
                }
            }
        }

        if (ret != AXCL_SKEL_STAGE_STAT_CONTINUE) {
            pFrame->FreeMem();
        }
    }

    /* destory thread context */
    axclrtDestroyContext(context);

    ALOGI("{} ---", m_strStageName.c_str());
}

AX_VOID CStage::DebugThreadFunc(AX_VOID) {
    prctl(PR_SET_NAME, "skel_dbg_thread");

    AX_U64 m_nTickStart = CTimeUtils::GetTickCount();
    AX_U64 m_nTickEnd = m_nTickStart;

    m_nTickStart = CTimeUtils::GetTickCount();

    m_bDebugWorking = AX_TRUE;

    while (m_bDebugWorking) {
        m_nTickEnd = CTimeUtils::GetTickCount();
        if ((m_nTickEnd - m_nTickStart) >= SKEL_DEBUG_PRINT_INTERVAL * 1000) {

            PRINT_ELAPSE_INFO;
            RESET_ELAPSE_INFO;

            m_nTickStart = m_nTickEnd;
        }

        CTimeUtils::msSleep(SKEL_DEBUG_PRINT_MSLEEP);
    }
}
