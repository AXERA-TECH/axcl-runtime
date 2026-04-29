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

#include <stdio.h>
#include <chrono>
#include <map>
#include <mutex>
#include "def.hpp"

namespace axcl::skel {
typedef struct axELAPSED_TIME_T {
    AX_U64 nElapsed;
    AX_U64 nMinElapsed;
    AX_U64 nMaxElapsed;
    AX_U64 nCount;
    AX_U32 nUnit;

    axELAPSED_TIME_T() {
        nElapsed = 0;
        nMinElapsed = 0;
        nMaxElapsed = 0;
        nCount = 0;
        nUnit = 0;
    };
} AX_ELAPSED_TIME_T;

///
class CElapsed {
public:
    enum { microseconds = 1, milliseconds = 2, seconds = 3 };

    AX_VOID SetStatus(AX_BOOL bEnable) {
        m_bEnable = bEnable;
    }

    AX_VOID Start(AX_VOID) {
        if (!m_bEnable) {
            return;
        }
        m_begin = std::chrono::steady_clock::now();
    }

    AX_VOID Stop(const AX_CHAR *pName, AX_BOOL bPrint = AX_FALSE, AX_U32 u32Unit = CElapsed::microseconds) {
        if (!m_bEnable) {
            return;
        }
        auto end = std::chrono::steady_clock::now();
        AX_U64 nElapsed = 0;
        switch (u32Unit) {
            case CElapsed::seconds: {
                nElapsed = (AX_U64)(std::chrono::duration_cast<std::chrono::seconds>(end - m_begin).count());
            } break;
            case CElapsed::microseconds: {
                nElapsed = (AX_U64)(std::chrono::duration_cast<std::chrono::microseconds>(end - m_begin).count());
            } break;
            default: { nElapsed = (AX_U64)(std::chrono::duration_cast<std::chrono::milliseconds>(end - m_begin).count()); } break;
        }

        Add(pName, nElapsed, bPrint, u32Unit);
    }

    AX_U64 Stop(AX_U32 u32Unit = CElapsed::microseconds) {
        if (!m_bEnable) {
            return 0;
        }
        auto end = std::chrono::steady_clock::now();
        AX_U64 elapsed = 0;
        switch (u32Unit) {
            case CElapsed::seconds:
                elapsed = (AX_U64)(std::chrono::duration_cast<std::chrono::seconds>(end - m_begin).count());
                break;
            case CElapsed::microseconds:
                elapsed = (AX_U64)(std::chrono::duration_cast<std::chrono::microseconds>(end - m_begin).count());
                break;
            default:
                elapsed = (AX_U64)(std::chrono::duration_cast<std::chrono::milliseconds>(end - m_begin).count());
                break;
        }

        return elapsed;
    }

    AX_VOID PrintElapsedInfo(AX_VOID) {
        std::unique_lock<std::mutex> lck(m_mtx);
        if (!m_bEnable) {
            return;
        }
        if (m_ElapsedMaps.size() > 0) {
            printf("======================\n");
            printf("SKEL Elapsed Info: \n");
            for (auto iter = m_ElapsedMaps.begin(); iter != m_ElapsedMaps.end();) {
                const AX_CHAR *strUnit = NULL;
                switch (iter->second.nUnit) {
                    case CElapsed::seconds:
                        strUnit = "s";
                        break;
                    case CElapsed::microseconds:
                        strUnit = "us";
                        break;
                    default:
                        strUnit = "ms";
                        break;
                }
                printf("%s elapsed: count:%lld, min: %lld %s, avr: %lld %s, max: %lld %s\n", iter->first.c_str(), iter->second.nCount,
                       iter->second.nMinElapsed, strUnit, iter->second.nElapsed / (iter->second.nCount ? iter->second.nCount : 1), strUnit,
                       iter->second.nMaxElapsed, strUnit);
                iter++;
            }
            printf("======================\n");
        }
    }

    AX_VOID Add(const AX_CHAR *pName, AX_U64 nElapsed, AX_BOOL bPrint = AX_FALSE, AX_U32 u32Unit = CElapsed::microseconds) {
        std::unique_lock<std::mutex> lck(m_mtx);
        if (!m_bEnable) {
            return;
        }

        std::string name = pName;

        switch (u32Unit) {
            case CElapsed::seconds: {
                if (bPrint) printf("%s elapsed time: %lld s\n", pName, nElapsed);
            } break;
            case CElapsed::microseconds: {
                if (bPrint) printf("%s elapsed time: %lld us\n", pName, nElapsed);
            } break;
            default: {
                if (bPrint) printf("%s elapsed time: %lld ms\n", pName, nElapsed);
            } break;
        }

        AX_BOOL bExist = AX_FALSE;
        if (m_ElapsedMaps.size() > 0) {
            auto it = m_ElapsedMaps.find(name);
            if (m_ElapsedMaps.end() != it) {
                it->second.nElapsed += nElapsed;
                if (nElapsed > it->second.nMaxElapsed) {
                    it->second.nMaxElapsed = nElapsed;
                }
                if (nElapsed < it->second.nMinElapsed) {
                    it->second.nMinElapsed = nElapsed;
                }
                it->second.nCount++;
                bExist = AX_TRUE;
            }
        }

        if (!bExist) {
            AX_ELAPSED_TIME_T stElapsed;
            stElapsed.nElapsed = nElapsed;
            stElapsed.nMinElapsed = nElapsed;
            stElapsed.nMaxElapsed = nElapsed;
            stElapsed.nCount = 1;
            stElapsed.nUnit = u32Unit;
            m_ElapsedMaps[name] = stElapsed;
        }
    }

    AX_VOID Add(const AX_CHAR *pName, std::chrono::steady_clock::time_point begin, AX_BOOL bPrint = AX_FALSE,
                AX_U32 u32Unit = CElapsed::microseconds) {
        if (!m_bEnable) {
            return;
        }
        auto end = std::chrono::steady_clock::now();
        AX_U64 nElapsed = 0;
        switch (u32Unit) {
            case CElapsed::seconds: {
                nElapsed = (AX_U64)(std::chrono::duration_cast<std::chrono::seconds>(end - begin).count());
            } break;
            case CElapsed::microseconds: {
                nElapsed = (AX_U64)(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
            } break;
            default: { nElapsed = (AX_U64)(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()); } break;
        }

        Add(pName, nElapsed, bPrint, u32Unit);
    }

    AX_VOID Reset(AX_VOID) {
        std::unique_lock<std::mutex> lck(m_mtx);
        if (!m_bEnable) {
            return;
        }
        for (auto iter = m_ElapsedMaps.begin(); iter != m_ElapsedMaps.end();) {
            m_ElapsedMaps.erase(iter++);
        }
    }

public:
    CElapsed(AX_VOID) noexcept = default;
    virtual ~CElapsed(AX_VOID) = default;

private:
    std::chrono::steady_clock::time_point m_begin = std::chrono::steady_clock::now();
    CSkelMap<std::string, AX_ELAPSED_TIME_T> m_ElapsedMaps;
    std::mutex m_mtx;
    AX_BOOL m_bEnable{AX_FALSE};
};
}

using namespace axcl::skel;
