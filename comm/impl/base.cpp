/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "base.hpp"
#include "os.hpp"

using namespace axcl::comm;

base::base(COMM_TYPE type) {
    m_attrs[icomm::COMM_TYPE_ATTR] = type;
}

bool base::get_attr(const char *name, attr &value) {
    auto it = m_attrs.find(name);
    if (it != m_attrs.end()) {
        value = it->second;
        return true;
    }

    return false;
}

bool base::set_attr(const char *name, const attr &value) {
    m_attrs[name] = value;
    return true;
}

uint64_t base::get_tick() {
    return xp_time_get_tick_ms();
}
