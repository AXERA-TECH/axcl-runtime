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

#include "detection.hpp"
#include "track.hpp"

namespace axcl::skel {
class CBYTETracker {
public:
    CBYTETracker(const AX_U32& n_classes, const AX_U32& frame_rate, const AX_U32& track_buffer, const float& high_det_thresh,
                const float& new_track_thresh, const float& high_match_thresh, const float& low_match_thresh,
                const float& unconfirmed_match_thresh);
    ~CBYTETracker();
    track_map<AX_U32, vector<CTrack>> update(const AX_U32 &nStreamId, const AX_U64 &nFrameId, const vector<Object>& objects);

private:
    vector<CTrack*> joinTracks(vector<CTrack*>& tlista, vector<CTrack>& tlistb);
    vector<CTrack> joinTracks(vector<CTrack>& tlista, vector<CTrack>& tlistb);
    vector<CTrack> subTracks(vector<CTrack>& tlista, vector<CTrack>& tlistb);
    void removeDuplicateTracks(vector<CTrack>& resa, vector<CTrack>& resb, vector<CTrack>& tracks_a, vector<CTrack>& tracks_b);
    void linearAssignment(vector<vector<float>>& cost_matrix, AX_U32 cost_matrix_size, AX_U32 cost_matrix_size_size, float thresh,
                          vector<vector<AX_S32>>& matches, vector<AX_S32>& unmatched_a, vector<AX_S32>& unmatched_b);
    vector<vector<float>> iouDistance(vector<CTrack*>& atracks, vector<CTrack>& btracks, AX_U32& dist_size, AX_U32& dist_size_size);
    vector<vector<float>> iouDistance(vector<CTrack>& atracks, vector<CTrack>& btracks);
    vector<vector<float>> ious(vector<vector<float>>& atlbrs, vector<vector<float>>& btlbrs);
    double lapjv(const vector<vector<float>>& cost, vector<AX_S32>& rowsol, vector<AX_S32>& colsol, bool extend_cost = false,
                 float cost_limit = LONG_MAX, bool return_cost = true);

private:
    float m_high_det_thresh;
    float m_new_track_thresh;
    float m_high_match_thresh;
    float m_low_match_thresh;
    float m_unconfirmed_match_thresh;
    AX_U64 m_frame_id;
    AX_U32 m_max_time_lost;

    // tracking object class number
    AX_U32 m_N_CLASSES;

    // 3 containers of the tracker
    track_map<AX_U32, track_map<AX_U32, vector<CTrack>>> m_tracked_tracks_dict;
    track_map<AX_U32, track_map<AX_U32, vector<CTrack>>> m_lost_tracks_dict;
    track_map<AX_U32, track_map<AX_U32, vector<CTrack>>> m_removed_tracks_dict;

    KalmanFilter m_kalman_filter;
};
}  // namespace axcl::skel

using namespace axcl::skel;
