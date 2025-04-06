#include "pch.h"
#include "ccd_solver.h"

namespace sputnik::graphics::core
{

CCDSolver::CCDSolver() : m_num_steps(15), m_threshold(1.0e-5) {}

unsigned int CCDSolver::Size()
{
    return (unsigned int)m_ik_chain.size();
}

void CCDSolver::Resize(unsigned int new_size)
{
    m_ik_chain.resize(new_size);
}

Transform& CCDSolver::operator[](unsigned int index)
{
    return m_ik_chain[index];
}

Transform CCDSolver::GetGlobalTransform(unsigned int index)
{
    unsigned int size = (unsigned int)m_ik_chain.size();

    // Get the global transform of the current joint by concatenating the transform of the current joint with the
    // transforms of all its parent joints.
    Transform world = m_ik_chain[index];
    for(int i = (int)index - 1; i >= 0; --i)
    {
        world = Combine(m_ik_chain[i], world);
    }
    return world;
}

unsigned int CCDSolver::GetNumSteps()
{
    return m_num_steps;
}

void CCDSolver::GetNumSteps(unsigned int num_steps)
{
    m_num_steps = num_steps;
}

double CCDSolver::GetThreshold()
{
    return m_threshold;
}

void CCDSolver::SetThreshold(float value)
{
    m_threshold = value;
}

bool CCDSolver::Solve(const Transform& target)
{
    unsigned int size = Size();

    if(size == 0)
    {
        return false;
    }

    unsigned int last         = size - 1;
    double       threshold_sq = m_threshold * m_threshold;

    for(unsigned int i = 0; i < m_num_steps; ++i)
    {
        // get the current end-effector transform in world space
        Transform end_effector_world_transform = GetGlobalTransform(last);

        // if the distance between the target and end effector is less than the threshold, the IK chain is considered
        // solved
        if(LengthSq(target.position - end_effector_world_transform.position) < threshold_sq)
        {
            return true;
        }

        // start from the joint before the end-effector, i.e., second to last joint in the IK chain
        for(int joint_index = (int)size - 2; joint_index >= 0; --joint_index)
        {
            end_effector_world_transform = GetGlobalTransform(last);

            Transform joint_world_transform = GetGlobalTransform(joint_index);

            // get the vector from current joint to the end effector
            Vector3 joint_to_end_effector = end_effector_world_transform.position - joint_world_transform.position;

            // get the vector from current joint to the target
            Vector3 joint_to_target = target.position - joint_world_transform.position;

            // find a quaternion that aligns the joint to end effector vector to the joint to target vector
            Quaternion end_effector_to_target_rotation;

            // Edge case: The two vectors above could be zero vectors. Therefore, check if the length of either of the
            // two vectors is above the threshold
            if(LengthSq(joint_to_target) > m_threshold)
            {
                end_effector_to_target_rotation = FromTo(joint_to_end_effector, joint_to_target);
            }

            // update the joint orientation in world space
            Quaternion updated_world_orientation = joint_world_transform.rotation * end_effector_to_target_rotation;

            // Rotate the world space orientation of the joint by the inverse of the joint's previous world rotation to
            // move the quaternion back into the joint space (local space).
            // Think of this calculation as following:
            // 1# updated rotation in world space = updated_rotation_delta_in_local_space *
            // updated_rotation_delta_in_world_space
            // 2# inverse of the joint's previous world space orientation cancels
            // out the updated rotation in world space, leaving only the local rotation delta
            Quaternion local_rotate          = updated_world_orientation * Inverse(joint_world_transform.rotation);
            m_ik_chain[joint_index].rotation = local_rotate * m_ik_chain[joint_index].rotation;

            // As the joint rotates, check how close the end-effector moved to the goal at each iteration
            // If the end-effector is close enough to the target, return early from the function.
            end_effector_world_transform = GetGlobalTransform(last);
            if(LengthSq(target.position - end_effector_world_transform.position) < threshold_sq)
            {
                return true;
            }
        }
    }

    // Return false if the goal wasn't reached, the IK chain can't be solved, atleast not it the number of iterations
    // specified
    return false;
}

void CCDSolver::FetchPoints(std::vector<ramanujan::experimental::vec4>& points)
{
    size_t req_size = Size();
    points.resize(req_size);

    for(unsigned int i = 0, size = Size(); i < size; ++i)
    {
        Transform world = GetGlobalTransform(i);
        points[i]       = ramanujan::experimental::vec4(world.position.x, world.position.y, world.position.z, 1.0f);
    }
}

void CCDSolver::FetchLines(std::vector<ramanujan::experimental::vec4>& points)
{
    if(Size() < 2)
    {
        return;
    }
    size_t required_verts = (static_cast<size_t>(Size()) - 1) * 2;
    points.resize(required_verts);

    unsigned int index = 0;
    for(unsigned int i = 0, size = Size(); i < size - 1; ++i)
    {
        Transform world = GetGlobalTransform(i);
        points[index++] = ramanujan::experimental::vec4(world.position.x, world.position.y, world.position.z, 1.0f);

        world           = GetGlobalTransform(i + 1);
        points[index++] = ramanujan::experimental::vec4(world.position.x, world.position.y, world.position.z, 1.0f);
    }
}

} // namespace sputnik::graphics::core