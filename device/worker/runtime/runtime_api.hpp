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

#define RUNTIME_API_PARAM(_MODULE_) \
                axcl::pkg::device::_MODULE_##_response *response, \
                axcl::pkg::device::_MODULE_##_request *request, \
                const void *data, \
                const size_t data_size

#define RUNTIME_API_DECL(_API_, _PARAM_) \
    int32_t _API_(_PARAM_)

#define RUNTIME_API_IMPL(_MODULE_, _API_, _PARAM_) \
    int32_t _MODULE_::_API_(_PARAM_)

#define RUNTIME_API_APPEND(_MODULE_, _API_, _PARAM_) \
    { \
        static_cast<int32_t>(axcl::pkg::runtime::_MODULE_::API::_API_), [this](_PARAM_) -> int32_t { \
                        /* LOG_MM_D(#_MODULE_, "{} ...", #_API_); */ \
                        return this->_API_(response, request, data, data_size); } \
    }

namespace axcl::worker {

class RuntimeApiException : public std::runtime_error {
private:
    int32_t m_error_code;
    std::string m_module;
    int32_t m_api;

public:
    RuntimeApiException(int32_t error_code, const std::string& module, int32_t api)
        : std::runtime_error("RuntimeApi err")
        , m_error_code(error_code)
        , m_module(module)
        , m_api(api) {}

    int32_t getErrorCode() const { return m_error_code; }
    const std::string& getModule() const { return m_module; }
    int32_t getApi() const { return m_api; }
};

class dispatcher;

template <typename RSP, typename REQ>
using runtime_api_func = std::function<int32_t(RSP *response,
                                                    REQ *request,
                                                    const void *data,
                                                    const size_t data_size)>;

template <typename API, typename RSP, typename REQ>
class runtime_api {
public:
    runtime_api(const std::string& module) : m_module(module) {};
    virtual ~runtime_api() = default;

public:
    void append(std::unordered_map<int32_t, runtime_api_func<RSP, REQ>> api) {
        m_api_list = api;
    }

    void run(idispatcher *dispatcher, const void *data, const size_t& size) {
        int32_t ret = -1;
        void *msg = nullptr;
        uint32_t msg_len = 0;
        API api = (API)-1;

        RSP decoder;
        REQ encoder;

        if (decoder.decode(data, size)) {
            api = decoder.get_api();
        }

        runtime_api_func<RSP, REQ> func = find(static_cast<int32_t>(api));

        // run api func
        if (func) {
            ret = func(&decoder, &encoder, data, size);
        } else {
            LOG_MM_E(m_module, "not find {} api #{}", m_module, (int32_t)api);
        }

        // if ack not required, return when correct or throw expception.
        if (handle_ack_err(decoder, ret)) {
            return;
        }

        encoder.set_api(api);
        encoder.set_status(ret);

        // encode
        {
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
    virtual bool handle_ack_err(const RSP& decoder, int32_t ret) {
        return false;
    }

    runtime_api_func<RSP, REQ> find(int32_t api) {
        auto it = m_api_list.find(api);

        if (it != m_api_list.end()) {
            return it->second;
        }

        return nullptr;
    }

private:
    std::string m_module;
    std::unordered_map<int32_t, runtime_api_func<RSP, REQ>> m_api_list;
};
}
