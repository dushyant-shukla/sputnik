#pragma once

#include "pch.h"

#include <transform.h>
#include <matrix4.h>

namespace sputnik::api::animation
{

/**
 * A pose is a hierarchy of transforms, where the value of each such transform affects all of its children. The pose
 * class keeps track of the tranformation of every joint in the skeleton. It also tracks the parent joint of every
 * joint.
 */
class Pose
{

public:
    // public interface
    Pose();
    Pose(const Pose& other);
    Pose& operator=(const Pose& other);
    Pose(size_t num_joints);

    void                 SetNumJoints(size_t new_num_joints);
    size_t               GetNumJoints();
    int                  GetParent(size_t joint_index);
    void                 SetParent(size_t joint_index, int parent_joint_index);
    ramanujan::Transform GetLocalTransform(size_t joint_index);
    void                 SetLocalTransform(size_t joint_index, const ramanujan::Transform& transform);
    ramanujan::Transform GetGlobalTransform(size_t joint_index);
    ramanujan::Transform operator[](size_t joint_index);

    /**
     * Global transforms for every joint in the transform hierarchy has to be passed to the gpu, therefore it must be
     * converted into a linear array of matrices. This method takes in a reference to a vector of Matrix4, and fills it
     * with the global transform matrices of all joints in the pose.
     *
     * \param out
     */
    void GetMatrixPalette(std::vector<ramanujan::Matrix4>& out);

    bool operator==(const Pose& other);
    bool operator!=(const Pose& other);

protected:
    /**
     * This stores transformations for every joint in the transform hierarchy.
     */
    std::vector<ramanujan::Transform> m_joints;

    /*
     * This stores index of the parent transform for each joint. Not all joints have parents. If a joint does not have a
     * parent, its parent value is negative.
     */
    std::vector<int> m_parents;

private:
    // private methods

private:
    // private data
};

} // namespace sputnik::api::animation