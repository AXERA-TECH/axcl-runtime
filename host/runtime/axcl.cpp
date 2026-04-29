/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "axcl.h"
#include <stdarg.h>
#include <string.h>
#if defined(__linux__) && defined(__GLIBC__)
#include <malloc.h>
#endif
#include "axcl_module_version.h"
#include "axcl_rt_config.h"
#include "device/device_manager.hpp"
#include "axcl_logger.hpp"

AXCL_EXPORT axclError axclInit(const char *config) {
#if defined(__linux__) && defined(__GLIBC__)
    mallopt(M_TRIM_THRESHOLD, 128 * 1024);
#endif

	if (config && ::strlen(config) > 0) {
        axclrtConfigLoad(config);
    }

    auto lv = static_cast<spdlog::level::level_enum>(axclrtConfigGetLogLevel(0));
    axcl::logger::get_instance(axclrtConfigGetLogPath())->set_level(lv);

    return DEVICE_MANAGER()->init();
}

AXCL_EXPORT axclError axclFinalize() {
    return DEVICE_MANAGER()->deinit();
}

AXCL_EXPORT axclError axclSetLogLevel(int32_t lv) {
    axcl::logger::get_instance(axclrtConfigGetLogPath())->set_level(axcl::logger::get_level(lv));
    return AXCL_SUCC;
}

AXCL_EXPORT void axclAppLog(int32_t lv, const char *func, const char *file, uint32_t line, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char buf[1024];
    int len = vsnprintf(buf, sizeof(buf), fmt, args);
    if (len > 0) {
        if (func && file) {
            AXCL_LOGGER->log(axcl::logger::get_level(lv), "[APP][{}][{}][{}]: {}", func, file, line, buf);
        } else if (func) {
            AXCL_LOGGER->log(axcl::logger::get_level(lv), "[APP][{}][{}]: {}", func, line, buf);
        } else if (file) {
            AXCL_LOGGER->log(axcl::logger::get_level(lv), "[APP][{}][{}]: {}", file, line, buf);
        } else {
            AXCL_LOGGER->log(axcl::logger::get_level(lv), "[APP][{}]: {}", line, buf);
        }
    }
}
