#include "pch.h"
#include "pose.h"

namespace sputnik::graphics::core
{

Pose::Pose() {}

Pose::Pose(const Pose& other)
{
    *this = other;
}

Pose::Pose(size_t num_joints)
{
    SetNumJoints(num_joints);
}

Pose& Pose::operator=(const Pose& other)
{
    if(this == &other)
    {
        return *this;
    }

    if(m_parents.size() != other.m_parents.size())
    {
        m_parents.resize(other.m_parents.size());
    }

    if(m_joint_transforms.size() != other.m_joint_transforms.size())
    {
        m_joint_transforms.resize(other.m_joint_transforms.size());
    }

    if(m_parents.size() != 0)
    {
        memcpy(&m_parents[0], &other.m_parents[0], sizeof(int) * m_parents.size());
    }

    if(m_joint_transforms.size() != 0)
    {
        memcpy(&m_joint_transforms[0],
               &other.m_joint_transforms[0],
               sizeof(ramanujan::Transform) * m_joint_transforms.size());
    }

    return *this;
}

void Pose::SetNumJoints(size_t new_num_joints)
{
    m_parents.resize(new_num_joints);
    m_joint_transforms.resize(new_num_joints);
}

size_t Pose::GetNumJoints() const
{
    return m_joint_transforms.size();
}

int Pose::GetParent(size_t joint_index) const
{
    return m_parents[joint_index];
}

void Pose::SetParent(size_t joint_index, int parent_joint_index)
{
    m_parents[joint_index] = parent_joint_index;
}

ramanujan::Transform Pose::GetLocalTransform(size_t joint_index) const
{
    return m_joint_transforms[joint_index];
}

void Pose::SetLocalTransform(size_t joint_index, const ramanujan::Transform& transform)
{
    m_joint_transforms[joint_index] = transform;
}

/**
 * Given a joint, the global transform is calculated by combining all the transforms up the parent hierarchy of the
 * joint until the root joint.
 *
 * \param joint_index
 * \return
 */
ramanujan::Transform Pose::GetGlobalTransform(size_t joint_index) const
{
    ramanujan::Transform result = m_joint_transforms[joint_index]; // start with the transform for the joint
    for(int parent_index = m_parents[joint_index]; parent_index >= 0; parent_index = m_parents[parent_index])
    {
        // build the global transform for the joint by combining the joint's transform with its parents' transform.
        // Transform concatenation is carried out from right-to-left.
        result = ramanujan::Combine(m_joint_transforms[parent_index], result);
    }
    return result;
}

/**
 * This operator is an alias for GetGlobalTransform().
 *
 * \param index
 * \return
 */
ramanujan::Transform Pose::operator[](size_t joint_index) const
{
    return GetGlobalTransform(joint_index);
}

void Pose::GetMatrixPalette(std::vector<ramanujan::Matrix4>& out_matrix_pallete)
{
    size_t num_joints = GetNumJoints();
    if(out_matrix_pallete.size() != num_joints)
    {
        out_matrix_pallete.resize(num_joints);
    }

    /**
     * Optimized approach.
     *
     * GetGlobalTransform() is slow as it loops through every joint all the way up the specified joints transform
     * hierarchy until the root joint. This can be improved, if the the m_joint_transforms array can be arranged in a
     * way so that the parent joints have a lower index than their children joints. If this is true, we can iterate
     * through all the joints knowing that the parent matrix of the joint at the current index has already been found as
     * all parent joints have a lower index than their children.
     */
    int i = 0;
    for(; i < num_joints; ++i)
    {
        int parent = m_parents[i];
        // check to see if the order of joints is valid, i.e., parent joint has lower index than the child joint
        if(parent > i)
        {
            // break, to go through the slower route for calculating the global transforms matrix pallete.
            break;
        }

        /**
         * To get the global transform of a joint, we just need to combine the local matrix for this joint with the
         * previously found global transform.
         */
        ramanujan::Matrix4 global_transform = ramanujan::ToMatrix4(m_joint_transforms[i]);
        if(parent >= 0)
        {
            global_transform = out_matrix_pallete[parent] * global_transform;
        }
        out_matrix_pallete[i] = global_transform;
    }

    // Fallback, slower approach.
    // It is not always to possible to ensure the required order of joints in the m_joint_transforms array. In such a
    // case, use the slower GetGlobalTransform() method.
    for(; i < num_joints; ++i)
    {
        ramanujan::Transform global_transform = GetGlobalTransform(i);
        out_matrix_pallete[i]                 = ramanujan::ToMatrix4(global_transform);
    }
}

bool Pose::operator==(const Pose& other)
{
    if(m_joint_transforms.size() != other.m_joint_transforms.size())
    {
        return false;
    }
    if(m_parents.size() != other.m_parents.size())
    {
        return false;
    }

    unsigned int size = (unsigned int)m_joint_transforms.size();
    for(unsigned int i = 0; i < size; ++i)
    {
        ramanujan::Transform this_local   = m_joint_transforms[i];
        ramanujan::Transform other_local  = other.m_joint_transforms[i];
        int                  this_parent  = m_parents[i];
        int                  other_parent = other.m_parents[i];

        if(this_parent != other_parent)
        {
            return false;
        }

        // Todo: Create a operator== for Transform class
        if(this_local.position != other_local.position)
        {
            return false;
        }

        if(other_local.rotation != other_local.rotation)
        {
            return false;
        }

        if(other_local.scale != other_local.scale)
        {
            return false;
        }
    }

    return true;
}

bool Pose::operator!=(const Pose& other)
{
    return !(*this == other);
}

bool IsInHierarchy(const Pose& pose, const unsigned int parent_node, const unsigned int search_node)
{
    if(search_node == parent_node)
    {
        return true;
    }

    int current_parent_node = pose.GetParent(search_node);

    while(current_parent_node >= 0)
    {
        if(current_parent_node == static_cast<int>(parent_node))
        {
            return true;
        }
        current_parent_node = pose.GetParent(current_parent_node);
    }

    return false;
}

/*
 * Blending is done in local space of the two poses, i.e, linearly interpolating between the local transforms of the
 * joints in the input poses that are being blended. Position and scale interpolation uses the vector lerp function,
 * whereas rotation uses the quaternion nlerp function.
 */
void Blend(Pose& pose_out, const Pose& pose_a, const Pose& pose_b, const float t, const int root_node)
{
    unsigned int num_joints = static_cast<unsigned int>(pose_out.GetNumJoints());
    for(unsigned int i = 0; i < num_joints; ++i)
    {
        if(root_node >= 0)
        {
            if(!IsInHierarchy(pose_out, root_node, i))
            {
                continue;
            }
        }

        pose_out.SetLocalTransform(i, ramanujan::Mix(pose_a.GetLocalTransform(i), pose_b.GetLocalTransform(i), t));
    }
}

} // namespace sputnik::graphics::core