/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "smi_util.hpp"

#if defined(WINDOWS)
#include <windows.h>
#else
#include <unistd.h>
#endif

std::string get_exe_full_path(uint32_t pid) {
#if defined(WINDOWS)
    HANDLE handle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
    if (!handle) {
        return "";
    }

    char path[MAX_PATH] = {0};
    DWORD size = MAX_PATH;
    QueryFullProcessImageNameA(handle, 0, path, &size);
    CloseHandle(handle);

    return std::string(path);
#else
    char path[260] = {0};
    std::string exe = "/proc/" + std::to_string(pid) + "/exe";
    ssize_t len = readlink(exe.c_str(), path, sizeof(path) - 1);
    if (len != -1) {
        path[len] = '\0';
        return std::string(path);
    } else {
        return "";
    }
#endif
}