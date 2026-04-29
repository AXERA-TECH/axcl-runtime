/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "axcl_base.h"
#include "pcie.hpp"
#if defined(WINDOWS)
#else
#include "socket.hpp"
#endif
#include "axcl_module_version.h"

AXCL_EXPORT std::shared_ptr<axcl::comm::icomm> axcl_create_comm_instance(axcl::comm::COMM_TYPE type) {
    switch (type) {
        case axcl::comm::COMM_TYPE::COMM_TYPE_PCIE:
            return std::make_shared<pcie>();
        case axcl::comm::COMM_TYPE::COMM_TYPE_SOCKET:
#if defined(WINDOWS)
            return nullptr;
#else
            return std::make_shared<socket>();
#endif
        default:
            return nullptr;
    }
}
