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

#include <chrono>
#include <signal.h>
#include <time.h>
#include "ax_global_type.h"

namespace axcl::skel {
class CTimeUtils {
public:
    CTimeUtils(void);
    virtual ~CTimeUtils(void);

public:
    static AX_U64  GetTickCount(void) {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
    }
    static void msSleep(AX_U32 milliseconds) {
        struct timespec ts = {
            (time_t)(milliseconds / 1000),
            (long)((milliseconds % 1000) * 1000000)
            };
        while ((-1 == nanosleep(&ts, &ts)) && (EINTR == errno));
    }
    static void usSleep(AX_U32 microseconds) {
        struct timespec ts = {
            (time_t)(microseconds / 1000000),
            (long)((microseconds % 1000000) * 1000)
            };
        while ((-1 == nanosleep(&ts, &ts)) && (EINTR == errno));
    }
    static void nsSleep(AX_U32 nanoseconds) {
        struct timespec ts = {
            (time_t)(nanoseconds / 1000000000),
            (long)(nanoseconds % 1000000000)
            };
        while ((-1 == nanosleep(&ts, &ts)) && (EINTR == errno));
    }
    static AX_CHAR* GetCurrTimeStr(AX_CHAR* szOut) {
        auto tp = std::chrono::system_clock::now();
        auto t = std::chrono::system_clock::to_time_t(tp);

        struct tm tm;
        localtime_r(&t, &tm);
        snprintf((char *)szOut, 10, "%02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec);

        return szOut;
    }
};
}

using namespace axcl::skel;
