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

namespace axcl::pkg::host {

struct log_response::impl {
    [[nodiscard]] bool select(axcl::runtime::system::response::Payload *payload) {
        if (axcl::runtime::system::Module::Log != payload->head().module()) {
            LOG_MM_E("package", "current module {} is not log module", static_cast<int32_t>(payload->head().module()));
            AXCL_ASSERT(0, "current module %d is not log module", static_cast<int32_t>(payload->head().module()));
            return false;
        }

        m_payload = payload->mutable_body()->mutable_log();
        AXCL_ASSERT(m_payload, "nil pointer");

        return true;
    }

    const std::string &get_name() const {
        return m_payload->name();
    }

    const std::string &get_content() const {
        return m_payload->content();
    }

    const char *get_content(size_t &size) const {
        const auto &data = m_payload->content();
        size = data.size();
        return data.data();
    }

private:
    axcl::runtime::system::response::Log *m_payload = {nullptr};
};

}  // namespace axcl::pkg::host