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

#include <string>
#include <unordered_map>
#include "axcl_comm.hpp"

class base : public axcl::comm::icomm {
public:
    base(axcl::comm::COMM_TYPE type);

    bool get_attr(const char *name, axcl::comm::attr &value) override;
    bool set_attr(const char *name, const axcl::comm::attr &value) override;

    static uint64_t get_tick();

protected:
    std::unordered_map<std::string, axcl::comm::attr> m_attrs;  //!< attributes key - value
};
