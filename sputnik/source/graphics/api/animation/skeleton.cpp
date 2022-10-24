#include "pch.h"
#include "skeleton.h"

#include <transform.h>

namespace sputnik::api::animation
{
Skeleton::Skeleton() {}

Skeleton::Skeleton(const Pose& rest_pose, const Pose& bind_pose, const std::vector<std::string>& joint_names)
{
    Set(rest_pose, bind_pose, joint_names);
}

void Skeleton::Set(const Pose& rest_pose, const Pose& bind_pose, const std::vector<std::string>& joint_names)
{
    m_rest_pose   = rest_pose;
    m_bind_pose   = bind_pose;
    m_joint_names = joint_names;
    UpdateInverseBindPose();
}

const Pose& Skeleton::GetBindPose() const
{
    return m_bind_pose;
}

const Pose& Skeleton::GetRestPose() const
{
    return m_rest_pose;
}

const std::vector<ramanujan::Matrix4>& Skeleton::GetInverseBindPose() const
{
    return m_inv_bind_pose;
}

const std::vector<std::string>& Skeleton::GetJointNames() const
{
    return m_joint_names;
}

std::string& Skeleton::GetJointName(size_t joint_index)
{
    return m_joint_names[joint_index];
}

void Skeleton::UpdateInverseBindPose()
{
    size_t num_joints = m_bind_pose.GetNumJoints();
    m_inv_bind_pose.resize(num_joints);

    for(size_t joint = 0; joint < num_joints; ++joint)
    {
        ramanujan::Transform world_transform = m_bind_pose.GetGlobalTransform(joint);
        m_inv_bind_pose[joint]               = ramanujan::Inverse(ramanujan::ToMatrix4(world_transform));
    }
}

} // namespace sputnik::api::animation