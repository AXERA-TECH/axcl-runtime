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

#include <vector>
#include "package/device/runtime/runtime_response.hpp"
#include "package/include/runtime/p2p/type.hpp"

namespace axcl::pkg::device {

class p2p_response : public runtime_response {
public:
    explicit p2p_response();
    virtual ~p2p_response();

    p2p_response(p2p_response&& other) noexcept;
    p2p_response& operator=(p2p_response&& other) noexcept;
    p2p_response(const p2p_response&) = delete;
    p2p_response& operator=(const p2p_response&) = delete;

    bool decode(const void* message, uint32_t size) const override;

    axcl::pkg::runtime::p2p::API get_api() const;

    std::vector<struct axcl::pkg::runtime::p2p::p2p_config> get_p2p_config() const;
    std::vector<struct axcl::pkg::runtime::p2p::bwt_config> get_bandwidth_test_config() const;
    uint32_t get_target_id() const;
    int32_t get_bandwidth_test_timeout() const;
    void remove_p2p_config();

private:
    struct impl;
    impl* m_impl;
};

}  // namespace axcl::pkg::device