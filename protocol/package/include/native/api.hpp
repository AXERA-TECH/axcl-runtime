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

#include "package/include/native/module/dmadim.hpp"
#include "package/include/native/module/engine.hpp"
#include "package/include/native/module/ive.hpp"
#include "package/include/native/module/ivps.hpp"
#include "package/include/native/module/sys.hpp"
#include "package/include/native/module/vdec.hpp"
#include "package/include/native/module/venc.hpp"
#include "package/include/native/module/dsp.hpp"

#define IS_NATIVE_API_ENUM_TYPE(T) \
    (std::is_same<T, axcl::pkg::native::SYS_API>::value    || std::is_same<T, axcl::pkg::native::VDEC_API>::value   || \
     std::is_same<T, axcl::pkg::native::VENC_API>::value   || std::is_same<T, axcl::pkg::native::IVPS_API>::value   || \
     std::is_same<T, axcl::pkg::native::IVE_API>::value    || std::is_same<T, axcl::pkg::native::ENGINE_API>::value || \
     std::is_same<T, axcl::pkg::native::DMADIM_API>::value || std::is_same<T, axcl::pkg::native::DSP_API>::value)

namespace axcl::pkg::native {

union API {
    SYS_API sys;
    VDEC_API vdec;
    VENC_API venc;
    IVPS_API ivps;
    IVE_API ive;
    ENGINE_API engine;
    DMADIM_API dmadim;
    DSP_API dsp;
};

}  // namespace axcl::pkg::native
