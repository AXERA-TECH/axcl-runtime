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
#include <algorithm>
#include "axcl_package.hpp"
#include "axcl_logger.hpp"
#include "package/include/type.hpp"
#include "os.hpp"

#define TAG "channel"

namespace axcl::rt {

channel::channel(std::shared_ptr<axcl::comm::icomm> comm) : m_comm(comm), m_offline(false) {
    axcl::comm::attr v;
    m_comm->get_attr(axcl::comm::icomm::COMM_TYPE_ATTR, v);
    m_type = std::get<axcl::comm::COMM_TYPE>(v);
}

bool channel::start() {
    LOG_MM_I(TAG, "+++");

    if (!m_comm) {
        LOG_MM_E(TAG, "comm instance is nil");
        return false;
    }

    m_offline = false;

    if (axcl::comm::COMM_TYPE::COMM_TYPE_PCIE == m_type) {
        m_thread[0].start("listen-1", SCHED_FIFO, 99, &channel::listen, this, 0, static_cast<int64_t>(axcl::comm::PCIE_MSG));
        m_thread[1].start("listen-2", SCHED_FIFO, 99, &channel::listen, this, 1, static_cast<int64_t>(axcl::comm::PCIE_DMA));
    } else {
        m_thread[0].start("listen", SCHED_FIFO, 99, &channel::listen, this, 0, 0);
    }

    LOG_MM_I(TAG, "---");
    return true;
}

void channel::stop() {
    LOG_MM_I(TAG, "stop listen-1");

    m_thread[0].stop();
    m_thread[0].join();

    if (axcl::comm::COMM_TYPE::COMM_TYPE_PCIE == m_type) {
        LOG_MM_I(TAG, "stop listen-2");
        m_thread[1].stop();
        m_thread[1].join();
    }

    LOG_MM_I(TAG, "---");
}

void channel::listen(int32_t index, int64_t flag) {
    LOG_MM_I(TAG, "{} flag: {} +++", index, flag);

    constexpr int32_t timeout = -1;
    void *msg = nullptr;
    size_t len;
    while (m_thread[index].running()) {
        if (m_offline.load()) {
            break;
        }

        len = m_comm->recv(&msg, flag, timeout);
        if (len <= PACKAGE_HEAD_SIZE) {
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


        dispatch(index, {hd->seq_num, hd->device, hd->context, hd->stream, msg, len});
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

void channel::register_sink(sinker *sink) {
    for (int32_t i = 0; i < MAX_SINKER_NUM; ++i) {
        std::lock_guard<std::mutex> lck(m_mtx_sinks[i]);
        if (std::find(m_sinks[i].begin(), m_sinks[i].end(), sink) != m_sinks[i].end()) {
            continue;
        }

        m_sinks[i].push_back(sink);
    }

    LOG_MM_D(TAG, "sink {:#x} is registed", reinterpret_cast<uintptr_t>(sink));
}

void channel::unregister_sink(sinker *sink) {
    for (int32_t i = 0; i < MAX_SINKER_NUM; ++i) {
        std::lock_guard<std::mutex> lck(m_mtx_sinks[i]);
        auto it = std::find(m_sinks[i].begin(), m_sinks[i].end(), sink);
        if (it != m_sinks[i].end()) {
            m_sinks[i].erase(it);
            continue;
        }
    }

    LOG_MM_D(TAG, "sink {:#x} is unregisted", reinterpret_cast<uintptr_t>(sink));
}

bool channel::send(const void *data, size_t size, int64_t flag, int32_t timeout) {
    if (m_offline) {
        return false;
    }

    size_t sz;
    if (axcl::comm::COMM_TYPE::COMM_TYPE_PCIE == m_type) {
        if (flag < 0) {
            flag = (size <= MAX_PCIE_MSG_SIZE) ? axcl::comm::PCIE_MSG : axcl::comm::PCIE_DMA;
        }

        sz = m_comm->send(data, size, flag, timeout);
    } else {
        sz = m_comm->send(data, size, flag, timeout);
    }

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

void channel::online() {
    LOG_MM_I(TAG, "set online");
    m_offline = false;
}


}  // namespace axcl::rt