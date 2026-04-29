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
#include <map>
#include <mutex>
#include <queue>
#include <thread>
#include <string>
#include <vector>
#include <string>
#include <stdio.h>
#include <string.h>
#include "SkelFrame.hpp"
#include "Stage.hpp"
#include "axcl_npu.h"
#include "ax_skel_type.h"
#include "def.hpp"
#include "detection.hpp"
#include "MemMgr.hpp"
#include "skelInit.hpp"
#include "ThreadPool.hpp"
#include <condition_variable>

namespace axcl::skel {
#define SKEL_DUMP_SRC_ENV_STR "SKEL_DUMP_SRC_IMAGE"
#define SKEL_DUMP_BODY_ENV_STR "SKEL_DUMP_BODY_IMAGE"
#define SKEL_DUMP_POSE_ENV_STR "SKEL_DUMP_POSE_IMAGE"

#define SKEL_CSC_INTF_TYPE_ENV_STR "SKEL_CSC_INTF_TYPE" // VPP, TDP, VGP, NPU, CPU
#define SKEL_RESIZE_INTF_TYPE_ENV_STR "SKEL_RESIZE_INTF_TYPE" // VPP, TDP, VGP, NPU, CPU

#define DEFAULT_SRC_H 1920
#define DEFAULT_SRC_W 1080
#define DEFAULT_QPLEVEL 75
#define DEFAULT_QPLEVEL_MIN 1
#define DEFAULT_QPLEVEL_MAX 99
#define DEFAULT_FRAME_DEPTH 1
#define DEFAULT_FRAME_CACHE_DEPTH 1
#define DEFAULT_POSE_BODY_COUNT 3
#define DEFAULT_BODY_PROB_THRESHOLD 0.55
#define DEFAULT_VEHICLE_PROB_THRESHOLD 0.5
#define DEFAULT_CYCLE_PROB_THRESHOLD 0.5
#define DEFAULT_FACE_PROB_THRESHOLD 0.5
#define DEFAULT_PLATE_PROB_THRESHOLD 0.5

typedef enum axclSKEL_CV_INTF_TYPE_E {
    AXCL_SKEL_CV_INTF_TYPE_VPP,
    AXCL_SKEL_CV_INTF_TYPE_TDP,
    AXCL_SKEL_CV_INTF_TYPE_VGP,
    AXCL_SKEL_CV_INTF_TYPE_BUTT
} AXCL_SKEL_CV_INTF_TYPE_E;

typedef struct axclSKEL_MAX_TARGET_COUNT_T {
    AX_U8 nBodyTargetCount;
    AX_U8 nVehicleTargetCount;
    AX_U8 nCycleTargetCount;
} AXCL_SKEL_MAX_TARGET_COUNT_T;

typedef struct axclSKEL_FILTER_CONFIG_T {
    AX_U32 nWidth;
    AX_U32 nHeight;
    AX_F32 fConfidence;
} AXCL_SKEL_FILTER_CONFIG_T;

typedef struct axclSKEL_PARAM_T {
    AXCL_SKEL_PPL_E ePPL;
    AX_U32 nNpuType;
    AX_U32 nFrameDepth;
    AX_U32 nWidth;
    AX_U32 nHeight;
    AX_U32 nFrameCacheDepth;
    AX_U32 nIoDepth;
    AX_F32 fCropEncoderQpLevel;
    AX_BOOL bPushBindEnable;
    AX_BOOL bTrackEnable;
    AXCL_SKEL_ROI_CONFIG_T stRoi;
    AXCL_SKEL_PUSH_STRATEGY_T stPushStrategy;
    AXCL_SKEL_MAX_TARGET_COUNT_T stMaxTargetCount;
    AXCL_SKEL_CROP_ENCODER_THRESHOLD_CONFIG_T stCropEncoderThreshold;
    AXCL_SKEL_RESIZE_CONFIG_T stPanoramaResizeConfig;
    AXCL_SKEL_PUSH_PANORAMA_CONFIG_T stPushPanoramaConfig;
    CSkelMap<std::string, AXCL_SKEL_FILTER_CONFIG_T> stFilterMaps;
    CSkelMap<std::string, AXCL_SKEL_ATTR_FILTER_CONFIG_T> stAttrFliterMaps;

    axclSKEL_PARAM_T() {
        ePPL = AXCL_SKEL_PPL_HVCP;
        nNpuType = 0;
        nFrameDepth = DEFAULT_FRAME_DEPTH;
        nWidth = DEFAULT_SRC_H;
        nHeight = DEFAULT_SRC_W;
        nFrameCacheDepth = DEFAULT_FRAME_CACHE_DEPTH;
        nIoDepth = 0;
        fCropEncoderQpLevel = DEFAULT_QPLEVEL;
        bPushBindEnable = AX_TRUE;
        bTrackEnable = AX_TRUE;
        memset(&stRoi, 0x00, sizeof(stRoi));
        memset(&stMaxTargetCount, 0x00, sizeof(stMaxTargetCount));
        memset(&stCropEncoderThreshold, 0x00, sizeof(stCropEncoderThreshold));
        memset(&stPanoramaResizeConfig, 0x00, sizeof(stPanoramaResizeConfig));
        memset(&stPushPanoramaConfig, 0x00, sizeof(stPushPanoramaConfig));
        stFilterMaps["body"] = {0, 0, DEFAULT_BODY_PROB_THRESHOLD};
        stFilterMaps["vehicle"] = {0, 0, DEFAULT_VEHICLE_PROB_THRESHOLD};
        stFilterMaps["cycle"] = {0, 0, DEFAULT_CYCLE_PROB_THRESHOLD};
        stFilterMaps["face"] = {0, 0, DEFAULT_FACE_PROB_THRESHOLD};
        stFilterMaps["plate"] = {0, 0, DEFAULT_PLATE_PROB_THRESHOLD};
        stPushStrategy.ePushMode = AXCL_SKEL_PUSH_MODE_BEST;
        stPushStrategy.nIntervalTimes = 2000;
        stPushStrategy.nPushCounts = 1;
        stPushStrategy.bPushSameFrame = AX_FALSE;
        stAttrFliterMaps["body"].stCommonAttrFilterConfig.fQuality = 0;
        stAttrFliterMaps["vehicle"].stCommonAttrFilterConfig.fQuality = 0;
        stAttrFliterMaps["cycle"].stCommonAttrFilterConfig.fQuality = 0;
        stAttrFliterMaps["face"].stFaceAttrFilterConfig.nWidth = 0;
        stAttrFliterMaps["face"].stFaceAttrFilterConfig.nHeight = 0;
        stAttrFliterMaps["face"].stFaceAttrFilterConfig.stPoseblur.fPitch = 180;
        stAttrFliterMaps["face"].stFaceAttrFilterConfig.stPoseblur.fYaw = 180;
        stAttrFliterMaps["face"].stFaceAttrFilterConfig.stPoseblur.fRoll = 180;
        stAttrFliterMaps["face"].stFaceAttrFilterConfig.stPoseblur.fBlur = 1.0;
        stAttrFliterMaps["plate"].stCommonAttrFilterConfig.fQuality = 0;
    }
} AXCL_SKEL_PARAM_T;

typedef struct axclSKEL_CALLBACK_T {
    AXCL_SKEL_RESULT_CALLBACK_FUNC callback;
    AX_VOID *pUserData;
} AXCL_SKEL_CALLBACK_T;
}

using namespace axcl::skel;
