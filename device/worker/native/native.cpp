/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "native.hpp"

#include "dispatcher.hpp"
#include "protocol.hpp"

#define TAG "native"

namespace axcl::worker {

native::native(dispatcher *dispatcher) : base(dispatcher) {
    m_sys = std::make_unique<sys>(dispatcher);
    m_vdec = std::make_unique<vdec>(dispatcher);
    m_venc = std::make_unique<venc>(dispatcher);
    m_ivps = std::make_unique<ivps>(dispatcher);
    m_ive = std::make_unique<ive>(dispatcher);
    m_npu = std::make_unique<npu>(dispatcher);
    m_dmadim = std::make_unique<dmadim>(dispatcher);
    m_dsp = std::make_unique<dsp>(dispatcher);
}

void native::run(const void *data, const size_t& size) {
    const auto hd = static_cast<const axcl::pkg::head *>(data);

    switch (static_cast<axcl::pkg::native::module>(GET_PACKAGE_SUB_MODULE(hd->type))) {
    case axcl::pkg::native::module::SYS:
        m_sys->run(data, size);
        break;

    case axcl::pkg::native::module::VDEC:
        m_vdec->run(data, size);
        break;

    case axcl::pkg::native::module::VENC:
        m_venc->run(data, size);
        break;

    case axcl::pkg::native::module::IVPS:
        m_ivps->run(data, size);
        break;

    case axcl::pkg::native::module::IVE:
        m_ive->run(data, size);
        break;

    case axcl::pkg::native::module::ENGINE:
        m_npu->run(data, size);
        break;

    case axcl::pkg::native::module::DMADIM:
        m_dmadim->run(data, size);
        break;

    case axcl::pkg::native::module::DSP:
        m_dsp->run(data, size);
        break;

    default:
        break;
    }
}

}  // namespace axcl::worker
