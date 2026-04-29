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

#include "package/host/runtime/runtime_request.hpp"
#include "package/include/runtime/p2p/type.hpp"

namespace axcl::pkg::host {

class p2p_request : public runtime_request {
public:
    explicit p2p_request();
    virtual ~p2p_request();

    p2p_request(p2p_request&& other) noexcept;
    p2p_request& operator=(p2p_request&& other) noexcept;
    p2p_request(const p2p_request&) = delete;
    p2p_request& operator=(const p2p_request&) = delete;

    void setup_p2p_config(const struct axcl::pkg::runtime::p2p::p2p_config* config, uint32_t num) const;
    void start_bandwidth_test(uint32_t target_id, const struct axcl::pkg::runtime::p2p::bwt_config* config, uint32_t num, int32_t timeout) const;
    void remove_p2p_config(void) const;


private:
    struct impl;
    impl* m_impl;
};

}  // namespace axcl::pkg::host