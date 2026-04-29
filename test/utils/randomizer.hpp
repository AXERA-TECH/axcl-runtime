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
#include <string.h>
#include <cstdint>
#include <random>
#include <array>

namespace axcl {

class randomizer {
public:
    randomizer() {
        m_gen.seed(std::random_device{}());
    }

    template <typename T>
    void randomize(T& obj) {
        static_assert(std::is_trivially_copyable<T>::value, "structure must be trivially copyable");

        std::uniform_int_distribution<unsigned short> dis(0, 0xFF);
        std::array<uint8_t, sizeof(T)> arr;
        for (auto&& m : arr) {
            m = static_cast<uint8_t>(dis(m_gen));
        }

        memcpy(&obj, arr.data(), sizeof(T));
    }

    void randomize(uint8_t *data, size_t size) {
        std::uniform_int_distribution<unsigned short> dis(0, 0xFF);
        for (size_t i = 0; i < size; ++i) {
            data[i] = static_cast<uint8_t>(dis(m_gen));
        }
    }

    size_t randomize_range(size_t min, size_t max) {
        std::uniform_int_distribution<size_t> dis(min, max);
        return dis(m_gen);
    }


private:
    std::mt19937 m_gen;
};

}  // namespace axcl