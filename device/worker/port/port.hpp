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

#include <array>
#include <cstdint>
#include <list>
#include <memory>
#include <mutex>
#include <atomic>
#include "axcl_comm.hpp"
#include "channel.hpp"
#include "dispatcher.hpp"

namespace axcl::worker {

class port   {
public:
    port(axcl::comm::COMM_TYPE type, std::array<uint32_t, 4> ports, uint32_t dma_buf_size);
    ~port() = default;

    bool open();
    void close();
    bool send(const void *data, size_t size, int64_t flag, int32_t timeout);

    bool start();

    bool register_sink(sinker *sink);
    bool unregister_sink(sinker *sink);

    std::shared_ptr<axcl::comm::icomm> get_comm() const { return m_comm; }

private:
    axcl::comm::COMM_TYPE m_comm_type;
    std::array<uint32_t, 4> m_ports;
    std::shared_ptr<axcl::comm::icomm> m_comm;
    std::unique_ptr<channel> m_channel;
    std::unique_ptr<dispatcher> m_dispatcher;
};

}  // namespace axcl::worker
