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

#include <mutex>
#include <unordered_map>
#include <string>
#include <vector>
#include "package/device/runtime/usrwork/usrwork_request.hpp"
#include "package/device/runtime/usrwork/usrwork_response.hpp"
#include "base.hpp"
#include "runtime_api.hpp"

#include "usrworker_def.h"
#include "usrworker_ipc.h"
#include "usrworker_manager.h"


#define RUNTIME_USRWORK_API_DECL(_API_) \
            RUNTIME_API_DECL(_API_, RUNTIME_API_PARAM(usrwork))

#define RUNTIME_USRWORK_API_IMPL(_API_) \
            RUNTIME_API_IMPL(usrwork, _API_, RUNTIME_API_PARAM(usrwork))

#define RUNTIME_USRWORK_API_APPEND(_API_) \
            RUNTIME_API_APPEND(usrwork, _API_, RUNTIME_API_PARAM(usrwork))

namespace axcl::worker {

class dispatcher;
class usrwork : public base {
public:
    usrwork(dispatcher *dispatcher);
    ~usrwork();

    virtual void run(const void *data, const size_t& size) override;

private:
    RUNTIME_USRWORK_API_DECL(TRANSFER_FILE);
    RUNTIME_USRWORK_API_DECL(EXEC_WORKER);
    RUNTIME_USRWORK_API_DECL(KILL_WORKER);

private:
    runtime_api<axcl::pkg::runtime::usrwork::API, axcl::pkg::device::usrwork_response, axcl::pkg::device::usrwork_request> m_api;
    std::unique_ptr<usrworker_manager> m_worker_manager;
};

}  // namespace axcl::worker