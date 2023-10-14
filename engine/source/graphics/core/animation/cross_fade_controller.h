#pragma once

#include "pch.h"
#include "cross_fade_target.h"
#include "skeleton.h"

namespace sputnik::graphics::core
{

class CrossFadeController
{

public:
protected:
    AnimationClip*               m_clip;
    float                        m_time;
    Pose                         m_pose;
    Skeleton                     m_skeleton;
    bool                         m_is_skeleton_set;
    std::vector<CrossFadeTarget> m_fade_targets;
};

} // namespace sputnik::graphics::core