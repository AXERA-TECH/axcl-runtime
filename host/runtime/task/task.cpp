/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "task.hpp"
#include "stream/stream.hpp"

namespace axcl::rt {


task::task(stream* s, uint64_t token, const void* data, size_t size, int32_t timeout, bool has_response /* = true */,
           int64_t flag /* = -1 */)
    : m_stream(s),
      m_token(token),
      m_data(data),
      m_size(size),
      m_timeout(timeout),
      m_has_response(has_response),
      m_flag(flag),
      m_result(false),
      m_deactived(false),
      m_nMask(m_has_response ? (TASK_EVENT_EXCU | TASK_EVENT_RESP) : TASK_EVENT_EXCU) {
}

bool task::execute() {
    m_event.reset_all();

    m_result = m_stream->execute(m_data, m_size, m_flag, m_timeout);
    wakeup(TASK_EVENT_EXCU);

    return m_result;
}

}  // namespace axcl::rt