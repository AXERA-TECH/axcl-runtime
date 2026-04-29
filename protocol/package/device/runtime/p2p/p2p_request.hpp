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

#include "package/device/runtime/runtime_request.hpp"
#include "package/include/runtime/p2p/type.hpp"

namespace axcl::pkg::device {

class p2p_request : public runtime_request {
public:
    explicit p2p_request();
    virtual ~p2p_request();

    p2p_request(p2p_request&& other) noexcept;
    p2p_request& operator=(p2p_request&& other) noexcept;
    p2p_request(const p2p_request&) = delete;
    p2p_request& operator=(const p2p_request&) = delete;

    void set_api(axcl::pkg::runtime::p2p::API api) const;
    void set_status(int32_t status) const;
    void set_bandwidth_test_result(const struct axcl::pkg::runtime::p2p::bwt_result &res) const;

private:
    struct impl;
    impl* m_impl;
};

}  // namespace axcl::pkg::device