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

#include "axcl_venc_type.h"
#include "package/device/native/native_request.hpp"
#include "package/include/native/api.hpp"

namespace axcl::pkg::device {

class venc_request : public native_request {
public:
    explicit venc_request();
    virtual ~venc_request();

    venc_request(venc_request &&other) noexcept;
    venc_request &operator=(venc_request &&other) noexcept;
    venc_request(const venc_request &) = delete;
    venc_request &operator=(const venc_request &) = delete;

    template <typename... Args>
    [[nodiscard]] bool pack(axcl::pkg::native::VENC_API api, Args... args) {
        if (!set_api(api)) {
            return false;
        }

        return m_args.set_args(args...);
    }

    /**
     * AX_VENC_GOP_SVC_T_T ==>  | u32GopSize | count  | s8SvcTCfg[0] | s8SvcTCfg[1] ... |
     * example:
     *  AX_VENC_GOP_SVC_T_T.u32GopSize = 4
     *  AX_VENC_GOP_SVC_T_T.s8SvcTCfg = {
     *      "hello",
     *      "world"
     *      NULL,
     *  };
     *  ==>
     *  04 00 00 00 02 00 00 00 'h' 'e' 'l' 'l' 'o' '\0' 'w' 'o' 'r' 'l' 'd' '\0'
     */
    bool serialize_gop_svc_t(const AX_VENC_GOP_SVC_T_T &svc, uint8_t *&buf, uint32_t &size);

protected:
    bool add_data(const void *data, size_t size) override;
    bool set_api(axcl::pkg::native::VENC_API api);

private:
    struct impl;
    impl *m_impl;
};

}  // namespace axcl::pkg::device