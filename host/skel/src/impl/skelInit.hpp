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
#include "def.hpp"
#include "ax_skel_type.h"
#include <condition_variable>

namespace axcl::skel {
#define AXCL_SKEL_SDK_VERSION "AXCL_SKEL_V1.0.0"

#define SKEL_HVCP_MODEL_KEY_STR "hvcp_algo_model"
#define SKEL_FACE_MODEL_KEY_STR "face_algo_model"
#define SKEL_FACEATTR_MODEL_KEY_STR "facewhat_algo_model"
#define SKEL_HVCP_MODEL_PPL_KEY_STR "hvcp_algo_ppl"
#define SKEL_FACE_MODEL_PPL_KEY_STR "face_algo_ppl"

/// @brief init parameter struct
typedef struct skel_INIT_PARAM_T {
    std::string strModelDeploymentPath;
    std::string strHvcpModel;
    std::string strHvcpModelName;
    std::string strFaceModel;
    std::string strFaceModelName;
    std::string strFaceAttrModel;
    std::string strFaceAttrModelName;

    skel_INIT_PARAM_T() {
        release();
    }

    AX_VOID release() {
        strModelDeploymentPath = "";
        strHvcpModel = "";
        strHvcpModelName = "";
        strFaceModel = "";
        strFaceModelName = "";
        strFaceAttrModel = "";
        strFaceAttrModelName = "";
    }
} SKEL_INIT_PARAM_T;

///
class CSKELInit {
public:
    CSKELInit(SKEL_INIT_PARAM_T &stInitParam);
    virtual ~CSKELInit(AX_VOID) = default;

    virtual AX_S32 Init(AX_VOID);
    virtual AX_S32 DeInit(AX_VOID);
    virtual AX_S32 GetCapability(const AXCL_SKEL_CAPABILITY_T **ppstCapability);
    virtual AX_S32 GetVersion(const AXCL_SKEL_VERSION_INFO_T **ppstVersion);
    virtual AX_BOOL GetPPLStatus(AXCL_SKEL_PPL_E ePPL);
    virtual SKEL_INIT_PARAM_T GetInitParam(AX_VOID) const;
    virtual AX_VOID InitMemCallback(MEMMGR_ADDR pAddr, AX_VOID *pParam);

public:
    AX_BOOL m_bInited{AX_FALSE};
    SKEL_INIT_PARAM_T m_stInitParam;
};
}

using namespace axcl::skel;
