#pragma once

#include "pch.h"

#include <transform.h>
#include <matrix4.h>

namespace sputnik::graphics::core
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

    ramanujan::Transform operator[](size_t joint_index) const;
    bool                 operator==(const Pose& other);
    bool                 operator!=(const Pose& other);

    void                 SetNumJoints(size_t new_num_joints);
    size_t               GetNumJoints() const;
    int                  GetParent(size_t joint_index) const;
    void                 SetParent(size_t joint_index, int parent_joint_index);
    ramanujan::Transform GetLocalTransform(size_t joint_index) const;
    void                 SetLocalTransform(size_t joint_index, const ramanujan::Transform& transform);
    ramanujan::Transform GetGlobalTransform(size_t joint_index) const;

    /**
     * Global transforms for every joint in the transform hierarchy has to be passed to the gpu, therefore it must be
     * converted into a linear array of matrices. This method takes in a reference to a vector of Matrix4, and fills it
     * with the global transform matrices of all joints in the pose.
     *
     * \param out
     */
    void GetMatrixPalette(std::vector<ramanujan::Matrix4>& out);

    /**
     * This method determines whether a node is in the hierarchy of the given parent node in an animation pose.
     *
     * \param pose The input pose
     * \param parent_node Parent node
     * \param search_node Node to searched
     * \return true if search node is in hierarchy of the parent node, false otherwise.
     */
    friend bool IsInHierarchy(const Pose& pose, const unsigned int parent_node, const unsigned int search_node);

    /**
     * This method blends two poses using linear interpolation.
     *
     * \param pose_out The result of the blending the two poses
     * \param pose_a The first pose
     * \param pose_b The second pose
     * \param t The interpolation value ranging between 0 and 1.
     * \param root_node The root node determines which nodes (and its children) of the second pose should be blended
     * into the first pose.
     */
    friend void Blend(Pose& pose_out, const Pose& pose_a, const Pose& pose_b, const float t, const int root_node);

protected:
    /**
     * This stores transformations for every joint in the transform hierarchy.
     */
    std::vector<ramanujan::Transform> m_joint_transforms;

    /*
     * This stores index of the parent transform for each joint. Not all joints have parents. If a joint does not have a
     * parent, its parent value is negative.
     */
    std::vector<int> m_parents;
};

} // namespace sputnik::graphics::core