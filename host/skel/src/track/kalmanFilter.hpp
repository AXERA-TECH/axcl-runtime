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

#include "dataType.hpp"

namespace axcl::skel {
class KalmanFilter {
public:
    static const double chi2inv95[10];

    KalmanFilter();
    KAL_DATA initiate(const DETECT_BOX& measurement);
    void predict(KAL_MEAN& mean, KAL_COVA& covariance);
    KAL_HDATA project(const KAL_MEAN& mean, const KAL_COVA& covariance, float score = .0f);  // NSA kalman filter
    KAL_DATA update(const KAL_MEAN& mean, const KAL_COVA& covariance, const DETECT_BOX& measurement,
                    float score = .0f);  // NSA kalman filter //

    Eigen::Matrix<float, 1, -1> gating_distance(const KAL_MEAN& mean, const KAL_COVA& covariance,
                                                const std::vector<DETECT_BOX>& measurements, bool only_position = false);

private:
    Eigen::Matrix<float, 8, 8, Eigen::RowMajor> _motion_mat;
    Eigen::Matrix<float, 4, 8, Eigen::RowMajor> _update_mat;
    float _std_weight_position;
    float _std_weight_velocity;
};
}  // namespace axcl::skel

using namespace axcl::skel;
