/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include "MemMgr.hpp"
#include "skelInit.hpp"

namespace {
#define SKEL_INIT_MEM_VERSION 0
#define SKEL_INIT_MEM_CAPABILITY 1
 
 AX_VOID init_mem_callback(MEMMGR_ADDR pAddr, AX_VOID *pUserData, AX_VOID *pParam) {
    CSKELInit *__this = (CSKELInit *)pUserData;

    if (__this) {
        __this->InitMemCallback(pAddr, pParam);
    }
    else {
        ALOGE("SKEL instance lose, will memory leak!");
    }
 }
} // namespace

CSKELInit::CSKELInit(SKEL_INIT_PARAM_T &stInitParam) {
    m_stInitParam = stInitParam;
}

AX_S32 CSKELInit::Init(AX_VOID) {
    if (m_bInited) {
        return AXCL_ERR_SKEL_INITED;
    }

    if (m_stInitParam.strModelDeploymentPath.size() == 0) {
        ALOGE("SKEL model deployment path empty");

        return AXCL_ERR_SKEL_ILLEGAL_PARAM;
    }

    if (m_stInitParam.strHvcpModel.size() > 0) {
        ALOGN("SKEL Hvcp model name: {}", m_stInitParam.strHvcpModel.c_str());
        m_bInited = AX_TRUE;
    }
    else {
        ALOGN("SKEL Hvcp model empty");
    }

    if (m_stInitParam.strFaceModel.size() > 0) {
        ALOGN("SKEL face model name: {}", m_stInitParam.strFaceModel.c_str());
        m_bInited = AX_TRUE;
    }
    else {
        ALOGN("SKEL face model empty");
    }

    if (m_stInitParam.strFaceAttrModel.size() > 0) {
        ALOGN("SKEL face attribute model name: {}", m_stInitParam.strFaceAttrModel.c_str());
        if (m_stInitParam.strFaceModel.size() > 0) {
            m_bInited = AX_TRUE;
        }
    }
    else {
        ALOGN("SKEL face attribute model empty");
    }

    if (!m_bInited) {
        return AXCL_ERR_SKEL_ILLEGAL_PARAM;
    }

    return AXCL_SKEL_SUCC;
}

AX_S32 CSKELInit::DeInit(AX_VOID) {
    CHECK_SKEL_INITED();

    m_bInited = AX_FALSE;

    m_stInitParam.release();

    return AXCL_SKEL_SUCC;
}

AX_S32 CSKELInit::GetCapability(const AXCL_SKEL_CAPABILITY_T **ppstCapability) {
    CHECK_SKEL_INITED();

    CHECK_PTR(ppstCapability);

    AX_S32 nRet = AXCL_SKEL_SUCC;
    AXCL_SKEL_CAPABILITY_T *pstCapability = new AXCL_SKEL_CAPABILITY_T;

    if (!pstCapability) {
        ALOGE("SKEL alloc capability fail");
        return AXCL_ERR_SKEL_NOMEM;
    }

    memset(pstCapability, 0x00, sizeof(AXCL_SKEL_CAPABILITY_T));

    pstCapability->nPPLConfigSize = ((m_stInitParam.strHvcpModelName.size() > 0) ? 1 : 0)
                                    + ((m_stInitParam.strFaceModelName.size() > 0) ? 1 : 0);

    if (pstCapability->nPPLConfigSize > 0) {
        pstCapability->pstPPLConfig = new AXCL_SKEL_PPL_CONFIG_T[pstCapability->nPPLConfigSize];

        if (!pstCapability->pstPPLConfig) {
            ALOGE("SKEL alloc PPL config fail");
            delete pstCapability;
            return AXCL_ERR_SKEL_NOMEM;
        }

        memset(pstCapability->pstPPLConfig, 0x00, sizeof(AXCL_SKEL_PPL_CONFIG_T) * pstCapability->nPPLConfigSize);

        auto GeneratePPLConfig = [&](std::string strModelName, AXCL_SKEL_PPL_E ePPL, const AX_CHAR *pstrKey,
                                     AXCL_SKEL_PPL_CONFIG_T *pstPPLConfig, AX_U32 &nIndex, AX_U32 nSize) -> AX_U32 {
            if (strModelName.size() > 0 && nIndex < nSize) {
                pstPPLConfig[nIndex].ePPL = ePPL;

                int strFileNameLen = strModelName.size();
                AX_CHAR *pFileName = strdup(strModelName.c_str());

                if (!pFileName) {
                    ALOGE("SKEL alloc file name fail");
                    return AXCL_ERR_SKEL_NOMEM;
                }

                int dotExtPos = 0;
                int VPos = 0;
                // IN: pFileName: ax_ax620u_person_algo_model_V1.0.3.joint
                for (int i = strFileNameLen - 1; i >= 0; i --) {
                    if (dotExtPos == 0 && pFileName[i] == '.') {
                        dotExtPos = i;
                        pFileName[i] = '\0';
                    }
                    else if (dotExtPos > 0 && VPos == 0 && pFileName[i] == 'V') {
                        VPos = i;

                        if (i > 0) {
                            pFileName[i - 1] = ':';
                        }
                    }
                }
                //OUT: pFileName: ax_ax620u_person_algo_model:V1.0.3

                AX_U32 nLen = strModelName.size() + 1 + strlen(pstrKey);
                pstPPLConfig[nIndex].pstrPPLConfigKey = new AX_CHAR[nLen + 1];

                if (!pstPPLConfig[nIndex].pstrPPLConfigKey) {
                    ALOGE("SKEL alloc PPL config key fail");
                    free(pFileName);
                    return AXCL_ERR_SKEL_NOMEM;
                }

                memset(pstPPLConfig[nIndex].pstrPPLConfigKey, 0x00, nLen + 1);
                strncat(pstPPLConfig[nIndex].pstrPPLConfigKey, pFileName, nLen + 1); //FIXME.
                strncat(pstPPLConfig[nIndex].pstrPPLConfigKey, ":", nLen + 1); //FIXME.
                strncat(pstPPLConfig[nIndex].pstrPPLConfigKey, pstrKey, nLen + 1); //FIXME.

                free(pFileName);

                nIndex++;
            }

            return AXCL_SKEL_SUCC;
        };

        AX_U32 nPPLConfigIndex = 0;

        nRet = GeneratePPLConfig(m_stInitParam.strHvcpModelName, AXCL_SKEL_PPL_HVCP, SKEL_HVCP_MODEL_PPL_KEY_STR, pstCapability->pstPPLConfig,
                                 nPPLConfigIndex, pstCapability->nPPLConfigSize);

        if (nRet != AXCL_SKEL_SUCC) {
            goto EXIT;
        }

        nRet = GeneratePPLConfig(m_stInitParam.strFaceModelName, AXCL_SKEL_PPL_FACE, SKEL_FACE_MODEL_PPL_KEY_STR, pstCapability->pstPPLConfig,
                                 nPPLConfigIndex, pstCapability->nPPLConfigSize);

        if (nRet != AXCL_SKEL_SUCC) {
            goto EXIT;
        }
    }

EXIT:
    if (nRet != AXCL_SKEL_SUCC) {
        InitMemCallback(pstCapability, (AX_VOID *)SKEL_INIT_MEM_CAPABILITY);
        return nRet;
    }

    *ppstCapability = pstCapability;

    MEMMGROBJ->Add(pstCapability, this, (AX_VOID *)SKEL_INIT_MEM_CAPABILITY, init_mem_callback);

    return AXCL_SKEL_SUCC;
}

AX_S32 CSKELInit::GetVersion(const AXCL_SKEL_VERSION_INFO_T **ppstVersion) {
    CHECK_SKEL_INITED();

    CHECK_PTR(ppstVersion);

    AXCL_SKEL_VERSION_INFO_T *pstVersion = new AXCL_SKEL_VERSION_INFO_T;

    if (!pstVersion) {
        ALOGE("SKEL alloc version fail");
        return AXCL_ERR_SKEL_NOMEM;
    }

    memset(pstVersion, 0x00, sizeof(AXCL_SKEL_VERSION_INFO_T));

    AX_U32 nLen = strlen(AXCL_SKEL_SDK_VERSION);
    pstVersion->pstrVersion = new AX_CHAR[nLen + 1];

    if (!pstVersion->pstrVersion) {
        ALOGE("SKEL alloc version string fail");
        delete pstVersion;
        return AXCL_ERR_SKEL_NOMEM;
    }
    memset(pstVersion->pstrVersion, 0x00, nLen + 1);
    strncat(pstVersion->pstrVersion, AXCL_SKEL_SDK_VERSION, nLen + 1); //FIXME.

    *ppstVersion = pstVersion;

    MEMMGROBJ->Add(pstVersion, this, (AX_VOID *)SKEL_INIT_MEM_VERSION, init_mem_callback);

    return AXCL_SKEL_SUCC;
}

AX_BOOL CSKELInit::GetPPLStatus(AXCL_SKEL_PPL_E ePPL) {
    if (m_bInited) {
        if (AXCL_SKEL_PPL_HVCP == ePPL
            && m_stInitParam.strHvcpModel.size() > 0) {
            return AX_TRUE;
        }
        else if (AXCL_SKEL_PPL_FACE == ePPL
            && m_stInitParam.strFaceModel.size() > 0) {
            return AX_TRUE;
        }
    }

    return AX_FALSE;
}

SKEL_INIT_PARAM_T CSKELInit::GetInitParam(AX_VOID) const {
    return m_stInitParam;
}

AX_VOID CSKELInit::InitMemCallback(MEMMGR_ADDR pAddr, AX_VOID *pParam) {
    MEMMGR_PARAM_T nType = (MEMMGR_PARAM_T)pParam;

    switch (nType) {
     case SKEL_INIT_MEM_VERSION: {
             AXCL_SKEL_VERSION_INFO_T *p = (AXCL_SKEL_VERSION_INFO_T *)pAddr;
 
             if (p) {
                 if (p->pstrVersion) {
                     delete[] p->pstrVersion;
                 }
 
                 if (p->pstMetaInfo) {
                     for (size_t i = 0; i < p->nMetaInfoSize; i++) {
                         if (p->pstMetaInfo[i].pstrType) {
                             delete[] p->pstMetaInfo[i].pstrType;
                         }
                         if (p->pstMetaInfo[i].pstrValue) {
                             delete[] p->pstMetaInfo[i].pstrValue;
                         }
                     }
                     delete[] p->pstMetaInfo;
                 }
 
                 delete p;
             }
         }
         break;
 
     case SKEL_INIT_MEM_CAPABILITY: {
             AXCL_SKEL_CAPABILITY_T *p = (AXCL_SKEL_CAPABILITY_T *)pAddr;
 
             if (p) {
                 if (p->pstPPLConfig) {
                     for (size_t i = 0; i < p->nPPLConfigSize; i++) {
                         if (p->pstPPLConfig[i].pstrPPLConfigKey) {
                             delete[] p->pstPPLConfig[i].pstrPPLConfigKey;
                         }
                     }
                     delete[] p->pstPPLConfig;
                 }
 
                 if (p->pstMetaInfo) {
                     for (size_t i = 0; i < p->nMetaInfoSize; i++) {
                         if (p->pstMetaInfo[i].pstrType) {
                             delete[] p->pstMetaInfo[i].pstrType;
                         }
                         if (p->pstMetaInfo[i].pstrValue) {
                             delete[] p->pstMetaInfo[i].pstrValue;
                         }
                     }
                     delete[] p->pstMetaInfo;
                 }
 
                 delete p;
             }
         }
         break;
 
     default:
         ALOGE("SKEL Release invalid memory type: {}", nType);
         break;
     }
}
