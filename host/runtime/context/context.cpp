/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "context.hpp"
#include "context/context_manager.hpp"
#include "axcl_logger.hpp"
#include "stream/stream_manager.hpp"

#define TAG "context"

namespace axcl::rt {

context::context(context_manager *mgr, uint32_t context_id) : m_context_manager(mgr), m_context_id(context_id) {
    m_stream_manager = std::make_shared<stream_manager>(this);
}

void context::sync() {
    m_stream_manager->sync();
}

}  // namespace axcl::rt