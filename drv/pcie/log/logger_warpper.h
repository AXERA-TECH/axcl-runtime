/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/
#ifndef __AXCL_PCIE_LOG__
#define __AXCL_PCIE_LOG__

#ifdef __cplusplus
extern "C" {
#endif

void axcl_pcie_debug_log(const char *fmt, ...);
void axcl_pcie_info_log(const char *fmt, ...);
void axcl_pcie_err_log(const char *fmt, ...);
void axcl_pcie_warn_log(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* __AXCL_PCIE_LOG__*/
