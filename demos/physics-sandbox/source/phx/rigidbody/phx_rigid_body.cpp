#include "phx_rigid_body.hpp"
#include "../phx_math_utils.hpp"

#include "core/core.h"

namespace phx::rb
{

static void
calculateTransformationMatrix(const PhxVec3& position, const PhxQuat& orientation, PhxMat4& out_transform_matrix)
{
    out_transform_matrix = glm::translate(PhxMat4(1.0f), position) * glm::mat4_cast(orientation);
}

static void calculateInvInsertiaTensorInWorldSpace(const PhxMat3& inv_inertia_tensor_local,
                                                   const PhxMat3& inertia_tensor_local,
                                                   const PhxMat4& transform_matrix,
                                                   PhxMat3&       out_inertia_tensor_world,
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
    out_inertia_tensor_world     = rot_mat * inertia_tensor_local * glm::transpose(rot_mat);
}

void PhxRigidBody::setInertiaTensor(const PhxMat3& inertia_tensor)
{
    // if(CMP_FLOAT_EQ(m_inv_mass, 0.0f))
    //{
    //     m_inv_inertia_tensor_local = PhxMat3(1.0f);
    //     return;
    // }
    // m_inv_inertia_tensor_local = glm::inverse(inertia_tensor);

    if(CMP_FLOAT_EQ(m_inv_mass, 0.0f))
    {
        m_inertia_tensor_local     = PhxMat3(0.0f);
        m_inv_inertia_tensor_local = PhxMat3(0.0f);
    }
    else
    {
        m_inertia_tensor_local     = inertia_tensor;
        m_inv_inertia_tensor_local = phx_inv_mat3(m_inertia_tensor_local);
    }
}

void PhxRigidBody::setDamping(const PhxReal& linear_damping, const PhxReal& angular_damping)
{
    m_linear_damping  = linear_damping;
    m_angular_damping = angular_damping;
    m_enable_damping  = true;
}

void PhxRigidBody::setAcceleration(const PhxVec3& acceleration)
{
    m_acceleration = acceleration;
}

void PhxRigidBody::setPosition(const PhxVec3& position)
{
    m_position_global = position;
    // calculateDerivedData(); // Todo: Is this really needed?
}

void PhxRigidBody::setVelocity(const PhxVec3& velocity)
{
    m_linear_velocity_global = velocity;
}

void PhxRigidBody::setRotation(const PhxVec3& rotation)
{
    m_angular_velocity_global = rotation;
}

void PhxRigidBody::setOrientation(const PhxQuat& orientation)
{
    m_orientation_world = glm::normalize(orientation);
}

void PhxRigidBody::setOrientation(const PhxReal& w, const PhxReal& x, const PhxReal& y, const PhxReal& z)
{
    m_orientation_world.w = w;
    m_orientation_world.x = x;
    m_orientation_world.y = y;
    m_orientation_world.z = z;
    m_orientation_world   = glm::normalize(m_orientation_world);
}

void PhxRigidBody::setInertiaTensorWithHalfSizesAndMass(const PhxVec3& half_sizes, const PhxReal& mass)
{
    PhxVec3 squares = {half_sizes.x * half_sizes.x, half_sizes.y * half_sizes.y, half_sizes.z * half_sizes.z};
    setInertiaTensorWithCoefficients(0.3f * mass * (squares.y + squares.z),
                                     0.3f * mass * (squares.x + squares.z),
                                     0.3f * mass * (squares.x + squares.y));
}

void PhxRigidBody::setInertiaTensorWithCoefficients(const PhxReal& ix,
                                                    const PhxReal& iy,
                                                    const PhxReal& iz,
                                                    const PhxReal& ixy,
                                                    const PhxReal& ixz,
                                                    const PhxReal& iyz)
{
    PhxMat3 inertia_tensor{};
    inertia_tensor[0][0] = ix;
    inertia_tensor[0][1] = -ixy;
    inertia_tensor[0][2] = -ixz;
    inertia_tensor[1][0] = -ixy;
    inertia_tensor[1][1] = iy;
    inertia_tensor[1][2] = -iyz;
    inertia_tensor[2][0] = -ixz;
    inertia_tensor[2][1] = -iyz;
    inertia_tensor[2][2] = iz;

    m_inv_inertia_tensor_local = glm::inverse(inertia_tensor);
}

void PhxRigidBody::setElasticity(const PhxReal& elasticity)
{
    m_elasticity = elasticity;
}

void PhxRigidBody::setFriction(const PhxReal& friction)
{
    m_friction = friction;
}

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

    /////////////////////////////////////////////////////////////////////////////////////////////

    // m_acceleration_last_frame = m_acceleration;
    // m_acceleration_last_frame += m_accumulated_force * m_inv_mass;

    // PhxVec3 angular_acceleration = m_inv_inertia_tensor_world * m_accumulated_torque;

    // m_velocity_world += m_acceleration_last_frame * duration;

    // m_angular_velocity_world += angular_acceleration * duration;

    // m_velocity_world *= std::pow(m_linear_damping, duration);
    // m_angular_velocity_world *= std::pow(m_angular_damping, duration);

    // m_position_world += m_velocity_world * duration;
    // m_orientation_world +=
    //     0.5f * PhxQuat(0.0f, m_angular_velocity_world.x, m_angular_velocity_world.y, m_angular_velocity_world.z) *
    //     m_orientation_world;

    // calculateDerivedData();

    // clearAccumulators();

    // if(m_can_sleep)
    //{
    //     PhxReal current_motion =
    //         phx_dot(m_velocity_world, m_velocity_world) + phx_dot(m_angular_velocity_world,
    //         m_angular_velocity_world);
    //     PhxReal bias = std::pow(0.5f, duration);
    //     m_motion     = bias * m_motion + (1.0f - bias) * current_motion;
    //     if(m_motion < phx::kPhxEpsilon)
    //     {
    //         setAwake(false);
    //     }
    //     else if(m_motion > 10.0f * phx::kPhxEpsilon)
    //     {
    //         m_motion = 10.0f * phx::kPhxEpsilon;
    //     }
    // }

    /////////////////////////////////////////////////////////////////////////////////////////////

    // m_acceleration_last_frame = m_acceleration;
    //// m_velocity_world += m_acceleration_last_frame * duration;

    // PhxVec3 total_impluse = getMass() * m_acceleration_last_frame * duration;

    // applyLinearImpulse(total_impluse, duration);

    // m_position_world += m_velocity_world * duration;

    // calculateDerivedData();
}

void PhxRigidBody::addForceAtPoint(const PhxVec3& force, const PhxVec3& point)
{
    m_accumulated_force += force;
    PhxVec3 torque = phx_cross(point - m_position_global, force);
    m_accumulated_torque += torque;
    m_is_awake = true;
}

void PhxRigidBody::addForceAtBodyPoint(const PhxVec3& force, const PhxVec3& point)
{
    PhxVec3 world_point = PhxMat3(m_transform_matrix_global) * point;
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
        m_is_awake                = false;
        m_linear_velocity_global  = PhxVec3(0.0f);
        m_angular_velocity_global = PhxVec3(0.0f);
    }
}

void PhxRigidBody::enableDamping(bool enable_damping)
{
    m_enable_damping = enable_damping;
}

void PhxRigidBody::setCanSleep(bool can_sleep)
{
    m_can_sleep = can_sleep;
    if(!m_can_sleep && !m_is_awake)
    {
        setAwake();
    }
}

void PhxRigidBody::setMass(const PhxReal& mass)
{
    // assert(fabsf(mass) > kPhxEpsilon);
    if(CMP_FLOAT_EQ(mass, 0.0f))
    {
        m_inv_mass = 0.0f;
    }
    else
    {
        m_inv_mass = 1.0f / mass;
    }
}

void PhxRigidBody::setCenterOfMass(const PhxVec3& center_of_mass)
{
    m_center_of_mass = center_of_mass;
}

// static void
// calculateInvInsertiaTensorInWorldSpace(const PhxMat3& inv_inertia_tensor_local,
//                                        const PhxMat3& inertia_tensor_local,
//                                        const PhxMat4& transform_matrix,
//                                        PhxMat3&       out_inertia_tensor_world,
//                                        PhxMat3& out_inv_inertia_tensor_world)

void PhxRigidBody::calculateDerivedData()
{
    m_orientation_world = glm::normalize(m_orientation_world);

    calculateTransformationMatrix(m_position_global, m_orientation_world, m_transform_matrix_global);

    calculateInvInsertiaTensorInWorldSpace(m_inv_inertia_tensor_local,
                                           m_inertia_tensor_local,
                                           m_transform_matrix_global,
                                           m_inertia_tensor_global,
                                           m_inv_inertia_tensor_global);
}

bool PhxRigidBody::hasFiniteMass() const
{
    return m_inv_mass > kPhxEpsilon;
    // return m_inv_mass >= 0.0f;
}

PhxReal PhxRigidBody::getMass() const
{
    if(CMP_FLOAT_EQ(m_inv_mass, 0.0f))
    {
        return 0.0f;
    }
    return ((PhxReal)1.0f) / m_inv_mass;
}

const PhxReal& PhxRigidBody::getInverseMass() const
{
    return m_inv_mass;
}

const PhxMat4& PhxRigidBody::getWorldTransform() const
{
    return m_transform_matrix_global;
}

PhxVec3 PhxRigidBody::getCenterOfMassInWorldSpace() const
{
    PhxVec4 point = PhxVec4(m_position_global, 0.0f) +
                    (m_orientation_world * PhxVec4(m_center_of_mass, 0.0f) * glm::inverse(m_orientation_world));
    return PhxVec3{point.x, point.y, point.z};
}

PhxVec3 PhxRigidBody::getCenterOfMassInLocalSpace() const
{
    return m_center_of_mass;
}

PhxVec3 PhxRigidBody::getPointInLocalSpace(const PhxVec3& point) const
{
    PhxVec3 result          = point - getCenterOfMassInWorldSpace();
    PhxQuat inv_orientation = phx_inv_quat(m_orientation_world);
    return phxRotatePoint(inv_orientation, result);
}

PhxVec3 PhxRigidBody::getPointInWorldSpace(const PhxVec3& point) const
{
    PhxVec3 result = getCenterOfMassInWorldSpace() + phxRotatePoint(m_orientation_world, point);
    return result;
}

const PhxMat3& PhxRigidBody::getInertiaTensorLocal() const
{
    return m_inertia_tensor_local;
}

const PhxMat3& PhxRigidBody::getInertiaTensorGlobal() const
{
    return m_inertia_tensor_global;
}

const PhxMat3& PhxRigidBody::getInverseInertiaTensorInWorldSpace() const
{
    return m_inv_inertia_tensor_global;
}

const PhxVec3& PhxRigidBody::getAcceleration() const
{
    return m_acceleration;
}

const PhxVec3& PhxRigidBody::getWorldPosition() const
{
    return m_position_global;
}

const PhxVec3& PhxRigidBody::getLinerVelocity() const
{
    return m_linear_velocity_global;
}

const PhxVec3& PhxRigidBody::getAngularVelocity() const
{
    return m_angular_velocity_global;
}

const PhxReal& PhxRigidBody::getElasticity() const
{
    return m_elasticity;
}

const PhxReal& PhxRigidBody::getFriction() const
{
    return m_friction;
}

void PhxRigidBody::applyImpulse(const PhxVec3& impluse, const PhxVec3& impluse_point, const PhxReal& dt)
{
    if(CMP_FLOAT_EQ(m_inv_mass, 0.0f))
    {
        return;
    }

    applyLinearImpulse(impluse, dt);

    PhxVec3 position        = getCenterOfMassInWorldSpace();
    PhxVec3 angular_impluse = phx_cross(impluse_point - position, impluse);
    applyAngularImpulse(angular_impluse, dt);
}

void PhxRigidBody::applyLinearImpulse(const PhxVec3& impluse, const PhxReal& dt)
{
    if(CMP_FLOAT_EQ(m_inv_mass, 0.0f))
    {
        return;
    }
    m_linear_velocity_global += impluse * m_inv_mass;

    if(m_enable_damping && !CMP_FLOAT_EQ(m_linear_damping, 0.0f))
    {
        m_linear_velocity_global *= std::pow(m_linear_damping, dt);
    }
}

void PhxRigidBody::applyAngularImpulse(const PhxVec3& implulse, const PhxReal& dt)
{
    if(CMP_FLOAT_EQ(m_inv_mass, 0.0f))
    {
        return;
    }
    m_angular_velocity_global += m_inv_inertia_tensor_global * implulse;

    if(m_enable_damping && !CMP_FLOAT_EQ(m_angular_damping, 0.0f))
    {
        m_angular_velocity_global *= std::pow(m_angular_damping, dt);
    }
}

// void PhxRigidBody::update(const PhxReal& dt)
//{
//     if(CMP_FLOAT_EQ(m_inv_mass, 0.0f))
//     {
//         return;
//     }
//
//     m_position_global += m_linear_velocity_global * dt;
//
//     PhxVec3 position_cm    = getCenterOfMassInWorldSpace();
//     PhxVec3 cm_to_position = m_position_global - position_cm;
//
//     PhxMat3 rot_mat = glm::mat3_cast(m_orientation_world);
//     PhxVec3 alpha =
//         m_inv_inertia_tensor_global *
//         (phx_cross(m_angular_velocity_global, glm::inverse(m_inv_inertia_tensor_global) *
//         m_angular_velocity_global));
//     m_angular_velocity_global += alpha * dt;
//
//     PhxVec3 delta_theta           = m_angular_velocity_global * dt;
//     PhxReal delta_theta_magnitude = phx_magnitude(delta_theta);
//     PhxQuat dq                    = glm::angleAxis(delta_theta_magnitude, delta_theta);
//
//     m_orientation_world = phx_normalize(dq * m_orientation_world);
//
//     m_position_global = position_cm + phxRotatePoint(dq, cm_to_position);
//
//     calculateDerivedData();
// }

void PhxRigidBody::update(const PhxReal& dt)
{
    if(CMP_FLOAT_EQ(m_inv_mass, 0.0f))
    {
        return;
    }

    m_position_global += m_linear_velocity_global * dt;

    PhxVec3 position_cm    = getCenterOfMassInWorldSpace();
    PhxVec3 cm_to_position = m_position_global - position_cm;

    PhxMat3 rot_mat = glm::mat3_cast(m_orientation_world);

    PhxVec3 alpha = m_inv_inertia_tensor_global *
                    (phx_cross(m_angular_velocity_global, m_inertia_tensor_global * m_angular_velocity_global));

    m_angular_velocity_global += alpha * dt;

    // m_angular_velocity_global *= PhxVec3{10.0f, 10.0f, 10.0f};
    // if(phx_magnitude_sq(m_angular_velocity_global) > 30.0f * 30.0f)
    //{
    //     m_angular_velocity_global = phx_normalize(m_angular_velocity_global) * 30.0f;
    // }

    // APPLICATION_CRITICAL("Angular velocity: {}, {}, {}",
    //                      m_angular_velocity_global.x,
    //                      m_angular_velocity_global.y,
    //                      m_angular_velocity_global.z);

    PhxVec3 delta_theta           = m_angular_velocity_global * dt;
    PhxReal delta_theta_magnitude = phx_magnitude(delta_theta);
    PhxQuat dq                    = glm::angleAxis(delta_theta_magnitude, delta_theta);

    m_orientation_world = phx_normalize(dq * m_orientation_world);

    m_position_global = position_cm + phxRotatePoint(dq, cm_to_position);

    calculateDerivedData();
}

} // namespace phx::rb