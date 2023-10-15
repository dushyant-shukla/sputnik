#pragma once

#include "track.h"
#include "fast_track.h"
#include "transform.h"

namespace sputnik::graphics::core
{

template <typename VEC_TRACK_TYPE, typename QUAT_TRACK_TYPE>
class TAnimationTrack
{

public:
    TAnimationTrack();
    unsigned int           GetJointId() const;
    void                   SetJointId(unsigned int joint_id);
    void                   SetPositionTrack(const VEC_TRACK_TYPE& position);
    VEC_TRACK_TYPE&        GetPositionTrack();
    const VEC_TRACK_TYPE&  GetPositionTrack() const;
    void                   SetScaleTrack(const VEC_TRACK_TYPE& scale);
    VEC_TRACK_TYPE&        GetScaleTrack();
    const VEC_TRACK_TYPE&  GetScaleTrack() const;
    void                   SetRotationTrack(const QUAT_TRACK_TYPE& rotation);
    QUAT_TRACK_TYPE&       GetRotationTrack();
    const QUAT_TRACK_TYPE& GetRotationTrack() const;
    float                  GetStartTime() const;
    float                  GetEndTime() const;
    bool                   IsValid() const;
    ramanujan::Transform   Sample(const ramanujan::Transform& transform, float time, bool looping);

protected:
    unsigned int    m_joint_id;
    VEC_TRACK_TYPE  m_position;
    VEC_TRACK_TYPE  m_scale;
    QUAT_TRACK_TYPE m_rotation;
};

/*
 * Common AnimationTrack types
 */
typedef TAnimationTrack<VectorTrack, QuaternionTrack>         AnimationTrack;
typedef TAnimationTrack<FastVectorTrack, FastQuaternionTrack> FastAnimationTrack;

FastAnimationTrack OptimizeAnimationTrack(AnimationTrack& input);

} // namespace sputnik::graphics::core
