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
#include <list>
#include <memory>
#include <mutex>
#include <atomic>
#include "axcl_comm.hpp"
#include "threadx.hpp"

#define MAX_LISTEN_NUM (2)
#define MAX_SINKER_NUM (MAX_LISTEN_NUM)

namespace axcl::rt {

struct channel_data {
    uint64_t token;
    uint32_t device;
    uint32_t context;
    uint32_t stream;
    void *data;
    size_t size;
};

class sinker {
public:
    virtual ~sinker() = default;
    virtual void on_channel_data(std::shared_ptr<channel_data> data) = 0;
};

class channel {
public:
    channel(std::shared_ptr<axcl::comm::icomm> comm);
    ~channel() = default;

    void register_sink(sinker *sink);
    void unregister_sink(sinker *sink);

    bool send(const void *data, size_t size, int64_t flag /* -1: auto flag */, int32_t timeout);

    bool start();
    void stop();

    void offline();
    void online();

protected:
    void listen(int32_t index, int64_t flag);
    void dispatch(int32_t index, const channel_data &data);

    std::shared_ptr<channel_data> create_channel_data(const channel_data &data);

private:
    axcl::threadx m_thread[MAX_LISTEN_NUM];
    std::shared_ptr<axcl::comm::icomm> m_comm;
    std::mutex m_mtx_sinks[MAX_SINKER_NUM];
    std::list<sinker *> m_sinks[MAX_SINKER_NUM];
    std::atomic<bool> m_offline;
    axcl::comm::COMM_TYPE m_type;
};

}  // namespace axcl::rt