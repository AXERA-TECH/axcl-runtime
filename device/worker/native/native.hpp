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

#include "base.hpp"
#include "dmadim.hpp"
#include "npu.hpp"
#include "ive.hpp"
#include "ivps.hpp"
#include "sys.hpp"
#include "vdec.hpp"
#include "venc.hpp"
#include "dsp.hpp"

namespace axcl::worker {

class dispatcher;
class native : public base {
public:
    native(dispatcher *dispatcher);
    ~native() = default;

    virtual void run(const void *data, const size_t& size) override;

private:
    std::unique_ptr<sys> m_sys;
    std::unique_ptr<vdec> m_vdec;
    std::unique_ptr<venc> m_venc;
    std::unique_ptr<ivps> m_ivps;
    std::unique_ptr<ive> m_ive;
    std::unique_ptr<npu> m_npu;
    std::unique_ptr<dmadim> m_dmadim;
    std::unique_ptr<dsp> m_dsp;
};

}  // namespace axcl::worker
