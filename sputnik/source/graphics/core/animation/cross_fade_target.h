#pragma once

#include "pose.h"
#include "animation_clip.h"

namespace sputnik::graphics::core
{
struct CrossFadeTarget
{
    Pose           m_pose;
    AnimationClip* m_clip;
    float          m_time;
    float          m_duration;
    float          m_elapsed;

    inline CrossFadeTarget() : m_clip(0), m_time(0.0f), m_duration(0.0f), m_elapsed(0.0f) {}

    inline CrossFadeTarget(AnimationClip* const target_clip, const Pose& pose, float duration)
        : m_clip(target_clip)
        , m_time(target_clip->GetStartTime())
        , m_pose(pose)
        , m_duration(duration)
        , m_elapsed(0.0f)
    {
    }
};
} // namespace sputnik::graphics::core
