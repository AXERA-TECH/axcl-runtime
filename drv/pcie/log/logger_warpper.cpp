/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/
#include "logger_warpper.h"
#include "axcl_logger.hpp"
#include <string.h>
#include <stdarg.h>

static void axcl_pcie_fmt_log(spdlog::level::level_enum lv, const char *fmt, va_list args) {
    char buf[1024];
    int len = vsnprintf(buf, sizeof(buf), fmt, args);
    if (len > 0) {
        AXCL_LOGGER->log(lv, buf);
    }
}

extern "C" void axcl_pcie_debug_log(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    axcl_pcie_fmt_log(spdlog::level::level_enum::debug, fmt, args);
    va_end(args);
}

extern "C" void axcl_pcie_info_log(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    axcl_pcie_fmt_log(spdlog::level::level_enum::info, fmt, args);
    va_end(args);
}

extern "C" void axcl_pcie_err_log(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    axcl_pcie_fmt_log(spdlog::level::level_enum::err, fmt, args);
    va_end(args);
}

extern "C" void axcl_pcie_warn_log(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    axcl_pcie_fmt_log(spdlog::level::level_enum::warn, fmt, args);
    va_end(args);
}