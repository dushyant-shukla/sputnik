#include "pch.h"
#include "fabrik_solver.h"

#include <transform.h>

namespace sputnik::graphics::core
{
FabrikSolver::FabrikSolver() : m_num_steps(15), m_threshold(1.0e-5f) {}

bool FabrikSolver::Solve(const Transform& target)
{
    unsigned int size = Size();
    if(size == 0)
    {
        return false;
    }

    unsigned int last_joint_index = size - 1;
    float        threshold_sq     = m_threshold * m_threshold;

    LocalTransformsToGlobalPositions();

    Vector3 target_position = target.position;
    Vector3 base            = m_global_ik_chain[0];

    for(unsigned int i = 0; i < m_num_steps; ++i)
    {
        Vector3 end_effector = m_global_ik_chain[last_joint_index];
        if(LengthSq(target_position - end_effector) < threshold_sq)
        {
            GlobalPositionsToLocalTransforms();
            return true;
        }

        IterateBackward(target_position);
        IterateForward(base);
    }

    GlobalPositionsToLocalTransforms();
    Vector3 end_effector = GetGlobalTransform(last_joint_index).position;
    if(LengthSq(target_position - end_effector) < threshold_sq)
    {
        return true;
    }

    return false;
}

unsigned int FabrikSolver::Size()
{
    return static_cast<unsigned int>(m_local_ik_chain.size());
}

void FabrikSolver::Resize(unsigned int new_size)
{
    m_local_ik_chain.resize(new_size);
    m_global_ik_chain.resize(new_size);
    m_distances.resize(new_size);
}

unsigned int FabrikSolver::GetNumSteps()
{
    return m_num_steps;
}

void FabrikSolver::SetNumSteps(unsigned int num_steps)
{
    m_num_steps = num_steps;
}

float FabrikSolver::GetThreshold()
{
    return m_threshold;
}

void FabrikSolver::SetThreshold(float threshold)
{
    m_threshold = threshold;
}

Transform FabrikSolver::GetLocalTransform(unsigned int joint_index)
{
    return m_local_ik_chain[joint_index];
}

void FabrikSolver::SetLocalTransform(unsigned int joint_index, const Transform& transform)
{
    m_local_ik_chain[joint_index] = transform;
}

Transform FabrikSolver::GetGlobalTransform(unsigned int joint_index)
{
    Transform global_transform = m_local_ik_chain[joint_index];
    for(int i = joint_index - 1; i >= 0; --i)
    {
        global_transform = Combine(m_local_ik_chain[i], global_transform);
    }
    return global_transform;
}

Transform& FabrikSolver::operator[](unsigned int index)
{
    return m_local_ik_chain[index];
}

void FabrikSolver::LocalTransformsToGlobalPositions()
{
    unsigned int size = Size();
    for(unsigned int joint_index = 0; joint_index < size; ++joint_index)
    {
        Transform global_transform     = GetGlobalTransform(joint_index);
        m_global_ik_chain[joint_index] = global_transform.position;
        if(joint_index >= 1)
        {
            Vector3 prev_joint_global_position = m_global_ik_chain[joint_index - 1];
            m_distances[joint_index]           = Distance(m_global_ik_chain[joint_index], prev_joint_global_position);
        }
    }
    if(size > 0)
    {
        m_distances[0] = 0.0f;
    }
}

void FabrikSolver::IterateForward(const Vector3& base)
{
    unsigned int size = (unsigned int)Size();
    if(size > 0)
    {
        // Place the initial joint to be at the base
        m_global_ik_chain[0] = base;
    }

    // Adjust rest of the IK chain so that it remains intact
    for(size_t joint_index = 1; joint_index < size; ++joint_index)
    {
        Vector3 direction = Normalized(m_global_ik_chain[joint_index] - m_global_ik_chain[joint_index - 1]);
        Vector3 offset    = direction * m_distances[joint_index];
        m_global_ik_chain[joint_index] = m_global_ik_chain[joint_index - 1] + offset;
    }
}

void FabrikSolver::IterateBackward(const Vector3& target)
{
    unsigned int size = (unsigned int)Size();
    if(size > 0)
    {
        // Place the end-effector (last joint in the IK chain) to be at the target
        m_global_ik_chain[size - 1] = target;

        // Adjust rest of the IK chain using the stored distances so that the chain remains intact
        for(int i = size - 2; i >= 0; --i)
        {
            size_t  joint_index = static_cast<size_t>(i);
            Vector3 direction   = Normalized(m_global_ik_chain[joint_index] - m_global_ik_chain[joint_index + 1]);
            Vector3 offset      = direction * m_distances[joint_index + 1];
            m_global_ik_chain[joint_index] = m_global_ik_chain[joint_index + 1] + offset;
        }
    }
}

void FabrikSolver::GlobalPositionsToLocalTransforms()
{
    unsigned int size = Size();
    if(size == 0)
    {
        return;
    }

    for(unsigned int joint_index = 0; joint_index < size - 1; ++joint_index)
    {
        // TODO: Need to understand this better

        Transform current_global_transform    = GetGlobalTransform(joint_index);
        Transform next_joint_global_transform = GetGlobalTransform(joint_index + 1);

        Vector3 direction_to_next_joint = next_joint_global_transform.position - current_global_transform.position;
        direction_to_next_joint         = Inverse(current_global_transform.rotation) * direction_to_next_joint;

        Vector3 desired_direction_to_next_joint =
            m_global_ik_chain[joint_index + 1] - current_global_transform.position;
        desired_direction_to_next_joint = Inverse(current_global_transform.rotation) * desired_direction_to_next_joint;

        Quaternion delta                       = FromTo(direction_to_next_joint, desired_direction_to_next_joint);
        m_local_ik_chain[joint_index].rotation = delta * m_local_ik_chain[joint_index].rotation;
    }
}

} // namespace sputnik::graphics::core