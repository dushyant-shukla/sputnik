#pragma once

#include "track.h"
#include "transform.h"

namespace sputnik::api::animation
{

class AnimationTrack
{

public:
    AnimationTrack();
    unsigned int         GetJointId();
    void                 SetJointId(unsigned int joint_id);
    VectorTrack&         GetPositionTrack();
    VectorTrack&         GetScaleTrack();
    QuaternionTrack&     GetRotationTrack();
    float                GetStartTime();
    float                GetEndTime();
    bool                 IsValid() const;
    ramanujan::Transform Sample(const ramanujan::Transform& transform, float time, bool looping);

protected:
    unsigned int    m_joint_id;
    VectorTrack     m_position;
    VectorTrack     m_scale;
    QuaternionTrack m_rotation;
};

} // namespace sputnik::api::animation