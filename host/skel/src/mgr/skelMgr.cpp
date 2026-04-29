/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "skelMgr.hpp"
#include "MemMgr.hpp"

namespace {
AX_S32 GetSkelModels(SKEL_INIT_PARAM_T &stInitParam) {
    DIR *dir_img = NULL;
    struct dirent *ptrl = NULL;
    const AX_CHAR *pStrModelDeploymentPath = stInitParam.strModelDeploymentPath.c_str();

    dir_img = opendir(pStrModelDeploymentPath);
    AX_U32 nLen = strlen(pStrModelDeploymentPath);

    if (!dir_img) {
        ALOGE("SKEL open dir: {} fail", pStrModelDeploymentPath);

        return AXCL_ERR_SKEL_ILLEGAL_PARAM;
    }

    while ((ptrl = readdir(dir_img)) != NULL) {
        if (ptrl->d_type == DT_DIR) {
            continue;
        }

        std::string pathName = pStrModelDeploymentPath;
        std::string fileName = ptrl->d_name;

        if (strstr(ptrl->d_name, SKEL_HVCP_MODEL_KEY_STR) != NULL) {
            stInitParam.strHvcpModelName = fileName;

            if (pStrModelDeploymentPath[nLen - 1] == '/') {
                stInitParam.strHvcpModel = pathName + fileName;
            } else {
                stInitParam.strHvcpModel = pathName + "/" + fileName;
            }
        } else if (strstr(ptrl->d_name, SKEL_FACE_MODEL_KEY_STR) != NULL) {
            stInitParam.strFaceModelName = fileName;

            if (pStrModelDeploymentPath[nLen - 1] == '/') {
                stInitParam.strFaceModel = pathName + fileName;
            } else {
                stInitParam.strFaceModel = pathName + "/" + fileName;
            }
        } else if (strstr(ptrl->d_name, SKEL_FACEATTR_MODEL_KEY_STR) != NULL) {
            stInitParam.strFaceAttrModelName = fileName;

            if (pStrModelDeploymentPath[nLen - 1] == '/') {
                stInitParam.strFaceAttrModel = pathName + fileName;
            } else {
                stInitParam.strFaceAttrModel = pathName + "/" + fileName;
            }
        }
    }

    closedir(dir_img);

    return AXCL_SKEL_SUCC;
}

AX_S32 CheckNpuTypeConfig(AX_U32 nNpuType) {
    AX_ENGINE_NPU_ATTR_T stNpuAttr;
    memset(&stNpuAttr, 0x00, sizeof(stNpuAttr));

    AX_S32 nRet = AXCL_ENGINE_GetVNPUAttr(&stNpuAttr);

    if (nRet != 0) {
        ALOGE("AXCL_ENGINE_GetVNPUAttr fail ret={:#x}", nRet);
        return AXCL_ERR_SKEL_ILLEGAL_PARAM;
    }

    if (stNpuAttr.eHardMode == AX_ENGINE_VIRTUAL_NPU_DISABLE) {
        if (nNpuType > 0) {
            ALOGE("vnpu disable, no allow nNpuType:{:#x}", nNpuType);
            return AXCL_ERR_SKEL_ILLEGAL_PARAM;
        }
    }
    else if (stNpuAttr.eHardMode == AX_ENGINE_VIRTUAL_NPU_STD) {
        if (nNpuType != (AX_U32)AXCL_SKEL_NPU_DEFAULT
            && nNpuType != (AX_U32)AXCL_SKEL_STD_VNPU_1
            && nNpuType != (AX_U32)AXCL_SKEL_STD_VNPU_2
            && nNpuType != (AX_U32)AXCL_SKEL_STD_VNPU_3) {
            ALOGE("vnpu STD, no allow nNpuType:{:#x}", nNpuType);
            return AXCL_ERR_SKEL_ILLEGAL_PARAM;
        }
    }
    else if (stNpuAttr.eHardMode == AX_ENGINE_VIRTUAL_NPU_BIG_LITTLE) {
        if (nNpuType != (AX_U32)AXCL_SKEL_NPU_DEFAULT
            && nNpuType != (AX_U32)AXCL_SKEL_BL_VNPU_1
            && nNpuType != (AX_U32)AXCL_SKEL_BL_VNPU_2) {
            ALOGE("vnpu BIG-LITTLE, no allow nNpuType:{:#x}", nNpuType);
            return AXCL_ERR_SKEL_ILLEGAL_PARAM;
        }
    }
    else {
        ALOGE("get vnpu attribute fail, eHardMode:{}, nNpuType:{:#x}", static_cast<int>(stNpuAttr.eHardMode), nNpuType);
        return AXCL_ERR_SKEL_ILLEGAL_PARAM;
    }

    return AXCL_SKEL_SUCC;
}

AX_S32 ParseConfigParam(const AXCL_SKEL_CONFIG_T &stConfig, AXCL_SKEL_PARAM_T &stParam) {
    if (stConfig.nSize > 0 && stConfig.pstItems) {
        for (size_t i = 0; i < stConfig.nSize; i++) {
            if (stConfig.pstItems[i].pstrType && stConfig.pstItems[i].pstrValue) {
                // cmd: "body_max_target_count", value_type: AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T *
                if (strcmp(stConfig.pstItems[i].pstrType, "body_max_target_count") == 0) {
                    if (stConfig.pstItems[i].nValueSize == sizeof(AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T)) {
                        AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T *pstConf = (AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T *)stConfig.pstItems[i].pstrValue;
                        stParam.stMaxTargetCount.nBodyTargetCount = (AX_U8)pstConf->fValue;
                        ALOGI("SKEL set {}: {}", stConfig.pstItems[i].pstrType, stParam.stMaxTargetCount.nBodyTargetCount);
                    } else {
                        ALOGE("SKEL {} size({}) no match", stConfig.pstItems[i].pstrType, stConfig.pstItems[i].nValueSize);
                    }
                }
                // cmd: "vehicle_max_target_count", value_type: AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T *
                else if (strcmp(stConfig.pstItems[i].pstrType, "vehicle_max_target_count") == 0) {
                    if (stConfig.pstItems[i].nValueSize == sizeof(AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T)) {
                        AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T *pstConf = (AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T *)stConfig.pstItems[i].pstrValue;
                        stParam.stMaxTargetCount.nVehicleTargetCount = (AX_U8)pstConf->fValue;
                        ALOGI("SKEL set {}: {}", stConfig.pstItems[i].pstrType, stParam.stMaxTargetCount.nVehicleTargetCount);
                    } else {
                        ALOGE("SKEL {} size({}) no match", stConfig.pstItems[i].pstrType, stConfig.pstItems[i].nValueSize);
                    }
                }
                // cmd: "cycle_max_target_count", value_type: AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T *
                else if (strcmp(stConfig.pstItems[i].pstrType, "cycle_max_target_count") == 0) {
                    if (stConfig.pstItems[i].nValueSize == sizeof(AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T)) {
                        AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T *pstConf = (AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T *)stConfig.pstItems[i].pstrValue;
                        stParam.stMaxTargetCount.nCycleTargetCount = (AX_U8)pstConf->fValue;
                        ALOGI("SKEL set {}: {}", stConfig.pstItems[i].pstrType, stParam.stMaxTargetCount.nCycleTargetCount);
                    } else {
                        ALOGE("SKEL {} size({}) no match", stConfig.pstItems[i].pstrType, stConfig.pstItems[i].nValueSize);
                    }
                }
                // cmd: "body_confidence", value_type: AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T *
                else if (strcmp(stConfig.pstItems[i].pstrType, "body_confidence") == 0) {
                    if (stConfig.pstItems[i].nValueSize == sizeof(AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T)) {
                        AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T *pstConf = (AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T *)stConfig.pstItems[i].pstrValue;
                        stParam.stFilterMaps["body"].fConfidence = pstConf->fValue;
                        ALOGI("SKEL set {}: {}", stConfig.pstItems[i].pstrType, stParam.stFilterMaps["body"].fConfidence);
                    } else {
                        ALOGE("SKEL {} size({}) no match", stConfig.pstItems[i].pstrType, stConfig.pstItems[i].nValueSize);
                    }
                }
                // cmd: "face_confidence", value_type: AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T *
                else if (strcmp(stConfig.pstItems[i].pstrType, "face_confidence") == 0) {
                    if (stConfig.pstItems[i].nValueSize == sizeof(AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T)) {
                        AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T *pstConf = (AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T *)stConfig.pstItems[i].pstrValue;
                        stParam.stFilterMaps["face"].fConfidence = pstConf->fValue;
                        ALOGI("SKEL set {}: {}", stConfig.pstItems[i].pstrType, stParam.stFilterMaps["face"].fConfidence);
                    } else {
                        ALOGE("SKEL {} size({}) no match", stConfig.pstItems[i].pstrType, stConfig.pstItems[i].nValueSize);
                    }
                }
                // cmd: "vehicle_confidence", value_type: AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T *
                else if (strcmp(stConfig.pstItems[i].pstrType, "vehicle_confidence") == 0) {
                    if (stConfig.pstItems[i].nValueSize == sizeof(AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T)) {
                        AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T *pstConf = (AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T *)stConfig.pstItems[i].pstrValue;
                        stParam.stFilterMaps["vehicle"].fConfidence = pstConf->fValue;
                        ALOGI("SKEL set {}: {}", stConfig.pstItems[i].pstrType, stParam.stFilterMaps["vehicle"].fConfidence);
                    } else {
                        ALOGE("SKEL {} size({}) no match", stConfig.pstItems[i].pstrType, stConfig.pstItems[i].nValueSize);
                    }
                }
                // cmd: "cycle_confidence", value_type: AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T *
                else if (strcmp(stConfig.pstItems[i].pstrType, "cycle_confidence") == 0) {
                    if (stConfig.pstItems[i].nValueSize == sizeof(AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T)) {
                        AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T *pstConf = (AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T *)stConfig.pstItems[i].pstrValue;
                        stParam.stFilterMaps["cycle"].fConfidence = pstConf->fValue;
                        ALOGI("SKEL set {}: {}", stConfig.pstItems[i].pstrType, stParam.stFilterMaps["cycle"].fConfidence);
                    } else {
                        ALOGE("SKEL {} size({}) no match", stConfig.pstItems[i].pstrType, stConfig.pstItems[i].nValueSize);
                    }
                }
                // cmd: "plate_confidence", value_type: AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T *
                else if (strcmp(stConfig.pstItems[i].pstrType, "plate_confidence") == 0) {
                    if (stConfig.pstItems[i].nValueSize == sizeof(AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T)) {
                        AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T *pstConf = (AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T *)stConfig.pstItems[i].pstrValue;
                        stParam.stFilterMaps["plate"].fConfidence = pstConf->fValue;
                        ALOGI("SKEL set {}: {}", stConfig.pstItems[i].pstrType, stParam.stFilterMaps["plate"].fConfidence);
                    } else {
                        ALOGE("SKEL {} size({}) no match", stConfig.pstItems[i].pstrType, stConfig.pstItems[i].nValueSize);
                    }
                }
                // cmd: "crop_encoder_qpLevel", value_type: AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T *
                else if (strcmp(stConfig.pstItems[i].pstrType, "crop_encoder_qpLevel") == 0) {
                    if (stConfig.pstItems[i].nValueSize == sizeof(AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T)) {
                        AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T *pstConf = (AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T *)stConfig.pstItems[i].pstrValue;
                        stParam.fCropEncoderQpLevel = pstConf->fValue;
                        ALOGI("SKEL set {}: {}", stConfig.pstItems[i].pstrType, stParam.fCropEncoderQpLevel);
                    } else {
                        ALOGE("SKEL {} size({}) no match", stConfig.pstItems[i].pstrType, stConfig.pstItems[i].nValueSize);
                    }
                }
                // cmd: "body_min_size",  value_type: AXCL_SKEL_OBJECT_SIZE_FILTER_CONFIG_T *
                else if (strcmp(stConfig.pstItems[i].pstrType, "body_min_size") == 0) {
                    if (stConfig.pstItems[i].nValueSize == sizeof(AXCL_SKEL_OBJECT_SIZE_FILTER_CONFIG_T)) {
                        AXCL_SKEL_OBJECT_SIZE_FILTER_CONFIG_T *pstConf =
                            (AXCL_SKEL_OBJECT_SIZE_FILTER_CONFIG_T *)stConfig.pstItems[i].pstrValue;
                        stParam.stFilterMaps["body"].nWidth = pstConf->nWidth;
                        stParam.stFilterMaps["body"].nHeight = pstConf->nHeight;
                        ALOGI("SKEL set {} {}x{}", stConfig.pstItems[i].pstrType, stParam.stFilterMaps["body"].nWidth, stParam.stFilterMaps["body"].nHeight);
                    } else {
                        ALOGE("SKEL {} size({}) no match", stConfig.pstItems[i].pstrType, stConfig.pstItems[i].nValueSize);
                    }
                }
                // cmd: "face_min_size",  value_type: AXCL_SKEL_OBJECT_SIZE_FILTER_CONFIG_T *
                else if (strcmp(stConfig.pstItems[i].pstrType, "face_min_size") == 0) {
                    if (stConfig.pstItems[i].nValueSize == sizeof(AXCL_SKEL_OBJECT_SIZE_FILTER_CONFIG_T)) {
                        AXCL_SKEL_OBJECT_SIZE_FILTER_CONFIG_T *pstConf =
                            (AXCL_SKEL_OBJECT_SIZE_FILTER_CONFIG_T *)stConfig.pstItems[i].pstrValue;
                        stParam.stFilterMaps["face"].nWidth = pstConf->nWidth;
                        stParam.stFilterMaps["face"].nHeight = pstConf->nHeight;
                        ALOGI("SKEL set {} {}x{}", stConfig.pstItems[i].pstrType, stParam.stFilterMaps["face"].nWidth, stParam.stFilterMaps["face"].nHeight);
                    } else {
                        ALOGE("SKEL {} size({}) no match", stConfig.pstItems[i].pstrType, stConfig.pstItems[i].nValueSize);
                    }
                }
                // cmd: "vehicle_min_size",  value_type: AXCL_SKEL_OBJECT_SIZE_FILTER_CONFIG_T *
                else if (strcmp(stConfig.pstItems[i].pstrType, "vehicle_min_size") == 0) {
                    if (stConfig.pstItems[i].nValueSize == sizeof(AXCL_SKEL_OBJECT_SIZE_FILTER_CONFIG_T)) {
                        AXCL_SKEL_OBJECT_SIZE_FILTER_CONFIG_T *pstConf =
                            (AXCL_SKEL_OBJECT_SIZE_FILTER_CONFIG_T *)stConfig.pstItems[i].pstrValue;
                        stParam.stFilterMaps["vehicle"].nWidth = pstConf->nWidth;
                        stParam.stFilterMaps["vehicle"].nHeight = pstConf->nHeight;
                        ALOGI("SKEL set {} {}x{}", stConfig.pstItems[i].pstrType, stParam.stFilterMaps["vehicle"].nWidth, stParam.stFilterMaps["vehicle"].nHeight);
                    } else {
                        ALOGE("SKEL {} size({}) no match", stConfig.pstItems[i].pstrType, stConfig.pstItems[i].nValueSize);
                    }
                }
                // cmd: "cycle_min_size",  value_type: AXCL_SKEL_OBJECT_SIZE_FILTER_CONFIG_T *
                else if (strcmp(stConfig.pstItems[i].pstrType, "cycle_min_size") == 0) {
                    if (stConfig.pstItems[i].nValueSize == sizeof(AXCL_SKEL_OBJECT_SIZE_FILTER_CONFIG_T)) {
                        AXCL_SKEL_OBJECT_SIZE_FILTER_CONFIG_T *pstConf =
                            (AXCL_SKEL_OBJECT_SIZE_FILTER_CONFIG_T *)stConfig.pstItems[i].pstrValue;
                        stParam.stFilterMaps["cycle"].nWidth = pstConf->nWidth;
                        stParam.stFilterMaps["cycle"].nHeight = pstConf->nHeight;
                        ALOGI("SKEL set {} {}x{}", stConfig.pstItems[i].pstrType, stParam.stFilterMaps["cycle"].nWidth, stParam.stFilterMaps["cycle"].nHeight);
                    } else {
                        ALOGE("SKEL {} size({}) no match", stConfig.pstItems[i].pstrType, stConfig.pstItems[i].nValueSize);
                    }
                }
                // cmd: "plate_min_size",  value_type: AXCL_SKEL_OBJECT_SIZE_FILTER_CONFIG_T *
                else if (strcmp(stConfig.pstItems[i].pstrType, "plate_min_size") == 0) {
                    if (stConfig.pstItems[i].nValueSize == sizeof(AXCL_SKEL_OBJECT_SIZE_FILTER_CONFIG_T)) {
                        AXCL_SKEL_OBJECT_SIZE_FILTER_CONFIG_T *pstConf =
                            (AXCL_SKEL_OBJECT_SIZE_FILTER_CONFIG_T *)stConfig.pstItems[i].pstrValue;
                        stParam.stFilterMaps["plate"].nWidth = pstConf->nWidth;
                        stParam.stFilterMaps["plate"].nHeight = pstConf->nHeight;
                        ALOGI("SKEL set {} {}x{}", stConfig.pstItems[i].pstrType, stParam.stFilterMaps["plate"].nWidth, stParam.stFilterMaps["plate"].nHeight);
                    } else {
                        ALOGE("SKEL {} size({}) no match", stConfig.pstItems[i].pstrType, stConfig.pstItems[i].nValueSize);
                    }
                }
                // cmd: "detect_roi",  value_type: AXCL_SKEL_ROI_CONFIG_T *
                else if (strcmp(stConfig.pstItems[i].pstrType, "detect_roi") == 0) {
                    if (stConfig.pstItems[i].nValueSize == sizeof(AXCL_SKEL_ROI_CONFIG_T)) {
                        AXCL_SKEL_ROI_CONFIG_T *pstConf = (AXCL_SKEL_ROI_CONFIG_T *)stConfig.pstItems[i].pstrValue;
                        stParam.stRoi = *pstConf;
                        ALOGI("SKEL set {} [{}]:[{},{},{},{}]", stConfig.pstItems[i].pstrType, stParam.stRoi.bEnable, stParam.stRoi.stRect.fX,
                              stParam.stRoi.stRect.fY, stParam.stRoi.stRect.fW, stParam.stRoi.stRect.fH);
                    } else {
                        ALOGE("SKEL {} size({}) no match", stConfig.pstItems[i].pstrType, stConfig.pstItems[i].nValueSize);
                    }
                }
                // cmd: "push_strategy",  value_type: AXCL_SKEL_PUSH_STRATEGY_T *
                else if (strcmp(stConfig.pstItems[i].pstrType, "push_strategy") == 0) {
                    if (stConfig.pstItems[i].nValueSize == sizeof(AXCL_SKEL_PUSH_STRATEGY_T)) {
                        AXCL_SKEL_PUSH_STRATEGY_T *pstConf = (AXCL_SKEL_PUSH_STRATEGY_T *)stConfig.pstItems[i].pstrValue;
                        stParam.stPushStrategy = *pstConf;
                        ALOGI("SKEL set {} [mode:{}, times:{}, count:{}, same:{}]", stConfig.pstItems[i].pstrType, stParam.stPushStrategy.ePushMode, stParam.stPushStrategy.nIntervalTimes,
                                stParam.stPushStrategy.nPushCounts, stParam.stPushStrategy.bPushSameFrame);
                    } else {
                        ALOGE("SKEL {} size({}) no match", stConfig.pstItems[i].pstrType, stConfig.pstItems[i].nValueSize);
                    }
                }
                // cmd: "crop_encoder",  value_type: AXCL_SKEL_CROP_ENCODER_THRESHOLD_CONFIG_T *
                else if (strcmp(stConfig.pstItems[i].pstrType, "crop_encoder") == 0) {
                    if (stConfig.pstItems[i].nValueSize == sizeof(AXCL_SKEL_CROP_ENCODER_THRESHOLD_CONFIG_T)) {
                        AXCL_SKEL_CROP_ENCODER_THRESHOLD_CONFIG_T *pstConf = (AXCL_SKEL_CROP_ENCODER_THRESHOLD_CONFIG_T *)stConfig.pstItems[i].pstrValue;
                        stParam.stCropEncoderThreshold = *pstConf;
                        ALOGI("SKEL set {} [{}, {}, {}, {}]", stConfig.pstItems[i].pstrType,
                                stParam.stCropEncoderThreshold.fScaleLeft, stParam.stCropEncoderThreshold.fScaleRight,
                                stParam.stCropEncoderThreshold.fScaleTop, stParam.stCropEncoderThreshold.fScaleBottom);
                    } else {
                        ALOGE("SKEL {} size({}) no match", stConfig.pstItems[i].pstrType, stConfig.pstItems[i].nValueSize);
                    }
                }
                // cmd: "resize_panorama_encoder_config",  value_type: AXCL_SKEL_RESIZE_CONFIG *
                else if (strcmp(stConfig.pstItems[i].pstrType, "resize_panorama_encoder_config") == 0) {
                    if (stConfig.pstItems[i].nValueSize == sizeof(AXCL_SKEL_RESIZE_CONFIG_T)) {
                        AXCL_SKEL_RESIZE_CONFIG_T *pstConf = (AXCL_SKEL_RESIZE_CONFIG_T *)stConfig.pstItems[i].pstrValue;
                        stParam.stPanoramaResizeConfig = *pstConf;
                        ALOGI("SKEL set {} [{}, {}]", stConfig.pstItems[i].pstrType, stParam.stPanoramaResizeConfig.fW, stParam.stPanoramaResizeConfig.fH);
                    } else {
                        ALOGE("SKEL {} size({}) no match", stConfig.pstItems[i].pstrType, stConfig.pstItems[i].nValueSize);
                    }
                }
                // cmd: "push_panorama",  value_type: AXCL_SKEL_PUSH_PANORAMA_CONFIG_T *
                else if (strcmp(stConfig.pstItems[i].pstrType, "push_panorama") == 0) {
                    if (stConfig.pstItems[i].nValueSize == sizeof(AXCL_SKEL_PUSH_PANORAMA_CONFIG_T)) {
                        AXCL_SKEL_PUSH_PANORAMA_CONFIG_T *pstConf = (AXCL_SKEL_PUSH_PANORAMA_CONFIG_T *)stConfig.pstItems[i].pstrValue;
                        stParam.stPushPanoramaConfig = *pstConf;
                        ALOGI("SKEL set {} [Enable: {}, Quality: {}]", stConfig.pstItems[i].pstrType,
                                stParam.stPushPanoramaConfig.bEnable, stParam.stPushPanoramaConfig.nQuality);
                    } else {
                        ALOGE("SKEL {} size({}) no match", stConfig.pstItems[i].pstrType, stConfig.pstItems[i].nValueSize);
                    }
                }
                // cmd: "push_quality_body",  value_type: AXCL_SKEL_ATTR_FILTER_CONFIG_T *
                else if (strcmp(stConfig.pstItems[i].pstrType, "push_quality_body") == 0) {
                    if (stConfig.pstItems[i].nValueSize == sizeof(AXCL_SKEL_ATTR_FILTER_CONFIG_T)) {
                        AXCL_SKEL_ATTR_FILTER_CONFIG_T *pstConf = (AXCL_SKEL_ATTR_FILTER_CONFIG_T *)stConfig.pstItems[i].pstrValue;
                        stParam.stAttrFliterMaps["body"] = *pstConf;
                        ALOGI("SKEL set {} [Q: {}]", stConfig.pstItems[i].pstrType, stParam.stAttrFliterMaps["body"].stCommonAttrFilterConfig.fQuality);
                    } else {
                        ALOGE("SKEL {} size({}) no match", stConfig.pstItems[i].pstrType, stConfig.pstItems[i].nValueSize);
                    }
                }
                // cmd: "push_quality_vehicle",  value_type: AXCL_SKEL_ATTR_FILTER_CONFIG_T *
                else if (strcmp(stConfig.pstItems[i].pstrType, "push_quality_vehicle") == 0) {
                    if (stConfig.pstItems[i].nValueSize == sizeof(AXCL_SKEL_ATTR_FILTER_CONFIG_T)) {
                        AXCL_SKEL_ATTR_FILTER_CONFIG_T *pstConf = (AXCL_SKEL_ATTR_FILTER_CONFIG_T *)stConfig.pstItems[i].pstrValue;
                        stParam.stAttrFliterMaps["vehicle"] = *pstConf;
                        ALOGI("SKEL set {} [Q: {}]", stConfig.pstItems[i].pstrType, stParam.stAttrFliterMaps["vehicle"].stCommonAttrFilterConfig.fQuality);
                    } else {
                        ALOGE("SKEL {} size({}) no match", stConfig.pstItems[i].pstrType, stConfig.pstItems[i].nValueSize);
                    }
                }
                // cmd: "push_quality_cycle",  value_type: AXCL_SKEL_ATTR_FILTER_CONFIG_T *
                else if (strcmp(stConfig.pstItems[i].pstrType, "push_quality_cycle") == 0) {
                    if (stConfig.pstItems[i].nValueSize == sizeof(AXCL_SKEL_ATTR_FILTER_CONFIG_T)) {
                        AXCL_SKEL_ATTR_FILTER_CONFIG_T *pstConf = (AXCL_SKEL_ATTR_FILTER_CONFIG_T *)stConfig.pstItems[i].pstrValue;
                        stParam.stAttrFliterMaps["cycle"] = *pstConf;
                        ALOGI("SKEL set {} [Q: {}]", stConfig.pstItems[i].pstrType, stParam.stAttrFliterMaps["cycle"].stCommonAttrFilterConfig.fQuality);
                    } else {
                        ALOGE("SKEL {} size({}) no match", stConfig.pstItems[i].pstrType, stConfig.pstItems[i].nValueSize);
                    }
                }
                // cmd: "push_quality_face",  value_type: AXCL_SKEL_ATTR_FILTER_CONFIG_T *
                else if (strcmp(stConfig.pstItems[i].pstrType, "push_quality_face") == 0) {
                    if (stConfig.pstItems[i].nValueSize == sizeof(AXCL_SKEL_ATTR_FILTER_CONFIG_T)) {
                        AXCL_SKEL_ATTR_FILTER_CONFIG_T *pstConf = (AXCL_SKEL_ATTR_FILTER_CONFIG_T *)stConfig.pstItems[i].pstrValue;
                        stParam.stAttrFliterMaps["face"] = *pstConf;
                        ALOGI("SKEL set {} [W: {}, H: {}, P: {}, Y: {}, R: {}, B: {}]", stConfig.pstItems[i].pstrType,
                                stParam.stAttrFliterMaps["face"].stFaceAttrFilterConfig.nWidth, stParam.stAttrFliterMaps["face"].stFaceAttrFilterConfig.nHeight,
                                stParam.stAttrFliterMaps["face"].stFaceAttrFilterConfig.stPoseblur.fPitch, stParam.stAttrFliterMaps["face"].stFaceAttrFilterConfig.stPoseblur.fYaw,
                                stParam.stAttrFliterMaps["face"].stFaceAttrFilterConfig.stPoseblur.fRoll, stParam.stAttrFliterMaps["face"].stFaceAttrFilterConfig.stPoseblur.fBlur);
                    } else {
                        ALOGE("SKEL {} size({}) no match", stConfig.pstItems[i].pstrType, stConfig.pstItems[i].nValueSize);
                    }
                }
                // cmd: "push_quality_plate",  value_type: AXCL_SKEL_ATTR_FILTER_CONFIG_T *
                else if (strcmp(stConfig.pstItems[i].pstrType, "push_quality_plate") == 0) {
                    if (stConfig.pstItems[i].nValueSize == sizeof(AXCL_SKEL_ATTR_FILTER_CONFIG_T)) {
                        AXCL_SKEL_ATTR_FILTER_CONFIG_T *pstConf = (AXCL_SKEL_ATTR_FILTER_CONFIG_T *)stConfig.pstItems[i].pstrValue;
                        stParam.stAttrFliterMaps["plate"] = *pstConf;
                        ALOGI("SKEL set {} [Q: {}]", stConfig.pstItems[i].pstrType, stParam.stAttrFliterMaps["plate"].stCommonAttrFilterConfig.fQuality);
                    } else {
                        ALOGE("SKEL {} size({}) no match", stConfig.pstItems[i].pstrType, stConfig.pstItems[i].nValueSize);
                    }
                }
                // cmd: "track_disable",  value_type: AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T *
                else if (strcmp(stConfig.pstItems[i].pstrType, "track_disable") == 0) {
                    if (stConfig.pstItems[i].nValueSize == sizeof(AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T)) {
                        AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T *pstConf = (AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T *)stConfig.pstItems[i].pstrValue;
                        stParam.bTrackEnable = (AX_BOOL)(!((AX_BOOL)pstConf->fValue));
                        ALOGI("SKEL set {}: {}", stConfig.pstItems[i].pstrType, !stParam.bTrackEnable);
                    } else {
                        ALOGE("SKEL {} size({}) no match", stConfig.pstItems[i].pstrType, stConfig.pstItems[i].nValueSize);
                    }
                }
                else {
                    ALOGE("SKEL cmd: {} not support", stConfig.pstItems[i].pstrType);
                }
            }
        }
    }

    return AXCL_SKEL_SUCC;
}

AX_S32 GenerateConfigParam(AXCL_SKEL_PARAM_T &stParam, AXCL_SKEL_CONFIG_T &stConfig) {
    // FIXME.
    stConfig.nSize = 27;
    stConfig.pstItems = new AXCL_SKEL_CONFIG_ITEM_T[stConfig.nSize];

    if (!stConfig.pstItems) {
        ALOGE("SKEL alloc item fail");
        return AXCL_ERR_SKEL_NOMEM;
    }

    AX_U32 nSize = 0;
    memset(stConfig.pstItems, 0x00, sizeof(AXCL_SKEL_CONFIG_ITEM_T) * stConfig.nSize);

    // body_max_target_count
    stConfig.pstItems[nSize].pstrType = (AX_CHAR *)"body_max_target_count";
    stConfig.pstItems[nSize].nValueSize = sizeof(AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T);
    AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T stBodyMaxTargetCountThreshold;
    stBodyMaxTargetCountThreshold.fValue = (AX_F32)stParam.stMaxTargetCount.nBodyTargetCount;
    stConfig.pstItems[nSize].pstrValue = (AX_VOID *)malloc(stConfig.pstItems[nSize].nValueSize);
    memcpy(stConfig.pstItems[nSize].pstrValue, &stBodyMaxTargetCountThreshold, stConfig.pstItems[nSize].nValueSize);
    nSize++;

    // vehicle_max_target_count
    stConfig.pstItems[nSize].pstrType = (AX_CHAR *)"vehicle_max_target_count";
    stConfig.pstItems[nSize].nValueSize = sizeof(AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T);
    AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T stVehicleMaxTargetCountThreshold;
    stVehicleMaxTargetCountThreshold.fValue = (AX_F32)stParam.stMaxTargetCount.nVehicleTargetCount;
    stConfig.pstItems[nSize].pstrValue = (AX_VOID *)malloc(stConfig.pstItems[nSize].nValueSize);
    memcpy(stConfig.pstItems[nSize].pstrValue, &stVehicleMaxTargetCountThreshold, stConfig.pstItems[nSize].nValueSize);
    nSize++;

    // cycle_max_target_count
    stConfig.pstItems[nSize].pstrType = (AX_CHAR *)"cycle_max_target_count";
    stConfig.pstItems[nSize].nValueSize = sizeof(AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T);
    AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T stCycleMaxTargetCountThreshold;
    stCycleMaxTargetCountThreshold.fValue = (AX_F32)stParam.stMaxTargetCount.nCycleTargetCount;
    stConfig.pstItems[nSize].pstrValue = (AX_VOID *)malloc(stConfig.pstItems[nSize].nValueSize);
    memcpy(stConfig.pstItems[nSize].pstrValue, &stCycleMaxTargetCountThreshold, stConfig.pstItems[nSize].nValueSize);
    nSize++;

    // body_confidence
    stConfig.pstItems[nSize].pstrType = (AX_CHAR *)"body_confidence";
    stConfig.pstItems[nSize].nValueSize = sizeof(AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T);
    AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T stBodyConfidenceThreshold;
    stBodyConfidenceThreshold.fValue = (AX_F32)stParam.stFilterMaps["body"].fConfidence;
    stConfig.pstItems[nSize].pstrValue = (AX_VOID *)malloc(stConfig.pstItems[nSize].nValueSize);
    memcpy(stConfig.pstItems[nSize].pstrValue, &stBodyConfidenceThreshold, stConfig.pstItems[nSize].nValueSize);
    nSize++;

    // face_confidence
    stConfig.pstItems[nSize].pstrType = (AX_CHAR *)"face_confidence";
    stConfig.pstItems[nSize].nValueSize = sizeof(AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T);
    AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T stFaceConfidenceThreshold;
    stFaceConfidenceThreshold.fValue = (AX_F32)stParam.stFilterMaps["face"].fConfidence;
    stConfig.pstItems[nSize].pstrValue = (AX_VOID *)malloc(stConfig.pstItems[nSize].nValueSize);
    memcpy(stConfig.pstItems[nSize].pstrValue, &stFaceConfidenceThreshold, stConfig.pstItems[nSize].nValueSize);
    nSize++;

    // vehicle_confidence
    stConfig.pstItems[nSize].pstrType = (AX_CHAR *)"vehicle_confidence";
    stConfig.pstItems[nSize].nValueSize = sizeof(AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T);
    AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T stVehicleConfidenceThreshold;
    stVehicleConfidenceThreshold.fValue = (AX_F32)stParam.stFilterMaps["vehicle"].fConfidence;
    stConfig.pstItems[nSize].pstrValue = (AX_VOID *)malloc(stConfig.pstItems[nSize].nValueSize);
    memcpy(stConfig.pstItems[nSize].pstrValue, &stVehicleConfidenceThreshold, stConfig.pstItems[nSize].nValueSize);
    nSize++;

    // cycle_confidence
    stConfig.pstItems[nSize].pstrType = (AX_CHAR *)"cycle_confidence";
    stConfig.pstItems[nSize].nValueSize = sizeof(AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T);
    AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T stCycleConfidenceThreshold;
    stCycleConfidenceThreshold.fValue = (AX_F32)stParam.stFilterMaps["cycle"].fConfidence;
    stConfig.pstItems[nSize].pstrValue = (AX_VOID *)malloc(stConfig.pstItems[nSize].nValueSize);
    memcpy(stConfig.pstItems[nSize].pstrValue, &stCycleConfidenceThreshold, stConfig.pstItems[nSize].nValueSize);
    nSize++;

    // plate_confidence
    stConfig.pstItems[nSize].pstrType = (AX_CHAR *)"plate_confidence";
    stConfig.pstItems[nSize].nValueSize = sizeof(AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T);
    AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T stPlateConfidenceThreshold;
    stPlateConfidenceThreshold.fValue = (AX_F32)stParam.stFilterMaps["plate"].fConfidence;
    stConfig.pstItems[nSize].pstrValue = (AX_VOID *)malloc(stConfig.pstItems[nSize].nValueSize);
    memcpy(stConfig.pstItems[nSize].pstrValue, &stPlateConfidenceThreshold, stConfig.pstItems[nSize].nValueSize);
    nSize++;

    // crop_encoder_qpLevel
    stConfig.pstItems[nSize].pstrType = (AX_CHAR *)"crop_encoder_qpLevel";
    stConfig.pstItems[nSize].nValueSize = sizeof(AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T);
    AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T stCropEncoderQpLevelThreshold;
    stCropEncoderQpLevelThreshold.fValue = (AX_F32)stParam.fCropEncoderQpLevel;
    stConfig.pstItems[nSize].pstrValue = (AX_VOID *)malloc(stConfig.pstItems[nSize].nValueSize);
    memcpy(stConfig.pstItems[nSize].pstrValue, &stCropEncoderQpLevelThreshold, stConfig.pstItems[nSize].nValueSize);
    nSize++;

    // body_min_size
    stConfig.pstItems[nSize].pstrType = (AX_CHAR *)"body_min_size";
    stConfig.pstItems[nSize].nValueSize = sizeof(AXCL_SKEL_OBJECT_SIZE_FILTER_CONFIG_T);
    AXCL_SKEL_OBJECT_SIZE_FILTER_CONFIG_T stBodySizeFilter;
    stBodySizeFilter.nWidth = stParam.stFilterMaps["body"].nWidth;
    stBodySizeFilter.nHeight = stParam.stFilterMaps["body"].nHeight;
    stConfig.pstItems[nSize].pstrValue = (AX_VOID *)malloc(stConfig.pstItems[nSize].nValueSize);
    memcpy(stConfig.pstItems[nSize].pstrValue, &stBodySizeFilter, stConfig.pstItems[nSize].nValueSize);
    nSize++;

    // face_min_size
    stConfig.pstItems[nSize].pstrType = (AX_CHAR *)"face_min_size";
    stConfig.pstItems[nSize].nValueSize = sizeof(AXCL_SKEL_OBJECT_SIZE_FILTER_CONFIG_T);
    AXCL_SKEL_OBJECT_SIZE_FILTER_CONFIG_T stFaceSizeFilter;
    stFaceSizeFilter.nWidth = stParam.stFilterMaps["face"].nWidth;
    stFaceSizeFilter.nHeight = stParam.stFilterMaps["face"].nHeight;
    stConfig.pstItems[nSize].pstrValue = (AX_VOID *)malloc(stConfig.pstItems[nSize].nValueSize);
    memcpy(stConfig.pstItems[nSize].pstrValue, &stFaceSizeFilter, stConfig.pstItems[nSize].nValueSize);
    nSize++;

    // vehicle_min_size
    stConfig.pstItems[nSize].pstrType = (AX_CHAR *)"vehicle_min_size";
    stConfig.pstItems[nSize].nValueSize = sizeof(AXCL_SKEL_OBJECT_SIZE_FILTER_CONFIG_T);
    AXCL_SKEL_OBJECT_SIZE_FILTER_CONFIG_T stVehicleSizeFilter;
    stVehicleSizeFilter.nWidth = stParam.stFilterMaps["vehicle"].nWidth;
    stVehicleSizeFilter.nHeight = stParam.stFilterMaps["vehicle"].nHeight;
    stConfig.pstItems[nSize].pstrValue = (AX_VOID *)malloc(stConfig.pstItems[nSize].nValueSize);
    memcpy(stConfig.pstItems[nSize].pstrValue, &stVehicleSizeFilter, stConfig.pstItems[nSize].nValueSize);
    nSize++;

    // cycle_min_size
    stConfig.pstItems[nSize].pstrType = (AX_CHAR *)"cycle_min_size";
    stConfig.pstItems[nSize].nValueSize = sizeof(AXCL_SKEL_OBJECT_SIZE_FILTER_CONFIG_T);
    AXCL_SKEL_OBJECT_SIZE_FILTER_CONFIG_T stCycleSizeFilter;
    stCycleSizeFilter.nWidth = stParam.stFilterMaps["cycle"].nWidth;
    stCycleSizeFilter.nHeight = stParam.stFilterMaps["cycle"].nHeight;
    stConfig.pstItems[nSize].pstrValue = (AX_VOID *)malloc(stConfig.pstItems[nSize].nValueSize);
    memcpy(stConfig.pstItems[nSize].pstrValue, &stCycleSizeFilter, stConfig.pstItems[nSize].nValueSize);
    nSize++;

    // plate_min_size
    stConfig.pstItems[nSize].pstrType = (AX_CHAR *)"plate_min_size";
    stConfig.pstItems[nSize].nValueSize = sizeof(AXCL_SKEL_OBJECT_SIZE_FILTER_CONFIG_T);
    AXCL_SKEL_OBJECT_SIZE_FILTER_CONFIG_T stPlateSizeFilter;
    stPlateSizeFilter.nWidth = stParam.stFilterMaps["plate"].nWidth;
    stPlateSizeFilter.nHeight = stParam.stFilterMaps["plate"].nHeight;
    stConfig.pstItems[nSize].pstrValue = (AX_VOID *)malloc(stConfig.pstItems[nSize].nValueSize);
    memcpy(stConfig.pstItems[nSize].pstrValue, &stPlateSizeFilter, stConfig.pstItems[nSize].nValueSize);
    nSize++;

    // detect_roi
    stConfig.pstItems[nSize].pstrType = (AX_CHAR *)"detect_roi";
    stConfig.pstItems[nSize].nValueSize = sizeof(AXCL_SKEL_ROI_CONFIG_T);
    AXCL_SKEL_ROI_CONFIG_T stRoi = stParam.stRoi;
    stConfig.pstItems[nSize].pstrValue = (AX_VOID *)malloc(stConfig.pstItems[nSize].nValueSize);
    memcpy(stConfig.pstItems[nSize].pstrValue, &stRoi, stConfig.pstItems[nSize].nValueSize);
    nSize++;

    // push_strategy
    stConfig.pstItems[nSize].pstrType = (AX_CHAR *)"push_strategy";
    stConfig.pstItems[nSize].nValueSize = sizeof(AXCL_SKEL_PUSH_STRATEGY_T);
    AXCL_SKEL_PUSH_STRATEGY_T stPushStrategy = stParam.stPushStrategy;
    stConfig.pstItems[nSize].pstrValue = (AX_VOID *)malloc(stConfig.pstItems[nSize].nValueSize);
    memcpy(stConfig.pstItems[nSize].pstrValue, &stPushStrategy, stConfig.pstItems[nSize].nValueSize);
    nSize++;

    // crop_encoder
    stConfig.pstItems[nSize].pstrType = (AX_CHAR *)"crop_encoder";
    stConfig.pstItems[nSize].nValueSize = sizeof(AXCL_SKEL_CROP_ENCODER_THRESHOLD_CONFIG_T);
    AXCL_SKEL_CROP_ENCODER_THRESHOLD_CONFIG_T stCropEncoderThreshold = stParam.stCropEncoderThreshold;
    stConfig.pstItems[nSize].pstrValue = (AX_VOID *)malloc(stConfig.pstItems[nSize].nValueSize);
    memcpy(stConfig.pstItems[nSize].pstrValue, &stCropEncoderThreshold, stConfig.pstItems[nSize].nValueSize);
    nSize++;

    // resize_panorama_encoder_config
    stConfig.pstItems[nSize].pstrType = (AX_CHAR *)"resize_panorama_encoder_config";
    stConfig.pstItems[nSize].nValueSize = sizeof(AXCL_SKEL_RESIZE_CONFIG_T);
    AXCL_SKEL_RESIZE_CONFIG_T stPanoramaResizeConfig = stParam.stPanoramaResizeConfig;
    stConfig.pstItems[nSize].pstrValue = (AX_VOID *)malloc(stConfig.pstItems[nSize].nValueSize);
    memcpy(stConfig.pstItems[nSize].pstrValue, &stPanoramaResizeConfig, stConfig.pstItems[nSize].nValueSize);
    nSize++;

    // push_panorama
    stConfig.pstItems[nSize].pstrType = (AX_CHAR *)"push_panorama";
    stConfig.pstItems[nSize].nValueSize = sizeof(AXCL_SKEL_PUSH_PANORAMA_CONFIG_T);
    AXCL_SKEL_PUSH_PANORAMA_CONFIG_T stPushPanoramaConfig = stParam.stPushPanoramaConfig;
    stConfig.pstItems[nSize].pstrValue = (AX_VOID *)malloc(stConfig.pstItems[nSize].nValueSize);
    memcpy(stConfig.pstItems[nSize].pstrValue, &stPushPanoramaConfig, stConfig.pstItems[nSize].nValueSize);
    nSize++;

    // push_quality_body
    stConfig.pstItems[nSize].pstrType = (AX_CHAR *)"push_quality_body";
    stConfig.pstItems[nSize].nValueSize = sizeof(AXCL_SKEL_ATTR_FILTER_CONFIG_T);
    AXCL_SKEL_ATTR_FILTER_CONFIG_T stBodyAttrFilter = stParam.stAttrFliterMaps["body"];
    stConfig.pstItems[nSize].pstrValue = (AX_VOID *)malloc(stConfig.pstItems[nSize].nValueSize);
    memcpy(stConfig.pstItems[nSize].pstrValue, &stBodyAttrFilter, stConfig.pstItems[nSize].nValueSize);
    nSize++;

    // push_quality_vehicle
    stConfig.pstItems[nSize].pstrType = (AX_CHAR *)"push_quality_vehicle";
    stConfig.pstItems[nSize].nValueSize = sizeof(AXCL_SKEL_ATTR_FILTER_CONFIG_T);
    AXCL_SKEL_ATTR_FILTER_CONFIG_T stVehicleAttrFilter = stParam.stAttrFliterMaps["vehicle"];
    stConfig.pstItems[nSize].pstrValue = (AX_VOID *)malloc(stConfig.pstItems[nSize].nValueSize);
    memcpy(stConfig.pstItems[nSize].pstrValue, &stVehicleAttrFilter, stConfig.pstItems[nSize].nValueSize);
    nSize++;

    // push_quality_cycle
    stConfig.pstItems[nSize].pstrType = (AX_CHAR *)"push_quality_cycle";
    stConfig.pstItems[nSize].nValueSize = sizeof(AXCL_SKEL_ATTR_FILTER_CONFIG_T);
    AXCL_SKEL_ATTR_FILTER_CONFIG_T stCycleAttrFilter = stParam.stAttrFliterMaps["cycle"];
    stConfig.pstItems[nSize].pstrValue = (AX_VOID *)malloc(stConfig.pstItems[nSize].nValueSize);
    memcpy(stConfig.pstItems[nSize].pstrValue, &stCycleAttrFilter, stConfig.pstItems[nSize].nValueSize);
    nSize++;

    // push_quality_face
    stConfig.pstItems[nSize].pstrType = (AX_CHAR *)"push_quality_face";
    stConfig.pstItems[nSize].nValueSize = sizeof(AXCL_SKEL_ATTR_FILTER_CONFIG_T);
    AXCL_SKEL_ATTR_FILTER_CONFIG_T stFaceAttrFilter = stParam.stAttrFliterMaps["face"];
    stConfig.pstItems[nSize].pstrValue = (AX_VOID *)malloc(stConfig.pstItems[nSize].nValueSize);
    memcpy(stConfig.pstItems[nSize].pstrValue, &stFaceAttrFilter, stConfig.pstItems[nSize].nValueSize);
    nSize++;

    // push_quality_plate
    stConfig.pstItems[nSize].pstrType = (AX_CHAR *)"push_quality_plate";
    stConfig.pstItems[nSize].nValueSize = sizeof(AXCL_SKEL_ATTR_FILTER_CONFIG_T);
    AXCL_SKEL_ATTR_FILTER_CONFIG_T stPlateAttrFilter = stParam.stAttrFliterMaps["plate"];
    stConfig.pstItems[nSize].pstrValue = (AX_VOID *)malloc(stConfig.pstItems[nSize].nValueSize);
    memcpy(stConfig.pstItems[nSize].pstrValue, &stPlateAttrFilter, stConfig.pstItems[nSize].nValueSize);
    nSize++;

    // push_bind_enable
    stConfig.pstItems[nSize].pstrType = (AX_CHAR *)"push_bind_enable";
    stConfig.pstItems[nSize].nValueSize = sizeof(AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T);
    AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T stPushBindEnableThreshold;
    stPushBindEnableThreshold.fValue = (AX_F32)stParam.bPushBindEnable;
    stConfig.pstItems[nSize].pstrValue = (AX_VOID *)malloc(stConfig.pstItems[nSize].nValueSize);
    memcpy(stConfig.pstItems[nSize].pstrValue, &stPushBindEnableThreshold, stConfig.pstItems[nSize].nValueSize);
    nSize++;

    // track_disable
    stConfig.pstItems[nSize].pstrType = (AX_CHAR *)"track_disable";
    stConfig.pstItems[nSize].nValueSize = sizeof(AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T);
    AXCL_SKEL_COMMON_THRESHOLD_CONFIG_T stTrackDisableThreshold;
    stTrackDisableThreshold.fValue = (AX_F32)(!stParam.bTrackEnable);
    stConfig.pstItems[nSize].pstrValue = (AX_VOID *)malloc(stConfig.pstItems[nSize].nValueSize);
    memcpy(stConfig.pstItems[nSize].pstrValue, &stTrackDisableThreshold, stConfig.pstItems[nSize].nValueSize);
    nSize++;

    return AXCL_SKEL_SUCC;
}

#define SKEL_MGR_MEM_CONFIG 0

AX_VOID mgr_mem_callback(MEMMGR_ADDR pAddr, AX_VOID *pUserData, AX_VOID *pParam) {
    CSKELMgr *__this = (CSKELMgr *)pUserData;

    if (__this) {
        __this->MgrMemCallback(pAddr, pParam);
    }
    else {
        ALOGE("SKEL mgr instance lose, will memory leak!");
    }
}
}  // namespace

///
AX_S32 CSKELMgr::Init(const AXCL_SKEL_INIT_PARAM_T *pstParam) {
    CHECK_PTR(pstParam);

    if (m_bInited || m_pstInitInst) {
        ALOGE("SKEL already init");
        return AXCL_ERR_SKEL_INITED;
    }

    if (!pstParam->pStrModelDeploymentPath) {
        ALOGE("Model deployment path empty");

        return AXCL_ERR_SKEL_ILLEGAL_PARAM;
    }

    m_Maps.clear();

    SKEL_INIT_PARAM_T stInitParam;
    stInitParam.strModelDeploymentPath = pstParam->pStrModelDeploymentPath;
    m_nDeviceId = pstParam->nDeviceId;

    AX_S32 nRet = GetSkelModels(stInitParam);

    if (nRet != AXCL_SKEL_SUCC) {
        return nRet;
    }

    if (stInitParam.strHvcpModel.size() > 0
        || stInitParam.strFaceModel.size() > 0) {
        m_pstInitInst = new (std::nothrow) CSKELInit(stInitParam);
    }

    if (!m_pstInitInst) {
        ALOGE("SKEL create init instance fail");
        nRet = AXCL_ERR_SKEL_NOMEM;

        goto EXIT;
    }

    nRet = m_pstInitInst->Init();

    if (AXCL_SKEL_SUCC == nRet) {
        m_bInited = AX_TRUE;
    }

EXIT:
    if (AXCL_SKEL_SUCC != nRet) {
        if (m_pstInitInst) {
            delete m_pstInitInst;
            m_pstInitInst = nullptr;
        }
    }

    return nRet;
}

AX_S32 CSKELMgr::DeInit(AX_VOID) {
    if (m_bInited) {
        /* clear and destory all chns */
        if (m_Maps.size() > 0) {
            for (auto &m : m_Maps) {
                CSKEL *pObj = m.second;
                if (pObj) {
                    // stop stage
                    pObj->Stop();

                    pObj->Destroy();

                    delete pObj;
                }

                delete (AXCL_SKEL_CTX_T *)m.first;
            }
            m_Maps.clear();
        }

        if (m_pstInitInst) {
            m_pstInitInst->DeInit();

            delete m_pstInitInst;
            m_pstInitInst = nullptr;
        }

        MEMMGROBJ->DeInit();

        m_bInited = AX_FALSE;
    }
    else {
        return AXCL_ERR_SKEL_NOT_INIT;
    }

    return AXCL_SKEL_SUCC;
}

AX_VOID CSKELMgr::MgrMemCallback(MEMMGR_ADDR pAddr, AX_VOID *pParam) {
    MEMMGR_PARAM_T nType = (MEMMGR_PARAM_T)pParam;

    switch (nType) {
        case SKEL_MGR_MEM_CONFIG: {
            AXCL_SKEL_CONFIG_T *p = (AXCL_SKEL_CONFIG_T *)pAddr;

            if (p) {
                if (p->pstItems) {
                    for (size_t i = 0; i < p->nSize; i++) {
                        if (p->pstItems[i].pstrValue) {
                            free(p->pstItems[i].pstrValue);
                        }
                    }
                    delete[] p->pstItems;
                }

                delete p;
            }
        } break;

        default:
            ALOGE("Release invalid memory type: {}", nType);
            break;
    }
}

AX_S32 CSKELMgr::GetCapability(const AXCL_SKEL_CAPABILITY_T **ppstCapability) {
    CHECK_SKEL_INITED();

    CHECK_PTR(ppstCapability);
    CHECK_PTR(m_pstInitInst);

    return m_pstInitInst->GetCapability(ppstCapability);
}

AX_S32 CSKELMgr::GetVersion(const AXCL_SKEL_VERSION_INFO_T **ppstVersion) {
    CHECK_SKEL_INITED();

    CHECK_PTR(ppstVersion);
    CHECK_PTR(m_pstInitInst);

    return m_pstInitInst->GetVersion(ppstVersion);
}

AX_S32 CSKELMgr::Create(const AXCL_SKEL_HANDLE_PARAM_T *pstParam, AXCL_SKEL_HANDLE *pHandle) {
    CHECK_SKEL_INITED();
    CHECK_PTR(pstParam);
    CHECK_PTR(pHandle);
    CHECK_PTR(m_pstInitInst);

    if (!m_pstInitInst->GetPPLStatus(pstParam->ePPL)) {
        ALOGE("SKEL ePPL:{} is not support", static_cast<int>(pstParam->ePPL));
        return AXCL_ERR_SKEL_NOT_SUPPORT;
    }

    AX_S32 nRet = CheckNpuTypeConfig(pstParam->nNpuType);

    if (nRet != AXCL_SKEL_SUCC) {
        return nRet;
    }

    AXCL_SKEL_CTX_T *pCtx = new AXCL_SKEL_CTX_T;
    if (!pCtx) {
        ALOGE("SKEL create ctx fail");
        return AXCL_ERR_SKEL_NOMEM;
    }

    pCtx->nDeviceId = m_nDeviceId;
    pCtx->stInitParam = m_pstInitInst->GetInitParam();
    pCtx->pHandle = (AXCL_SKEL_HANDLE)pCtx;

    pCtx->stParam.ePPL = pstParam->ePPL;
    pCtx->stParam.nNpuType = pstParam->nNpuType;
    pCtx->stParam.nFrameDepth = pstParam->nFrameDepth;
    pCtx->stParam.nFrameCacheDepth = pstParam->nFrameCacheDepth;
    pCtx->stParam.nIoDepth = pstParam->nIoDepth;
    pCtx->stParam.nWidth = pstParam->nWidth;
    pCtx->stParam.nHeight = pstParam->nHeight;
    ParseConfigParam(pstParam->stConfig, pCtx->stParam);

    memset(&pCtx->stCallback, 0x00, sizeof(pCtx->stCallback));

    if (pCtx->stParam.nFrameDepth == 0) {
        pCtx->stParam.nFrameDepth = 1;
    }

    CSKEL *pObj = nullptr;

    if (pstParam->ePPL == AXCL_SKEL_PPL_HVCP && pCtx->stInitParam.strHvcpModel.size() > 0) {
        pObj = new (std::nothrow) CHVCP(*pCtx);
    } else if (pstParam->ePPL == AXCL_SKEL_PPL_FACE && pCtx->stInitParam.strFaceModel.size() > 0) {
        pObj = new (std::nothrow) CFACE(*pCtx);
    }

    if (!pObj) {
        ALOGE("SKEL create instance fail");
        delete pCtx;
        return AXCL_ERR_SKEL_NOMEM;
    }

    AX_S32 ret = pObj->Create();
    if (AXCL_SKEL_SUCC != ret) {
        delete pCtx;
        delete pObj;

        return ret;
    }

    *pHandle = (AXCL_SKEL_HANDLE)pCtx;

    m_Maps[*pHandle] = pObj;

    // start stage for skel
    if ((pstParam->ePPL == AXCL_SKEL_PPL_HVCP && pCtx->stInitParam.strHvcpModel.size() > 0)
        || (pstParam->ePPL == AXCL_SKEL_PPL_FACE && pCtx->stInitParam.strFaceModel.size() > 0)) {
        pObj->SetCapacity(pstParam->nFrameDepth);
        pObj->Start();
    }

    return AXCL_SKEL_SUCC;
}

AX_S32 CSKELMgr::Destroy(AXCL_SKEL_HANDLE handle) {
    CHECK_SKEL_INITED();

    if (0 == m_Maps.size()) {
        ALOGE("SKEL handle {} not exist", handle);
        return AXCL_ERR_SKEL_UNEXIST;
    }

    auto it = m_Maps.find(handle);
    if (m_Maps.end() == it) {
        ALOGE("SKEL handle {} not found", handle);
        return AXCL_ERR_SKEL_UNEXIST;
    }

    CSKEL *pObj = it->second;
    if (!pObj) {
        ALOGE("SKEL handle {} instance is nil", handle);
        delete (AXCL_SKEL_CTX_T *)handle;

        return AXCL_ERR_SKEL_NULL_PTR;
    }

    // stop stage
    pObj->Stop();

    pObj->Destroy();

    delete pObj;

    delete (AXCL_SKEL_CTX_T *)handle;

    m_Maps.erase(it);

    return AXCL_SKEL_SUCC;
}

AX_S32 CSKELMgr::SendFrame(AXCL_SKEL_HANDLE handle, const AXCL_SKEL_FRAME_T *pstFrame, AX_S32 nTimeout) {
    CHECK_SKEL_INITED();

    CHECK_PTR(pstFrame);

    CSKEL *pObj = Lookup(handle);

    CHECK_PTR(pObj);

    return pObj->SendFrame(pstFrame, nTimeout);
}

AX_S32 CSKELMgr::RegisterResultCallback(AXCL_SKEL_HANDLE handle, AXCL_SKEL_RESULT_CALLBACK_FUNC callback, AX_VOID *pUserData) {
    CHECK_SKEL_INITED();

    CSKEL *pObj = Lookup(handle);

    CHECK_PTR(pObj);

    return pObj->RegisterResultCallback(callback, pUserData);
}

AX_S32 CSKELMgr::GetResult(AXCL_SKEL_HANDLE handle, AXCL_SKEL_RESULT_T **ppstResult, AX_S32 nTimeout) {
    CHECK_SKEL_INITED();

    CHECK_PTR(ppstResult);

    CSKEL *pObj = Lookup(handle);

    CHECK_PTR(pObj);

    return pObj->GetResult(ppstResult, nTimeout);
}

AX_S32 CSKELMgr::GetConfig(AXCL_SKEL_HANDLE handle, const AXCL_SKEL_CONFIG_T **ppstConfig) {
    CHECK_SKEL_INITED();

    CHECK_PTR(ppstConfig);

    CSKEL *pObj = Lookup(handle);

    CHECK_PTR(pObj);

    AXCL_SKEL_CONFIG_T *pstConfig = new AXCL_SKEL_CONFIG_T;

    if (!pstConfig) {
        ALOGE("SKEL alloc config fail");
        return AXCL_ERR_SKEL_NOMEM;
    }

    memset(pstConfig, 0x00, sizeof(AXCL_SKEL_CONFIG_T));
    AXCL_SKEL_PARAM_T stParam = pObj->GetConfig();

    AX_U32 nRet = GenerateConfigParam(stParam, *pstConfig);

    if (AXCL_SKEL_SUCC != nRet) {
        delete pstConfig;
        return nRet;
    }

    *ppstConfig = pstConfig;

    MEMMGROBJ->Add(pstConfig, this, (AX_VOID *)SKEL_MGR_MEM_CONFIG, mgr_mem_callback);

    return AXCL_SKEL_SUCC;
}

AX_S32 CSKELMgr::SetConfig(AXCL_SKEL_HANDLE handle, const AXCL_SKEL_CONFIG_T *pstConfig) {
    CHECK_SKEL_INITED();

    CHECK_PTR(pstConfig);

    CSKEL *pObj = Lookup(handle);

    CHECK_PTR(pObj);

    AXCL_SKEL_PARAM_T stParam = pObj->GetConfig();

    ParseConfigParam(*pstConfig, stParam);

    return pObj->SetConfig(stParam);
}

AX_S32 CSKELMgr::Release(AX_VOID *p) {
    CHECK_PTR(p);

    if (MEMMGROBJ->Rel(p) != 0) {
        return AXCL_ERR_SKEL_UNEXIST;
    }

    return AXCL_SKEL_SUCC;
}

CSKEL *CSKELMgr::Lookup(AXCL_SKEL_HANDLE handle) {
    if (m_Maps.size() > 0) {
        auto it = m_Maps.find(handle);
        if (m_Maps.end() != it) {
            return it->second;
        }
    }

    return nullptr;
}
