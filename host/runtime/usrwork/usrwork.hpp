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

#include <cstdint>
#include <cstddef>
#include <mutex>
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>

#include "axcl_rt_type.h"
#include "axcl_rt_type_internal.h"
#include "axcl_rt_usrwork_type.h"
#include "package/host/runtime/usrwork/usrwork_request.hpp"
#include "package/host/runtime/usrwork/usrwork_response.hpp"
#include "usrwork_channel.hpp"

namespace axcl::rt {

struct ProcessInfo {
    std::string path;
    std::shared_ptr<usrworkchannel> channel;

    ProcessInfo() = default;
    ProcessInfo(const std::string& p, std::shared_ptr<usrworkchannel> c)
        : path(p), channel(c) {}
};

class usrwork {
public:
    usrwork() = default;
    ~usrwork();

    int32_t transfer_file(const char *src_path, const char *dst_path, axclrtFileTransferPolicy policy);
    int32_t exec(const char *path, const int32_t *argc, const char *argv[], uint32_t *pid);
    int32_t kill(uint32_t pid);

    int32_t send(uint32_t pid, const void *buf, uint32_t size, int32_t timeout);
    int32_t recv(uint32_t pid, void *buf, uint32_t bufsize, uint32_t* recvlen, int32_t timeout);

    int32_t add_process(uint32_t pid, const std::string& path, std::shared_ptr<usrworkchannel> channel);
    int32_t get_process(uint32_t pid, ProcessInfo& info);
    int32_t del_process(uint32_t pid);
    bool has_process(uint32_t pid);
    size_t get_process_count();

protected:
    int32_t get_runtime_environment(axclrtStream &stream, axclrtEnvironment &env);
    int32_t invoke(axclrtStream stream, const axclrtEnvironment &env, int32_t timeout);

private:
    axcl::pkg::host::usrwork_request m_req;
    axcl::pkg::host::usrwork_response m_rsp;

    std::mutex m_mutex;
    std::unordered_map<uint32_t /* pid */, ProcessInfo> m_processes;
};

}  // namespace axcl::rt