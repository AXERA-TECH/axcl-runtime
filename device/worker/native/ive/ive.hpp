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

#include "package/device/native/module/ive_request.hpp"
#include "package/device/native/module/ive_response.hpp"
#include "ax_ive_api.h"
#include "base.hpp"
#include "native_api.hpp"

#define NATIVE_IVE_API_DECL(_API_) \
            NATIVE_API_DECL(_API_, NATIVE_API_PARAM(ive))

#define NATIVE_IVE_API_IMPL(_API_) \
            NATIVE_API_IMPL(ive, _API_, NATIVE_API_PARAM(ive))

#define NATIVE_IVE_API_APPEND(_API_) \
            NATIVE_API_APPEND(IVE, _API_, NATIVE_API_PARAM(ive))

namespace axcl::worker {

class dispatcher;
class ive : public base {
public:
    ive(dispatcher *dispatcher);
    ~ive() = default;

    virtual void run(const void *data, const size_t& size) override;

private:
    NATIVE_IVE_API_DECL(AX_IVE_Init);
    NATIVE_IVE_API_DECL(AX_IVE_Exit);
    NATIVE_IVE_API_DECL(AX_IVE_Query);
    NATIVE_IVE_API_DECL(AX_IVE_DMA);
    NATIVE_IVE_API_DECL(AX_IVE_Add);
    NATIVE_IVE_API_DECL(AX_IVE_Sub);
    NATIVE_IVE_API_DECL(AX_IVE_And);
    NATIVE_IVE_API_DECL(AX_IVE_Or);
    NATIVE_IVE_API_DECL(AX_IVE_Xor);
    NATIVE_IVE_API_DECL(AX_IVE_Mse);
    NATIVE_IVE_API_DECL(AX_IVE_CannyHysEdge);
    NATIVE_IVE_API_DECL(AX_IVE_CannyEdge);
    NATIVE_IVE_API_DECL(AX_IVE_CCL);
    NATIVE_IVE_API_DECL(AX_IVE_Erode);
    NATIVE_IVE_API_DECL(AX_IVE_Dilate);
    NATIVE_IVE_API_DECL(AX_IVE_Filter);
    NATIVE_IVE_API_DECL(AX_IVE_Hist);
    NATIVE_IVE_API_DECL(AX_IVE_EqualizeHist);
    NATIVE_IVE_API_DECL(AX_IVE_Integ);
    NATIVE_IVE_API_DECL(AX_IVE_MagAndAng);
    NATIVE_IVE_API_DECL(AX_IVE_Sobel);
    NATIVE_IVE_API_DECL(AX_IVE_GMM);
    NATIVE_IVE_API_DECL(AX_IVE_GMM2);
    NATIVE_IVE_API_DECL(AX_IVE_Thresh);
    NATIVE_IVE_API_DECL(AX_IVE_16BitTo8Bit);
    NATIVE_IVE_API_DECL(AX_IVE_CropImage);
    NATIVE_IVE_API_DECL(AX_IVE_CropResize);
    NATIVE_IVE_API_DECL(AX_IVE_CropResizeForSplitYUV);
    NATIVE_IVE_API_DECL(AX_IVE_CSC);
    NATIVE_IVE_API_DECL(AX_IVE_CropResize2);
    NATIVE_IVE_API_DECL(AX_IVE_CropResize2ForSplitYUV);
    NATIVE_IVE_API_DECL(AX_IVE_MAU_MatMul);
    NATIVE_IVE_API_DECL(AX_IVE_NPU_CreateMatMulHandle);
    NATIVE_IVE_API_DECL(AX_IVE_NPU_DestroyMatMulHandle);
    NATIVE_IVE_API_DECL(AX_IVE_NPU_MatMul);

private:
    native_api<axcl::pkg::native::IVE_API, axcl::pkg::device::ive_response, axcl::pkg::device::ive_request> m_api;
};

}  // namespace axcl::worker
