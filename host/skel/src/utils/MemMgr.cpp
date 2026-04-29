/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "MemMgr.hpp"
#include <errno.h>
#include <string.h>

AX_S32 CMemMgr::Add(MEMMGR_ADDR addr, AX_VOID *pUserData, AX_VOID *pParam, MEMMGR_CALLBACK_FUNC callback) {
    std::lock_guard<std::mutex> lck(m_Mutex);

    auto it = m_Maps.find(addr);
    if (m_Maps.end() != it) {
        MEMMGR_OBJECT_T stObj = it->second;
        Release(addr, stObj);

        m_Maps.erase(it);
    }

    MEMMGR_OBJECT_T stObj = {0};
    stObj.pUserData = pUserData;
    stObj.pParam = pParam;
    stObj.callback = callback;
    m_Maps[addr] = stObj;

    return 0;
}

AX_S32 CMemMgr::Rel(MEMMGR_ADDR addr) {
    std::lock_guard<std::mutex> lck(m_Mutex);

    auto it = m_Maps.find(addr);
    if (m_Maps.end() == it) {
        return -1;
    }

    MEMMGR_OBJECT_T stObj = it->second;
    Release(addr, stObj);

    m_Maps.erase(it);

    return AXCL_SKEL_SUCC;
}

AX_VOID CMemMgr::Release(MEMMGR_ADDR addr, MEMMGR_OBJECT_T &stObj) {
    if (stObj.callback) {
        stObj.callback(addr, stObj.pUserData, stObj.pParam);
    }
}

AX_S32 CMemMgr::Size(AX_VOID) {
    std::lock_guard<std::mutex> lck(m_Mutex);

    return m_Maps.size();
}

AX_S32 CMemMgr::DeInit(AX_VOID) {
    std::lock_guard<std::mutex> lck(m_Mutex);

    if (m_Maps.size() > 0) {
        for (auto &m : m_Maps) {
            MEMMGR_ADDR addr = m.first;
            MEMMGR_OBJECT_T stObj = m.second;

            Release(addr, stObj);
        }
        m_Maps.clear();
    }

    return AXCL_SKEL_SUCC;
}
