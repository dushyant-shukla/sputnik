#include "pch.h"
#include "animation_track.h"

namespace sputnik::graphics::core
{

AnimationTrack::AnimationTrack() : m_joint_id(0) {}

unsigned int AnimationTrack::GetJointId()
{
    return m_joint_id;
}

void AnimationTrack::SetJointId(unsigned int joint_id)
{
    m_joint_id = joint_id;
}

VectorTrack& AnimationTrack::GetPositionTrack()
{
    return m_position;
}

VectorTrack& AnimationTrack::GetScaleTrack()
{
    return m_scale;
}

QuaternionTrack& AnimationTrack::GetRotationTrack()
{
    return m_rotation;
}

float AnimationTrack::GetStartTime()
{
    float result = 0.0f;
    bool  is_set = false;

    if(m_position.GetSize() > 1)
    {
        result = m_position.GetStartTime();
        is_set = true;
    }

    if(m_rotation.GetSize() > 1)
    {
        float rotation_start = m_rotation.GetStartTime();
        if(rotation_start < result || !is_set)
        {
            result = rotation_start;
            is_set = true;
        }
    }

    if(m_scale.GetSize() > 1)
    {
        float scale_start = m_scale.GetStartTime();
        if(scale_start < result || !is_set)
        {
            result = scale_start;
        }
    }

    return result;
}

float AnimationTrack::GetEndTime()
{
    float result = 0.0f;
    bool  is_set = false;

    if(m_position.GetSize() > 1)
    {
        result = m_position.GetEndTime();
        is_set = true;
    }

    if(m_rotation.GetSize() > 1)
    {
        float rotation_end = m_rotation.GetEndTime();
        if(rotation_end > result || !is_set)
        {
            result = rotation_end;
            is_set = true;
        }
    }

    if(m_scale.GetSize() > 1)
    {
        float scale_end = m_scale.GetEndTime();
        if(scale_end > result || !is_set)
        {
            result = scale_end;
        }
    }

    return result;
}

bool AnimationTrack::IsValid() const
{
    return m_position.GetSize() > 1 || m_rotation.GetSize() > 1 || m_scale.GetSize() > 1;
}

ramanujan::Transform AnimationTrack::Sample(const ramanujan::Transform& transform, float time, bool looping)
{
    ramanujan::Transform result = transform;
    if(m_position.GetSize() > 1)
    {
        result.position = m_position.Sample(time, looping);
    }

    if(m_rotation.GetSize() > 1)
    {
        result.rotation = m_rotation.Sample(time, looping);
    }

    if(m_scale.GetSize() > 1)
    {
        result.scale = m_scale.Sample(time, looping);
    }

    return result;
}

} // namespace sputnik::graphics::core