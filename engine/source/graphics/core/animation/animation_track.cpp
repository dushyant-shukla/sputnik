#include "pch.h"
#include "animation_track.h"

namespace sputnik::graphics::core
{

/**
 * Template specializations
 */
template TAnimationTrack<VectorTrack, QuaternionTrack>;
template TAnimationTrack<FastVectorTrack, FastQuaternionTrack>;

template <typename VEC_TRACK_TYPE, typename QUAT_TRACK_TYPE>
TAnimationTrack<VEC_TRACK_TYPE, QUAT_TRACK_TYPE>::TAnimationTrack() : m_joint_id(0)
{
}

template <typename VEC_TRACK_TYPE, typename QUAT_TRACK_TYPE>
unsigned int TAnimationTrack<VEC_TRACK_TYPE, QUAT_TRACK_TYPE>::GetJointId() const
{
    return m_joint_id;
}

template <typename VEC_TRACK_TYPE, typename QUAT_TRACK_TYPE>
void TAnimationTrack<VEC_TRACK_TYPE, QUAT_TRACK_TYPE>::SetJointId(unsigned int joint_id)
{
    m_joint_id = joint_id;
}

template <typename VEC_TRACK_TYPE, typename QUAT_TRACK_TYPE>
void TAnimationTrack<VEC_TRACK_TYPE, QUAT_TRACK_TYPE>::SetPositionTrack(const VEC_TRACK_TYPE& position)
{
    m_position = position;
}

template <typename VEC_TRACK_TYPE, typename QUAT_TRACK_TYPE>
VEC_TRACK_TYPE& TAnimationTrack<VEC_TRACK_TYPE, QUAT_TRACK_TYPE>::GetPositionTrack()
{
    return m_position;
}

template <typename VEC_TRACK_TYPE, typename QUAT_TRACK_TYPE>
const VEC_TRACK_TYPE& TAnimationTrack<VEC_TRACK_TYPE, QUAT_TRACK_TYPE>::GetPositionTrack() const
{
    return m_position;
}

template <typename VEC_TRACK_TYPE, typename QUAT_TRACK_TYPE>
void TAnimationTrack<VEC_TRACK_TYPE, QUAT_TRACK_TYPE>::SetScaleTrack(const VEC_TRACK_TYPE& scale)
{
    m_scale = scale;
}

template <typename VEC_TRACK_TYPE, typename QUAT_TRACK_TYPE>
VEC_TRACK_TYPE& TAnimationTrack<VEC_TRACK_TYPE, QUAT_TRACK_TYPE>::GetScaleTrack()
{
    return m_scale;
}

template <typename VEC_TRACK_TYPE, typename QUAT_TRACK_TYPE>
const VEC_TRACK_TYPE& TAnimationTrack<VEC_TRACK_TYPE, QUAT_TRACK_TYPE>::GetScaleTrack() const
{
    return m_scale;
}

template <typename VEC_TRACK_TYPE, typename QUAT_TRACK_TYPE>
void TAnimationTrack<VEC_TRACK_TYPE, QUAT_TRACK_TYPE>::SetRotationTrack(const QUAT_TRACK_TYPE& rotation)
{
    m_rotation = rotation;
}

template <typename VEC_TRACK_TYPE, typename QUAT_TRACK_TYPE>
QUAT_TRACK_TYPE& TAnimationTrack<VEC_TRACK_TYPE, QUAT_TRACK_TYPE>::GetRotationTrack()
{
    return m_rotation;
}

template <typename VEC_TRACK_TYPE, typename QUAT_TRACK_TYPE>
const QUAT_TRACK_TYPE& TAnimationTrack<VEC_TRACK_TYPE, QUAT_TRACK_TYPE>::GetRotationTrack() const
{
    return m_rotation;
}

template <typename VEC_TRACK_TYPE, typename QUAT_TRACK_TYPE>
float TAnimationTrack<VEC_TRACK_TYPE, QUAT_TRACK_TYPE>::GetStartTime() const
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

template <typename VEC_TRACK_TYPE, typename QUAT_TRACK_TYPE>
float TAnimationTrack<VEC_TRACK_TYPE, QUAT_TRACK_TYPE>::GetEndTime() const
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

template <typename VEC_TRACK_TYPE, typename QUAT_TRACK_TYPE>
bool TAnimationTrack<VEC_TRACK_TYPE, QUAT_TRACK_TYPE>::IsValid() const
{
    return m_position.GetSize() > 1 || m_rotation.GetSize() > 1 || m_scale.GetSize() > 1;
}

template <typename VEC_TRACK_TYPE, typename QUAT_TRACK_TYPE>
ramanujan::Transform TAnimationTrack<VEC_TRACK_TYPE, QUAT_TRACK_TYPE>::Sample(const ramanujan::Transform& transform,
                                                                              float                       time,
                                                                              bool                        looping)
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

FastAnimationTrack OptimizeAnimationTrack(AnimationTrack& input)
{
    FastAnimationTrack result;
    result.SetJointId(input.GetJointId());
    result.SetPositionTrack(OptimizeTrack<ramanujan::Vector3, 3>(input.GetPositionTrack()));
    result.SetScaleTrack(OptimizeTrack<ramanujan::Vector3, 3>(input.GetScaleTrack()));
    result.SetRotationTrack(OptimizeTrack<ramanujan::Quaternion, 4>(input.GetRotationTrack()));
    return result;
}

} // namespace sputnik::graphics::core