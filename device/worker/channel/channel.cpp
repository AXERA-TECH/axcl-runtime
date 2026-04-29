/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "channel.hpp"
#include <pthread.h>
#include <algorithm>
#include "axcl_package.hpp"
#include "axcl_logger.hpp"
#include "package/include/type.hpp"

#define TAG "channel"

#include <string>

namespace axcl::worker {

channel::channel(std::shared_ptr<axcl::comm::icomm> comm, std::array<uint32_t, 4> ports) : m_ports(ports), m_comm(comm), m_offline(false) {
}

bool channel::start() {
    LOG_MM_I(TAG, "+++");

    if (!m_comm) {
        LOG_MM_E(TAG, "comm instance is nil");
        return false;
    }

    m_thread[0].start("listen-1", SCHED_FIFO, 99, &channel::listen, this, 0, static_cast<int64_t>(axcl::comm::PCIE_MSG));
    m_thread[1].start("listen-2", SCHED_FIFO, 99, &channel::listen, this, 1, static_cast<int64_t>(axcl::comm::PCIE_DMA));

    LOG_MM_I(TAG, "---");
    return true;
}

void channel::stop() {
    LOG_MM_I(TAG, "+++");

    LOG_MM_I(TAG, "stop listen-1");
    m_thread[0].stop();
    m_thread[0].join();

    LOG_MM_I(TAG, "stop listen-2");
    m_thread[1].stop();
    m_thread[1].join();

    LOG_MM_I(TAG, "---");
}

#if 0

static void print_head(const axcl::pkg::head& head) {
    uint8_t package_type = GET_PACKAGE_TYPE(head.type);
    std::string type_str = (package_type == axcl::pkg::PROTO) ? "PROTO (0)" : "RAW (1)";

    uint8_t module = GET_PACKAGE_MODULE(head.type);
    std::string module_str;
    if (module == 0) {
        module_str = "runtime (0)";
    } else if (module == 1) {
        module_str = "native (1)";
    } else {
        module_str = std::to_string(module);
    }

    uint8_t sub_module = GET_PACKAGE_SUB_MODULE(head.type);
    uint8_t api = GET_PACKAGE_SUB_MODULE_API(head.type);

    uint8_t handler = (head.type >> 24) & 0x3F;
    std::string handler_str;
    if (handler == 1) {
        handler_str = "memcpy (1)";
    } else {
        handler_str = std::to_string(handler);
    }

    LOG_MM_E(TAG, "head=seq:{:3}|dev:{:3}|ctx:{:3}|stm:{:3}|type:0x{:08x}|type:{:3}|handler:{:3}|api:{:3}|sub:{:3}|mod:{:3}|cmd:{:2}",
             head.seq_num, head.device, head.context, head.stream,
             head.type, type_str, handler_str, static_cast<int>(api),
             static_cast<int>(sub_module), module_str, head.command);
}
#endif

void channel::listen(int32_t index, int64_t flag) {
    LOG_MM_I(TAG, "{} flag: {} +++", index, flag);

    int32_t timeout = -1;
    void *msg = nullptr;
    size_t len;
    while (m_thread[index].running()) {
        if (m_offline.load()) {
            break;
        }

        len = m_comm->recv(&msg, flag, timeout);

        // timeout
        if (0 == len) {
            channel_data timeout_rsp;

            memset(&timeout_rsp, 0x00, sizeof(channel_data));
            timeout_rsp.port[0] = m_ports[index << 1];
            timeout_rsp.port[1] = m_ports[(index << 1) + 1];
            std::shared_ptr<channel_data> p = std::make_shared<channel_data>(timeout_rsp);

            dispatch(index, p);
            continue;
        } else if (len <= PACKAGE_HEAD_SIZE) {
            if (m_offline.load()) {
                break;
            }

            LOG_MM_E(TAG, "invalid recv size {}", len);

            if (len > 0 && msg) {
                m_comm->free(msg);
            }

            continue;
        }

        const auto hd = static_cast<const axcl::pkg::head *>(msg);
        // LOG_MM_D(TAG, "recv token {}, msg len {}", hd->seq_num, len);
        if (RUNTIME_MEMCPY_HANDLER_TYPE == hd->type || RUNTIME_MEMCPY_ASYNC_HANDLER_TYPE == hd->type) {
            /* nothing to do for memcpy between host and device */
            m_comm->free(msg);
            continue;
        }

        // print_head(*hd);
        dispatch(index, {{m_ports[index << 1], m_ports[(index << 1) + 1]}, hd->context, hd->stream, msg, len});
    }

    LOG_MM_I(TAG, "{} flag: {} ---", index, flag);
}

std::shared_ptr<channel_data> channel::create_channel_data(const channel_data &data) {
    return std::shared_ptr<channel_data>(new channel_data{data}, [this](channel_data *p) {
        if (p) {
            m_comm->free(p->data);
            delete p;
        }
    });
}

void channel::dispatch(int32_t index, const channel_data &data) {
    std::shared_ptr<channel_data> p = create_channel_data(data);

    std::lock_guard<std::mutex> lck(m_mtx_sinks[index]);
    for (auto &&sink : m_sinks[index]) {
        sink->on_channel_data(p);
    }
}

void channel::dispatch(int32_t index, std::shared_ptr<channel_data> data) {
    std::lock_guard<std::mutex> lck(m_mtx_sinks[index]);
    for (auto &&sink : m_sinks[index]) {
        sink->on_channel_data(data);
    }
}

bool channel::register_sink(sinker *sink) {
    for (int32_t i = 0; i < MAX_SINKER_NUM; ++i) {
        std::lock_guard<std::mutex> lck(m_mtx_sinks[i]);
        if (std::find(m_sinks[i].begin(), m_sinks[i].end(), sink) != m_sinks[i].end()) {
            LOG_MM_W(TAG, "sink {:#x} is already registered", reinterpret_cast<uintptr_t>(sink));
            continue;
        }

        m_sinks[i].push_back(sink);
    }

    LOG_MM_I(TAG, "sink {:#x} is registed", reinterpret_cast<uintptr_t>(sink));
    return true;
}

bool channel::unregister_sink(sinker *sink) {
    for (int32_t i = 0; i < MAX_SINKER_NUM; ++i) {
        std::lock_guard<std::mutex> lck(m_mtx_sinks[i]);
        auto it = std::find(m_sinks[i].begin(), m_sinks[i].end(), sink);
        if (it != m_sinks[i].end()) {
            m_sinks[i].erase(it);
            continue;
        }
    }

    LOG_MM_I(TAG, "sink {:#x} is unregisted", reinterpret_cast<uintptr_t>(sink));
    return true;
}

bool channel::send(const void *data, size_t size, int64_t flag, int32_t timeout) {
    if (m_offline) {
        return false;
    }

    // const auto hd = static_cast<const axcl::pkg::head *>(data);
    // LOG_MM_D(TAG, "send token {}, msg len {}", hd->seq_num, size);
    if (flag < 0) {
        flag = (size <= MAX_PCIE_MSG_SIZE) ? axcl::comm::PCIE_MSG : axcl::comm::PCIE_DMA;
    }

    size_t sz = m_comm->send(data, size, flag, timeout);
    if (sz != size) {
        LOG_MM_E(TAG, "send {} bytes, but {} bytes sent", size, sz);
        return false;
    }

    return true;
}

void channel::offline() {
    LOG_MM_I(TAG, "set offline");
    m_offline = true;
}

}  // namespace axcl::worker
