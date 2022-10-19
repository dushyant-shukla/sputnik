#include "pch.h"
#include "animation_clip.h"

#include <transform.h>

namespace sputnik::api::animation
{
AnimationClip::AnimationClip() : m_name("unnamed animation clip"), m_start_time(0.0f), m_end_time(0.0f), m_looping(true)
{
}

unsigned int AnimationClip::GetJointIdAtTrackIndex(unsigned int track_index)
{
    return m_tracks[track_index].GetJointId();
}

void AnimationClip::SetJointIdAtTrackIndex(unsigned int joint_id, unsigned int track_index)
{
    m_tracks[track_index].SetJointId(joint_id);
}

unsigned int AnimationClip::GetNumJoints()
{
    return static_cast<unsigned int>(m_tracks.size()); // Each track transform manages the transforms for one joint
}

/**
 * This method samples an animation clip for a given time into the Pose passed in as a reference. It returns a time
 * value.
 */
float AnimationClip::Sample(Pose& out_pose, float in_time)
{
    if(GetDuration() == 0.0f) // Check if the clip is valid
    {
        return 0.0f;
    }

    in_time = AdjustTimeToFitRange(in_time); // Ensure that the interpolation time is in the valid range of the clip

    size_t size = m_tracks.size();
    for(size_t i = 0; i < size; ++i)
    {
        unsigned int         joint_id            = m_tracks[i].GetJointId();
        ramanujan::Transform local_transform     = out_pose.GetLocalTransform(joint_id);
        ramanujan::Transform animation_transform = m_tracks[i].Sample(local_transform, in_time, m_looping);
        out_pose.SetLocalTransform(joint_id, animation_transform);
    }

    return in_time; // return the adjusted time
}

/**
 * The [] operator returns a transform track for the specified joint. If no track exists for a given joint, one is
 * created and returned.
 *
 * This method is intended to be called when loading an animation clip from a file.
 */
TrackTransform& AnimationClip::operator[](unsigned int joint_id)
{
    size_t num_tracks = m_tracks.size();

    // perform a linear search to see whether any of them targets the specified joint.
    for(int i = 0; i < num_tracks; ++i)
    {
        if(m_tracks[i].GetJointId() == joint_id)
        {
            return m_tracks[i]; // return the track transform with the required joint
        }
    }

    // If no required track transform is found, a new one is created and returned.
    m_tracks.push_back(TrackTransform());
    num_tracks = m_tracks.size();
    m_tracks[num_tracks - 1].SetJointId(joint_id);
    return m_tracks[num_tracks - 1];
}

/**
 * This method calculates the duration of an animation clip by determining the start and end times of an animation
 * clip.
 */
void AnimationClip::RecalculateDuration()
{
    m_start_time      = 0.0f;
    m_end_time        = 0.0f;
    bool   start_set  = false;
    bool   end_set    = false;
    size_t num_tracks = m_tracks.size();
    for(size_t i = 0; i < num_tracks; ++i)
    {
        if(m_tracks[i].IsValid())
        {
            float start_time = m_tracks[i].GetStartTime();
            float end_time   = m_tracks[i].GetEndTime();

            if(start_time < m_start_time || !start_set)
            {
                m_start_time = start_time;
                start_set    = true;
            }

            if(end_time > m_end_time || !end_set)
            {
                m_end_time = end_time;
                end_set    = true;
            }
        }
    }
}

std::string& AnimationClip::GetName()
{
    return m_name;
}

void AnimationClip::SetName(const std::string& name)
{
    m_name = name;
}

float AnimationClip::GetDuration()
{
    return m_end_time - m_start_time;
}

float AnimationClip::GetStartTime()
{
    return m_start_time;
}

float AnimationClip::GetEndTime()
{
    return m_end_time;
}

bool AnimationClip::IsLooping()
{
    return m_looping;
}

void AnimationClip::SetLooping(bool looping)
{
    m_looping = looping;
}

float AnimationClip::AdjustTimeToFitRange(float in_time)
{
    if(m_looping)
    {
        float duration     = m_end_time - m_start_time;
        float elapsed_time = in_time - m_start_time;
        if(duration <= 0.0f)
        {
            return 0.0f;
        }

        in_time = fmodf(elapsed_time, duration);
        if(in_time < 0.0f)
        {
            in_time += duration;
        }
    }
    else
    {
        if(in_time < m_start_time)
        {
            in_time = m_start_time;
        }
        if(in_time > m_end_time)
        {
            in_time = m_end_time;
        }
    }
    return in_time;
}

} // namespace sputnik::api::animation