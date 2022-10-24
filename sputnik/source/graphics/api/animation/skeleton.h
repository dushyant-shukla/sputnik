#pragma once

#include "pose.h"
#include "pch.h"

#include <matrix4.h>

namespace sputnik::api::animation
{

/**
 * The Skeleton class is a convenient structure for storing data that is common among multiple instances of an animated
 * model. It keeps track of the bind pose, inverse bind pose, joint names of an animated model. The skeleton is shared.
 * For example, you can have many characters (instances of a mesh), each with a unique animated pose, but they can all
 * share the same skeleton.
 */
class Skeleton
{

public:
    Skeleton();
    Skeleton(const Pose& rest_pose, const Pose& bind_pose, const std::vector<std::string>& joint_names);

    void Set(const Pose& rest_pose, const Pose& bind_pose, const std::vector<std::string>& joint_names);

    const Pose& GetBindPose() const;
    const Pose& GetRestPose() const;

    const std::vector<ramanujan::Matrix4>& GetInverseBindPose() const;
    const std::vector<std::string>&        GetJointNames() const;
    std::string&                           GetJointName(size_t joint_index);

protected:
    void UpdateInverseBindPose();

protected:
    Pose                            m_rest_pose;
    Pose                            m_bind_pose;
    std::vector<ramanujan::Matrix4> m_inv_bind_pose;
    std::vector<std::string>        m_joint_names;
};

} // namespace sputnik::api::animation