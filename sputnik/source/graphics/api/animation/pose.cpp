#include "pch.h"
#include "pose.h"

namespace sputnik::api::animation
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

    if(m_joints.size() != other.m_joints.size())
    {
        m_joints.resize(other.m_joints.size());
    }

    if(m_parents.size() != 0)
    {
        memcpy(&m_parents[0], &other.m_parents[0], sizeof(int) * m_parents.size());
    }

    if(m_joints.size() != 0)
    {
        memcpy(&m_joints[0], &other.m_joints[0], sizeof(int) * m_joints.size());
    }

    return *this;
}

void Pose::SetNumJoints(size_t new_num_joints)
{
    m_parents.resize(new_num_joints);
    m_joints.resize(new_num_joints);
}

size_t Pose::GetNumJoints()
{
    return m_joints.size();
}

int Pose::GetParent(size_t joint_index)
{
    return m_parents[joint_index];
}

void Pose::SetParent(size_t joint_index, int parent_joint_index)
{
    m_parents[joint_index] = parent_joint_index;
}

ramanujan::Transform Pose::GetLocalTransform(size_t joint_index)
{
    return m_joints[joint_index];
}

void Pose::SetLocalTransform(size_t joint_index, const ramanujan::Transform& transform)
{
    m_joints[joint_index] = transform;
}

/**
 * Given a joint, the global transform is calculated by combining all the transforms up the parent hierarchy of the
 * joint until the root joint.
 *
 * \param joint_index
 * \return
 */
ramanujan::Transform Pose::GetGlobalTransform(size_t joint_index)
{
    ramanujan::Transform result = m_joints[joint_index]; // start with the transform for the joint
    for(int parent_index = m_parents[joint_index]; parent_index >= 0; parent_index = m_parents[parent_index])
    {
        // build the global transform for the joint by combining the joint's transform with its parents' transform.
        // Transform concatenation is carried out from right-to-left.
        result = ramanujan::Combine(m_joints[parent_index], result);
    }
    return result;
}

/**
 * This operator is an alias for GetGlobalTransform().
 *
 * \param index
 * \return
 */
ramanujan::Transform Pose::operator[](size_t joint_index)
{
    return GetGlobalTransform(joint_index);
}

void Pose::GetMatrixPalette(std::vector<ramanujan::Matrix4>& out)
{
    size_t size = GetNumJoints();
    if(out.size() != size)
    {
        out.resize(size);
    }
    for(unsigned int i = 0; i < size; ++i)
    {
        ramanujan::Transform t = GetGlobalTransform(i);
        out[i]                 = ramanujan::ToMatrix4(t);
    }
}

bool Pose::operator==(const Pose& other)
{
    if(m_joints.size() != other.m_joints.size())
    {
        return false;
    }
    if(m_parents.size() != other.m_parents.size())
    {
        return false;
    }

    unsigned int size = (unsigned int)m_joints.size();
    for(unsigned int i = 0; i < size; ++i)
    {
        ramanujan::Transform this_local   = m_joints[i];
        ramanujan::Transform other_local  = other.m_joints[i];
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
} // namespace sputnik::api::animation