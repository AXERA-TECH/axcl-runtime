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

#include <cassert>
#include <cstdint>
#include <functional>
#include <type_traits>
#include "package/include/native/api.hpp"

// #define __PRINT_ARG_TYPE__

#ifndef __PRINT_ARG_TYPE__
#define PRINT_ARG_TYPE(...) (void)0
#else
#include <stdio.h>
#define PRINT_ARG_TYPE printf
#endif

namespace axcl::pkg::native {

using set_arg_proxy_func = const std::function<bool(const void *, size_t)>;

class args_helper {
public:
    args_helper(const set_arg_proxy_func &set_arg) : m_set_arg(set_arg) {
    }

    /* basic enum pass by reference */
    template <typename T>
    typename std::enable_if<std::is_enum<T>::value && !IS_NATIVE_API_ENUM_TYPE(T), bool>::type set_arg(const T &value) {
        PRINT_ARG_TYPE("set_arg: enum by value, sizeof = %ld\n", sizeof(T));
        return m_set_arg(&value, sizeof(T));
    }

    /* basic such as int, float, double pass by value or pass by reference */
    template <typename T>
    typename std::enable_if<std::is_fundamental<T>::value && !std::is_pointer<T>::value, bool>::type set_arg(const T &value) {
        PRINT_ARG_TYPE("set_arg: fundamental by value, sizeof = %ld\n", sizeof(T));
        return m_set_arg(&value, sizeof(T));
    }

    /**
     * structure pass by pointer
     * !!! NOTE: cannot support pointer in structure, such as
     *   struct T {
     *       int a;
     *       char *p;
     *   };
     */
    template <typename T>
    typename std::enable_if<std::is_pointer<T>::value &&
                                std::is_class<typename std::remove_pointer<typename std::remove_const<T>::type>::type>::value,
                            bool>::type
    set_arg(const T value) {
        using U = typename std::remove_pointer<typename std::remove_const<T>::type>::type;
        static_assert(std::is_trivially_copyable<U>::value, "type must be trivially copyable");
        PRINT_ARG_TYPE("set_arg: structure by pointer, sizeof = %ld\n", sizeof(U));
        return m_set_arg(value, sizeof(U));
    }

    /**
     * structure pass by value or by reference
     * !!! NOTE: cannot support pointer in structure, such as
     *   struct T {
     *       int a;
     *       char *p;
     *   };
     */
    template <typename T>
    typename std::enable_if<std::is_class<T>::value && !std::is_pointer<T>::value && !std::is_same<T, axcl::pkg::uint8_array>::value,
                            bool>::type
    set_arg(const T &value) {
        static_assert(std::is_trivially_copyable<T>::value, "type must be trivially copyable");
        PRINT_ARG_TYPE("set_arg: structure by value, sizeof = %ld\n", sizeof(T));
        return m_set_arg(&value, sizeof(T));
    }

    /* union pass by value except axcl::pkg::native::API */
    template <typename T>
    typename std::enable_if<std::is_union<T>::value && !std::is_pointer<T>::value && !std::is_same<T, axcl::pkg::native::API>::value,
                            bool>::type
    set_arg(const T &value) {
        PRINT_ARG_TYPE("set_arg: union by value, sizeof = %d\n", sizeof(T));
        return m_set_arg(&value, sizeof(T));
    }

    /* uint8 array */
    template <typename T>
    typename std::enable_if<std::is_same<T, axcl::pkg::uint8_array>::value && !std::is_pointer<T>::value, bool>::type set_arg(
        const T &value) {
        PRINT_ARG_TYPE("set_arg: uint8_t array by value, sizeof = %d\n", value.size);
        return m_set_arg(value.data, value.size);
    }

    template <typename T, typename... Args>
    bool set_args(T first, Args... args) {
        if (!set_arg(first)) {
            return false;
        }

        return set_args(args...);
    }

    bool set_args() {
        return true;
    }

private:
    set_arg_proxy_func m_set_arg;
};

}  // namespace axcl::pkg::native