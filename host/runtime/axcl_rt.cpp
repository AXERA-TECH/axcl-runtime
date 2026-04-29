/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "axcl_rt.h"
#include <string.h>
#include <regex>
#include <string>
#include "axcl_rt_control_internal.h"
#include "version.hpp"

#if defined(AXCL_BUILD_VERSION)
static const std::string VERSION_STRING = AXCL_BUILD_VERSION;
#else
static const std::string VERSION_STRING = "0.0.0";
#endif

AXCL_EXPORT axclError axclrtGetVersion(int32_t *major, int32_t *minor, int32_t *patch) {
    if (!major || !minor || !patch) {
        return AXCL_ERR_NULL_POINTER;
    }

    std::string version = VERSION_STRING;
    std::regex regex(R"(^\s*[Vv]?(\d+)(?:\.(\d+))?(?:\.(\d+))?(?:[\s_]+.*)?\s*$)");
    std::smatch match;
    if (std::regex_match(version, match, regex)) {
        try {
            *major = match[1].matched ? std::stoi(match[1].str()) : 0;
            *minor = match[2].matched ? std::stoi(match[2].str()) : 0;
            *patch = match[3].matched ? std::stoi(match[3].str()) : 0;
        } catch (const std::exception &) {
            *major = 0;
            *minor = 0;
            *patch = 0;
        }
    } else {
        *major = 0;
        *minor = 0;
        *patch = 0;
    }

    return AXCL_SUCC;
}

AXCL_EXPORT const char *axclrtGetFullVersion() {
    return VERSION_STRING.c_str();
}

AXCL_EXPORT const char *axclrtGetSocName() {
    constexpr const char *UNKNOWN = "UNKNOWN";
    const char *output = NULL;
    axclError ret = axclrtControlExecuteShellCmd("cat /proc/ax_proc/chip_type", NULL, 0, &output, 1000);
    if (ret != AXCL_SUCC || !output) {
        return UNKNOWN;
    }

    if (strstr(output, "AX650N")) {
        return "AX650N";
    } else if (strstr(output, "AX650C")) {
        return "AX8850";
    } else if (strstr(output, "AX650A")) {
        return "AX650A";
    } else {
        return UNKNOWN;
    }
}
