#pragma once

#include "track.h"
#include "transform.h"

namespace sputnik::api::animation
{

class TransformTrack
{

public:
    TransformTrack();
    unsigned int         GetId();
    void                 SetId(unsigned int id);
    VectorTrack&         GetPositionTrack();
    VectorTrack&         GetScaleTrack();
    QuaternionTrack&     GetRotationTrack();
    float                GetStartTime();
    float                GetEndTime();
    bool                 IsValid();
    ramanujan::Transform Sample(const ramanujan::Transform& transform, float time, bool looping);

protected:
    unsigned int    m_id;
    VectorTrack     m_position;
    VectorTrack     m_scale;
    QuaternionTrack m_rotation;
};

} // namespace sputnik::api::animation