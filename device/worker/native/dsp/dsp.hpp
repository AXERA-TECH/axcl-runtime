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

#include "package/device/native/module/dsp_request.hpp"
#include "package/device/native/module/dsp_response.hpp"

#include "base.hpp"
#include "native_api.hpp"

#define NATIVE_DSP_API_DECL(_API_) \
            NATIVE_API_DECL(_API_, NATIVE_API_PARAM(dsp))

#define NATIVE_DSP_API_IMPL(_API_) \
            NATIVE_API_IMPL(dsp, _API_, NATIVE_API_PARAM(dsp))

#define NATIVE_DSP_API_APPEND(_API_) \
            NATIVE_API_APPEND(DSP, _API_, NATIVE_API_PARAM(dsp))

namespace axcl::worker {

class dispatcher;
class dsp final: public base {
public:
    explicit dsp(dispatcher *dispatcher);
    ~dsp() override = default;

    void run(const void *data, const size_t& size) override;

private:
    NATIVE_DSP_API_DECL(AX_DSP_PowerOn);
    NATIVE_DSP_API_DECL(AX_DSP_PowerOff);
    NATIVE_DSP_API_DECL(AX_DSP_LoadBin);
    NATIVE_DSP_API_DECL(AX_DSP_EnableCore);
    NATIVE_DSP_API_DECL(AX_DSP_DisableCore);
    NATIVE_DSP_API_DECL(AX_DSP_PRC);
    NATIVE_DSP_API_DECL(AX_DSP_Query);

    native_api<pkg::native::DSP_API, pkg::device::dsp_response, pkg::device::dsp_request> m_api;
};

}  // namespace axcl::worker
