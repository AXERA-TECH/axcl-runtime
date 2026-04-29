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

#include "package/device/native/module/dmadim_request.hpp"
#include "package/device/native/module/dmadim_response.hpp"
#include "ax_dmadim_api.h"
#include "base.hpp"
#include "native_api.hpp"

#define NATIVE_DMADIM_API_DECL(_API_) \
            NATIVE_API_DECL(_API_, NATIVE_API_PARAM(dmadim))

#define NATIVE_DMADIM_API_IMPL(_API_) \
            NATIVE_API_IMPL(dmadim, _API_, NATIVE_API_PARAM(dmadim))

#define NATIVE_DMADIM_API_APPEND(_API_) \
            NATIVE_API_APPEND(DMADIM, _API_, NATIVE_API_PARAM(dmadim))

namespace axcl::worker {

class dispatcher;
class dmadim : public base {
public:
    dmadim(dispatcher *dispatcher);
    ~dmadim() = default;

    virtual void run(const void *data, const size_t& size) override;

private:
    NATIVE_DMADIM_API_DECL(AX_DMADIM_Open);
    NATIVE_DMADIM_API_DECL(AX_DMADIM_Cfg);
    NATIVE_DMADIM_API_DECL(AX_DMADIM_Start);
    NATIVE_DMADIM_API_DECL(AX_DMADIM_Waitdone);
    NATIVE_DMADIM_API_DECL(AX_DMADIM_Close);
    NATIVE_DMADIM_API_DECL(AX_DMA_MemCopy);
    NATIVE_DMADIM_API_DECL(AX_DMA_MemSet);
    NATIVE_DMADIM_API_DECL(AX_DMA_MemCopyXD);
    NATIVE_DMADIM_API_DECL(AX_DMA_CheckSum);

private:
    native_api<axcl::pkg::native::DMADIM_API, axcl::pkg::device::dmadim_response, axcl::pkg::device::dmadim_request> m_api;
};

}  // namespace axcl::worker
