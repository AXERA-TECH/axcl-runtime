/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "axcl_base.h"
#include "usrwork.hpp"
#include "axcl_rt_usrwork.h"

#ifdef __cplusplus
extern "C" {
#endif

static axcl::rt::usrwork& get_usrwork_instance() {
    static axcl::rt::usrwork instance;
    return instance;
}

AXCL_EXPORT axclError axclrtTransferFile(const char *src_path, const char *dst_path, axclrtFileTransferPolicy policy) {
    return get_usrwork_instance().transfer_file(src_path, dst_path, policy);
}

AXCL_EXPORT axclError axclrtExecWorker(const char *path, const int32_t *argc, const char *argv[], uint32_t *pid) {
    return get_usrwork_instance().exec(path, argc, argv, pid);
}

AXCL_EXPORT axclError axclrtKillWorker(uint32_t pid) {
    return get_usrwork_instance().kill(pid);
}

AXCL_EXPORT axclError axclrtWorkerSend(uint32_t pid, const void *buf, uint32_t size, int32_t timeout) {
    return get_usrwork_instance().send(pid, buf, size, timeout);
}

AXCL_EXPORT axclError axclrtWorkerRecv(uint32_t pid, void *buf, uint32_t bufsize, uint32_t* recvlen, int32_t timeout) {
    return get_usrwork_instance().recv(pid, buf, bufsize, recvlen, timeout);
}

#ifdef __cplusplus
}
#endif