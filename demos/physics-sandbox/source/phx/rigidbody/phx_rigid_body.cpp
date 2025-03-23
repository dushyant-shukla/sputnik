#include "phx_rigid_body.hpp"
#include "../phx_math_utils.hpp"

namespace phx::rb
{

static void
calculateTransformationMatrix(const PhxVec3& position, const PhxQuat& orientation, PhxMat4& out_transform_matrix)
{
    out_transform_matrix = glm::translate(PhxMat4(1.0f), position) * glm::mat4_cast(orientation);
}

static void calculateInvInsertiaTensorInWorldSpace(const PhxMat3& inv_inertia_tensor_local,
                                                   const PhxMat4& transform_matrix,
                                                   PhxMat3&       out_inv_inertia_tensor_world)
{
    // When transforming insertia tensor, we are only interested in the rotation part of the transformation matrix,
    // i.e., it does not matter where the body is in space, only the direction in which it is oriented.
    // Why? Because inertia tensor is a property of the body itself, it describes how a body's mass is distributed
    // relative to its center of mass. It is independent of the body's position in space.
    PhxMat3 rot_mat = glm::mat3_cast(glm::quat_cast(transform_matrix));

    // Look closely at the following transformartion. It is equivalent to change of basis conversion:
    // M = Mb * Mt * inv(Mb)), where,
    // Mb: Change of basis matrix between the two coordinate spaces
    // Mt: Required transformation (Matrix in the local coordinate space)
    // Remember, transpose of an orthogonal matrix (rotation matix) gives its inverse.
    out_inv_inertia_tensor_world = rot_mat * inv_inertia_tensor_local * glm::transpose(rot_mat);
}

void PhxRigidBody::setInertiaTensor(const PhxMat3& inertia_tensor)
{
    m_inv_inertia_tensor_local = glm::inverse(inertia_tensor);
}

void PhxRigidBody::setDamping(const PhxReal& linear_damping, const PhxReal& angular_damping) {}

void PhxRigidBody::setAcceleration(const PhxVec3& acceleration) {}

void PhxRigidBody::addForce(const PhxVec3& force)
{
    m_accumulated_force += force;
    m_is_awake = true;
}

void PhxRigidBody::clearAccumulators()
{
    m_accumulated_force  = PhxVec3(0.0f);
    m_accumulated_torque = PhxVec3(0.0f);
}

void PhxRigidBody::integrate(PhxReal duration)
{
    if(!m_is_awake)
    {
        return;
    }

    m_acceleration_last_frame = m_acceleration;
    m_acceleration_last_frame += m_accumulated_force * m_inv_mass;

    PhxVec3 angular_acceleration = m_inv_inertia_tensor_world * m_accumulated_torque;

    m_velocity_world += m_acceleration_last_frame * duration;

    m_rotation_world += angular_acceleration * duration;

    m_velocity_world *= std::pow(m_linear_damping, duration);
    m_rotation_world *= std::pow(m_angular_damping, duration);

    m_position_world += m_velocity_world * duration;
    m_orientation_world +=
        0.5f * PhxQuat(0.0f, m_rotation_world.x, m_rotation_world.y, m_rotation_world.z) * m_orientation_world;

    calculateDerivedData();

    clearAccumulators();

    if(m_can_sleep)
    {
        PhxReal current_motion =
            phx_dot(m_velocity_world, m_velocity_world) + phx_dot(m_rotation_world, m_rotation_world);
        PhxReal bias = std::pow(0.5f, duration);
        m_motion     = bias * m_motion + (1.0f - bias) * current_motion;
        if(m_motion < phx::kPhxEpsilon)
        {
            setAwake(false);
        }
        else if(m_motion > 10.0f * phx::kPhxEpsilon)
        {
            m_motion = 10.0f * phx::kPhxEpsilon;
        }
    }
}

void PhxRigidBody::addForceAtPoint(const PhxVec3& force, const PhxVec3& point)
{
    m_accumulated_force += force;
    PhxVec3 torque = phx_cross(point - m_position_world, force);
    m_accumulated_torque += torque;
    m_is_awake = true;
}

void PhxRigidBody::addForceAtBodyPoint(const PhxVec3& force, const PhxVec3& point)
{
    PhxVec3 world_point = PhxMat3(m_transform_matrix_world) * point;
    addForceAtPoint(force, world_point);
    m_is_awake = true;
}

void PhxRigidBody::setAwake(bool awake)
{
    if(awake)
    {
        m_is_awake = true;
        m_motion   = 10.0f * phx::kPhxEpsilon;
    }
    else
    {
        m_is_awake       = false;
        m_velocity_world = PhxVec3(0.0f);
        m_rotation_world = PhxVec3(0.0f);
    }
}

void PhxRigidBody::setCanSleep(bool can_sleep)
{
    m_can_sleep = can_sleep;
    if(!m_can_sleep && !m_is_awake)
    {
        setAwake();
    }
}

void PhxRigidBody::setMass(const PhxReal& mass) {}

void PhxRigidBody::calculateDerivedData()
{
    m_orientation_world = glm::normalize(m_orientation_world);

    calculateTransformationMatrix(m_position_world, m_orientation_world, m_transform_matrix_world);

    calculateInvInsertiaTensorInWorldSpace(m_inv_inertia_tensor_local,
                                           m_transform_matrix_world,
                                           m_inv_inertia_tensor_world);
}

} // namespace phx::rb