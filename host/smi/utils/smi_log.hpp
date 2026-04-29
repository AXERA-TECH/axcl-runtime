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
#include <cstdio>
#include <cstring>

// clang-format off
typedef enum {
    AXCL_SMI_LOG_EMERGENCY  = 0,
    AXCL_SMI_LOG_ALERT      = 1,
    AXCL_SMI_LOG_CRITICAL   = 2,
    AXCL_SMI_LOG_ERROR      = 3,
    AXCL_SMI_LOG_WARN       = 4,
    AXCL_SMI_LOG_NOTICE     = 5,
    AXCL_SMI_LOG_INFO       = 6,
    AXCL_SMI_LOG_DEBUG      = 7
} AXCL_SMI_LOG_LEVEL_E;

extern AXCL_SMI_LOG_LEVEL_E g_axcl_smi_log_level;

#if defined (WINDOWS)
    #define MACRO_BLACK     ""
    #define MACRO_RED       ""
    #define MACRO_GREEN     ""
    #define MACRO_YELLOW    ""
    #define MACRO_BLUE      ""
    #define MACRO_PURPLE    ""
    #define MACRO_WHITE     ""
    #define MACRO_END       ""
#else
    #define MACRO_BLACK     "\033[1;30;30m"
    #define MACRO_RED       "\033[1;30;31m"
    #define MACRO_GREEN     "\033[1;30;32m"
    #define MACRO_YELLOW    "\033[1;30;33m"
    #define MACRO_BLUE      "\033[1;30;34m"
    #define MACRO_PURPLE    "\033[1;30;35m"
    #define MACRO_WHITE     "\033[1;30;37m"
    #define MACRO_END       "\033[0m"
#endif

template <typename... Args>
inline void smi_log(AXCL_SMI_LOG_LEVEL_E lv, const char* tag, const char* clr, const char* func, int line, const char* fmt,
                    Args&&... args) {
    if (g_axcl_smi_log_level >= lv) {
        constexpr const char *LEVEL[] = {"M", "A", "C", "E", "W", "N", "I", "D"};
        auto now = std::chrono::system_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        auto t = std::chrono::system_clock::to_time_t(now);
        auto tm = std::localtime(&t);
        std::printf("%s[%02u-%02u %02u:%02u:%02u:%03u][%s][%s][%s][%04d]: ",
                    clr,
                    tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, static_cast<int>(ms.count()),
                    LEVEL[lv],
                    tag,
                    func,
                    line);
        std::printf(fmt, std::forward<Args>(args)...);
        std::printf("%s\n", MACRO_END);
    }
}

#define LOG_M_A(tag, fmt, ...) smi_log(AXCL_SMI_LOG_ALERT,    tag, MACRO_YELLOW, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_M_C(tag, fmt, ...) smi_log(AXCL_SMI_LOG_CRITICAL, tag, MACRO_YELLOW, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_M_E(tag, fmt, ...) smi_log(AXCL_SMI_LOG_ERROR,    tag, MACRO_RED,    __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_M_W(tag, fmt, ...) smi_log(AXCL_SMI_LOG_WARN,     tag, MACRO_YELLOW, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_M_I(tag, fmt, ...) smi_log(AXCL_SMI_LOG_INFO,     tag, MACRO_GREEN,  __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_M_D(tag, fmt, ...) smi_log(AXCL_SMI_LOG_DEBUG,    tag, MACRO_WHITE,  __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_M_N(tag, fmt, ...) smi_log(AXCL_SMI_LOG_NOTICE,   tag, MACRO_PURPLE, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
// clang-format on
