/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * License); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * AS IS BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#pragma once

#include <array>
#include <type_traits>
#include <vector>

#include "utilities/split.hpp"

namespace axcl::skel {
template <typename T, size_t N>
bool parse_string(const std::string& argument_string, std::array<T, N>& arguments, const std::string& delimiter = ",") {
    std::vector<std::string> result = split_string(argument_string, delimiter);

    if (N != result.size()) {
        return false;
    }

    for (size_t i = 0; i < N; i++) {
        if (std::is_integral<T>::value) {
            arguments[i] = std::stoi(result[i]);
        }

        if (std::is_floating_point<T>::value) {
            arguments[i] = std::stof(result[i]);
        }
    }

    return true;
}
}  // namespace axcl::skel

using namespace axcl::skel;
