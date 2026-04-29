/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "engine_p2p_task.hpp"
#include "engine_p2p_task_impl.hpp"

task_router::task_router() : impl_(new impl) {}

task_router::~task_router() { delete impl_; }

bool task_router::init() const {
    return this->impl_->init();
}

bool task_router::verify_buffer() const {
    return this->impl_->verify_buffer();
}

bool task_router::set_payload(const uint32_t size, const bool block) const {
    return impl_->set_payload(size, block);// && impl_->reset_buffer();
}

bool task_router::launch_parallel_task(std::vector<float>& elapsed) const {
    return impl_->launch_parallel_task(elapsed);
}

bool task_router::launch_sequential_task(std::vector<float>& elapsed) const {
    return impl_->launch_sequential_task(elapsed);
}

bool task_router::launch_interlace_task(std::vector<float>& elapsed) const {
    return impl_->launch_interlace_task(elapsed);
}
