#include "pch.h"
#include "transform_track.h"

namespace sputnik::api::animation
{

TransformTrack::TransformTrack() : m_id(0) {}

unsigned int TransformTrack::GetId()
{
    return m_id;
}

void TransformTrack::SetId(unsigned int id)
{
    m_id = id;
}

VectorTrack& TransformTrack::GetPositionTrack()
{
    return m_position;
}

VectorTrack& TransformTrack::GetScaleTrack()
{
    return m_scale;
}

QuaternionTrack& TransformTrack::GetRotationTrack()
{
    return m_rotation;
}

float TransformTrack::GetStartTime()
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

float TransformTrack::GetEndTime()
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
        if(rotation_start > result || !is_set)
        {
            result = rotation_start;
            is_set = true;
        }
    }

    if(m_scale.GetSize() > 1)
    {
        float scale_start = m_scale.GetStartTime();
        if(scale_start > result || !is_set)
        {
            result = scale_start;
        }
    }

    return result;
}

bool TransformTrack::IsValid()
{
    return m_position.GetSize() > 1 || m_rotation.GetSize() > 1 || m_scale.GetSize() > 1;
}

ramanujan::Transform TransformTrack::Sample(const ramanujan::Transform& transform, float time, bool looping)
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

} // namespace sputnik::api::animation