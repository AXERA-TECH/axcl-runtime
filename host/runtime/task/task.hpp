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

#include <atomic>
#include <channel/channel.hpp>
#include "event.hpp"

namespace axcl::rt {

const int TASK_EVENT_EXCU = 1 << 0;
const int TASK_EVENT_RESP = 1 << 1;

class stream;
class task {
public:
    task(stream* s, uint64_t token, const void* data, size_t size, int32_t timeout, bool has_response = true, int64_t flag = -1);

    virtual bool execute();

    int32_t wait() {

        if (!m_event.wait(m_nMask, m_timeout)) {
            return -1;
        }

        if (!m_result) {
            return -2;
        }

        return 0;
    }

    void wakeup(std::shared_ptr<channel_data> response) {
        m_response = response;
        wakeup(TASK_EVENT_RESP);
    }

    void wakeup(int bit_mask) {
        m_event.set(bit_mask);
    }

    uint64_t get_token() const {
        return m_token;
    }

    bool has_response() const {
        return m_has_response;
    }

    void deactive() {
        m_deactived = true;
    }

    bool is_deactived() const {
        return m_deactived;
    }

    bool has_got_response() const {
        return !m_has_response || (m_response != nullptr);
    }

    stream* get_stream() {
        return m_stream;
    }

    void* get_response(uint32_t& size) const {
        size = m_response->size;
        return m_response->data;
    }

private:

    stream* m_stream;
    uint64_t m_token;
    const void* m_data;
    size_t m_size;
    int32_t m_timeout;
    bool m_has_response;
    int64_t m_flag;
    bool m_result;

    /* task is deactived, means not be executed */
    std::atomic<bool> m_deactived;

    std::shared_ptr<channel_data> m_response;
    axcl::bitmask_event m_event;
    int m_nMask = 0;
};

}  // namespace axcl::rt