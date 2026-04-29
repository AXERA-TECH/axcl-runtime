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

#include "package/host/runtime/runtime_response.hpp"
#include "package/include/runtime/p2p/type.hpp"
#include <vector>

namespace axcl::pkg::host {

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
    int32_t get_status() const;

    struct axcl::pkg::runtime::p2p::bwt_result get_bandwidth_test_result() const;

private:
    struct impl;
    impl* m_impl;
};

}  // namespace axcl::pkg::host