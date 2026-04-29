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

#include <cstring>
#include <cassert>
#include <type_traits>

template <typename T>
typename std::enable_if<
    std::is_pointer<T>::value && std::is_class<typename std::remove_pointer<typename std::remove_const<T>::type>::type>::value, bool>::type
struct_equal(const T t1, const T t2) {
    using U = typename std::remove_pointer<typename std::remove_const<T>::type>::type;
    static_assert(std::is_trivially_copyable<U>::value, "structure must be trivially copyable");
    return (0 == std::memcmp(t1, t2, sizeof(U)));
}

template <typename T>
typename std::enable_if<std::is_class<T>::value && !std::is_pointer<T>::value, bool>::type struct_equal(const T& t1, const T& t2) {
    static_assert(std::is_trivially_copyable<T>::value, "structure must be trivially copyable");
    return (0 == std::memcmp(&t1, &t2, sizeof(T)));
}
