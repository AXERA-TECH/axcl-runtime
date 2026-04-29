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
#include <functional>
#include <unordered_map>
#include "idispatcher.hpp"
#include "axcl_package.hpp"
#include "axcl_logger.hpp"

#define NATIVE_API_PARAM(_MODULE_) \
                axcl::pkg::device::_MODULE_##_response *response, \
                axcl::pkg::device::_MODULE_##_request *request

#define NATIVE_API_DECL(_API_, _PARAM_) \
    bool _API_(_PARAM_)

#define NATIVE_API_IMPL(_MODULE_, _API_, _PARAM_) \
    bool _MODULE_::_API_(_PARAM_)

#define NATIVE_API_APPEND(_MODULE_, _API_, _PARAM_) \
    { \
        static_cast<int32_t>(axcl::pkg::native::_MODULE_##_API::_API_), [this](_PARAM_) -> bool { \
                        /* LOG_MM_D(#_MODULE_, "{} ...", #_API_); */ \
                        return this->_API_(response, request); } \
    }

namespace axcl::worker {
class dispatcher;

template <typename RSP, typename REQ>
using native_api_func = std::function<bool(RSP *response,
                                                    REQ *request)>;

template <typename API, typename RSP, typename REQ>
class native_api {
public:
    native_api(const std::string& module) : m_module(module) {};
    virtual ~native_api() = default;

public:
    void append(std::unordered_map<int32_t, native_api_func<RSP, REQ>> api) {
        m_api_list = api;
    }

    void run(idispatcher *dispatcher, const void *data, const size_t& size) {
        bool ret = false;
        void *msg = nullptr;
        uint32_t msg_len = 0;
        API api = (API)-1;

        RSP decoder;
        REQ encoder;

        if (decoder.decode(data, size)) {
            api = decoder.get_api();
        }

        native_api_func<RSP, REQ> func = find(static_cast<int32_t>(api));

        // run api func
        if (func) {
            ret = func(&decoder, &encoder);
        } else {
            LOG_MM_E(m_module, "not find {} api #{}", m_module, (int32_t)api);
        }

        if (!ret) {
            if (encoder.pack(api, -1)) {
                ret = true;
            }
        }

        if (ret) {
            const auto hd = static_cast<const axcl::pkg::head *>(data);
            msg = encoder.encode(*hd, msg_len);
        }

        if (msg && msg_len) {
            dispatcher->ack(msg, msg_len, -1, -1);
        } else {
            LOG_MM_E(m_module, "run {} api #{} failed, {:#x}", m_module, (int32_t)api, (uint32_t)ret);
        }
    }

protected:
    native_api_func<RSP, REQ> find(int32_t api) {
        auto it = m_api_list.find(api);

        if (it != m_api_list.end()) {
            return it->second;
        }

        return nullptr;
    }

private:
    std::string m_module;
    std::unordered_map<int32_t, native_api_func<RSP, REQ>> m_api_list;
};

template <typename T>
static inline std::unique_ptr<T*[]> unpackArray2PArray(axcl::pkg::uint8_array &array) {
    T *pasts = reinterpret_cast<T*>(array.data);
    uint32_t num = array.size / sizeof(T);
    auto spasts = std::make_unique<T*[]>(num);

    if (!spasts) {
        return nullptr;
    } else {
        for (uint32_t i = 0; i < num; ++i) {
            spasts[i] = &pasts[i];
        }
    }

    return spasts;
}
}
