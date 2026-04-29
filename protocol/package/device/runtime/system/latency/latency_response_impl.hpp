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

#include "axcl_assert.hpp"
#include "axcl_logger.hpp"
#include "protocol.hpp"

namespace axcl::pkg::device {

struct latency_response::impl {
    [[nodiscard]] bool select(axcl::runtime::system::request::Payload *payload) {
        if (axcl::runtime::system::Module::Latency != payload->head().module()) {
            LOG_MM_E("package", "current module {} is not latency module", static_cast<int32_t>(payload->head().module()));
            AXCL_ASSERT(0, "current module %d is not latency module", static_cast<int32_t>(payload->head().module()));
            return false;
        }

        m_payload = payload->mutable_body()->mutable_latency();
        AXCL_ASSERT(m_payload, "nil pointer");

        return true;
    }

    const void *get_latency_data(size_t &size) const {
        const auto &data = m_payload->data();
        size = data.size();
        return data.data();
    }

    const std::string &get_latency_data() const {
        return m_payload->data();
    }

private:
    axcl::runtime::system::request::Latency *m_payload = {nullptr};
};

}  // namespace axcl::pkg::device