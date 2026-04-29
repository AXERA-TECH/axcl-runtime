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
#include "port.hpp"
#include "device_info.h"

namespace axcl::worker {

class device {
public:
    device(axcl::comm::COMM_TYPE type, const std::array<uint32_t, 4> ports, uint32_t dma_buf_size);
    ~device() = default;

    bool open();
    void close();
    bool start();

private:
    axcl::comm::COMM_TYPE m_comm_type;
    std::array<uint32_t, 4> m_ports;
    std::shared_ptr<port> m_msg_port;
};

}  // namespace axcl::worker
