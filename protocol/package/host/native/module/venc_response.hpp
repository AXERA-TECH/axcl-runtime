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

#include <vector>
#include "axcl_venc_type.h"
#include "package/host/native/native_response.hpp"

namespace axcl::pkg::host {

class venc_response : public native_response {
public:
    explicit venc_response();
    virtual ~venc_response();

    venc_response(venc_response&& other) noexcept;
    venc_response& operator=(venc_response&& other) noexcept;
    venc_response(const venc_response&) = delete;
    venc_response& operator=(const venc_response&) = delete;

    [[nodiscard]] bool decode(const void* message, uint32_t size) const override;

    axcl::pkg::native::VENC_API get_api() const;

    template <typename... Args>
    [[nodiscard]] bool unpack(axcl::pkg::native::VENC_API api, Args... args) {
        axcl::pkg::native::API native_api;
        native_api.venc = api;
        return native_response::unpack(native_api, args...);
    }

    /**
     * AX_VENC_GOP_SVC_T_T
     *  static const  char *stSvcTCfg[] = {
            "Frame1:  P      1      0       0.4624        2        1           -1          1",
            "Frame2:  P      2      0       0.4624        1        1           -2          1",
            "Frame3:  P      3      0       0.4624        2        2           -1 -3       1 0",
            "Frame4:  P      4      0       0.4624        0        1           -4          1",
            NULL,
        };

        std::vector<uint8_t>& memory layout:
        |    char * array      | gop | num |                    stSvcTCfg buffer                     |
        |    char *p[5]        |     |  4  | Frame1:  P... Frame2:  P... Frame3:  P... Frame4:  P... |
        |                                    ^
        |    p[0] -------------------------- |             ^
        |    p[1] ---------------------------------------- |             ^
        |    p[2] ------------------------------------------------------ |             ^
        |    p[3] -------------------------------------------------------------------- |
        |    p[4] = nullptr
     */
    bool deserialize_gop_svc_t(const uint8_t* buf, uint32_t size, AX_VENC_GOP_SVC_T_T& svc, std::vector<uint8_t>& mem);

protected:
    void* get_payload() const override;
    bool check_api(axcl::pkg::native::API api) override;
    size_t get_data_size(int index) override;
    bool get_data(int index, void* data, size_t size) override;
    const std::string& get_data(int index) override;

private:
    struct impl;
    impl* m_impl;
};

}  // namespace axcl::pkg::host
