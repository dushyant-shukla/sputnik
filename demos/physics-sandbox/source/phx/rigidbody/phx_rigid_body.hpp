#ifndef PHX_RB_RIGID_BODY_HPP
#define PHX_RB_RIGID_BODY_HPP

#include "../phx_types.hpp"

namespace phx::rb
{

class PhxRigidBody
{

public:
    PhxRigidBody() = default;

    virtual ~PhxRigidBody() = default;

    /*!
     * @brief Adds a force to the rigid body. The force is expressed in world space. This force is applied to the center
     * of mass of the body, thus, has no torque component.
     * @param force The force to be applied to the body.
     */
    void addForce(const PhxVec3& force);

    /*!
     * @brief Clears the accumulated forces and torques on the rigid body. This method is called after each integration
     * step.
     */
    void clearAccumulators();

    /*!
     * @brief Integrates the rigid body forward in time by the given duration. This method uses Newton-Euler integration
     * to update the position and orientation of the rigid body.
     * @param duration The duration for which to integrate the rigid body.
     */
    void integrate(PhxReal duration);

    /*!
     * @brief Adds a force to the rigid body at a given point. The force and the point are expressed in world space.
     * This force is not applied necessarily being applied to the center of mass of the body, it'll have a torque
     * component to it as well.
     * @param force The force to be applied to the body (in world space).
     * @param point The point at which the force is applied (in world space).
     */
    void addForceAtPoint(const PhxVec3& force, const PhxVec3& point);

    /*!
     * @brief Adds a force to the rigid body at a given point. The force is expressed in world space. The application
     * point is defined in the body's local space.
     *
     * @details This method is useful when we'have a sprint attached to a fixed point on the body.
     *
     * @param force The force to be applied to the body (in world space).
     * @param point The point at which the force is applied (in body's local space).
     */
    void addForceAtBodyPoint(const PhxVec3& force, const PhxVec3& point);

    /*!
     * @brief Sets a rigid body awake.
     * @param awake
     */
    void setAwake(bool awake = true);

    void setCanSleep(bool can_sleep);

    void setMass(const PhxReal& mass);

    void setInertiaTensor(const PhxMat3& inertia_tensor);

    void setDamping(const PhxReal& linear_damping, const PhxReal& angular_damping);

    void setAcceleration(const PhxVec3& acceleration);

    void setPosition(const PhxVec3& position);

    void setVelocity(const PhxVec3& position);

    void setRotation(const PhxVec3& velocity);

    void setOrientation(const PhxQuat& orientation);

    void setOrientation(const PhxReal& w, const PhxReal& x, const PhxReal& y, const PhxReal& z);

    void setInertiaTensorWithHalfSizesAndMass(const PhxVec3& half_sizes, const PhxReal& mass);

    void setInertiaTensorWithCoefficients(const PhxReal& ix,
                                          const PhxReal& iy,
                                          const PhxReal& iz,
                                          const PhxReal& ixy = 0.0f,
                                          const PhxReal& ixz = 0.0f,
                                          const PhxReal& iyz = 0.0f);

    void setElasticity(const PhxReal& elasticity);

    /*!
     * @brief Calculate internal data from rigid body state. This method must be called anytime rigibody's state is
     * modified directly. It is also called during integration.
     */
    void calculateDerivedData();

    bool hasFiniteMass() const;

    PhxReal getMass() const;

    const PhxReal& getInverseMass() const;

    const PhxMat4& getWorldTransform() const;

    PhxVec3 getCenterOfMassInWorldSpace() const;

    PhxVec3 getCenterOfMassInLocalSpace() const;

    PhxVec3 getPointInLocalSpace(const PhxVec3& point) const;

    PhxVec3 getPointInWorldSpace(const PhxVec3& point) const;

    const PhxVec3& getAcceleration() const;

    const PhxVec3& getWorldPosition() const;

    const PhxVec3& getLinerVelocity() const;

    const PhxReal& getElasticity() const;

    void applyLinearImpluse(const PhxVec3& impluse);

protected:
    /*!
     * @brief Inverse mass of the rigid body. If the inverse mass is zero, the body is considered to have infinite mass.
     */
    PhxReal m_inv_mass;

    /*!
     * @brief Center of mass of the rigid body. This is the point about which all forces and torques are applied. It is
     * stored in local space.
     */
    PhxVec3 m_center_of_mass;

    /*!
     * @brief Damping applied to linear motion. Damping is required to remove extra energy from the system
     * resulting from instabilities in the numerical integration.
     */
    PhxReal m_linear_damping;

    /*!
     * @brief Dampling applied to angular motion. Damping is required to remove extra energy from the system resulting
     * from instabilities in the numerical integration.
     */
    PhxReal m_angular_damping;

    /*!
     * @brief Also known as the coefficient of restitution. It is a measure of how much kinetic energy is preserved in a
     * collision. It is a value between 0 and 1. A value of 0 means that the body will not bounce at all (all of the
     * kinetic energy is lost), while a value of 1 means that the body will bounce with the same energy it had before
     * the collision (no loss of kinetic energy).
     */
    PhxReal m_elasticity;

    /*!
     * @brief Linear position of the rigid body in world space.
     */
    PhxVec3 m_position_world;

    /*!
     * @brief Angular orientation of the rigid body in world space.
     */
    PhxQuat m_orientation_world;

    /*!
     * @brief Linear velocity of the rigid body in world space.
     */
    PhxVec3 m_velocity_world;

    /*!
     * @brief Angular velocity (or rotation) of the rigid body in world space.
     */
    PhxVec3 m_angular_velocity_world;

    /*!
     * @brief Holds the amount of motion of the body. The motion is a measure of how much the body is moving. It is a
     * recency weighted mean used to put the body to sleep.
     */
    PhxReal m_motion;

    /*!
     * @brief The transformation matrix for the rigid body. It is derived from the position and orientation of the body
     * once per frame.
     */
    PhxMat4 m_transform_matrix_world;

    /*!
     * @brief The inverse inertia tensor of the body in world space. The inverse inertia tensor is used to convert
     * angular acceleration into torque and vice versa. The inertia tensor is stored in rigidbody's local space.
     *
     * @details The moment of inertia is roughly the rotational analog of mass. It is a measure of how difficult it is
     * to change the rotational motion of an object. However, unlike mass, it depends, on how the body is spinning. Each
     * axis of rotation gets its own moment of inertia. The inertia tensor is a 3x3 matrix characteristic of a rigidbody
     * that compactly desribes the moment of inertia about body's local axes of rotation.
     */
    PhxMat3 m_inv_inertia_tensor_local;

    /*!
     * @brief The inverse inertia tensor of the body in world space.
     */
    PhxMat3 m_inv_inertia_tensor_world;

    /*!
     * @brief Holds the acceleration of the rigid body. This can be used to set acceleration due to gravity or any other
     * constant and persistent acceleration.
     */
    PhxVec3 m_acceleration;

    /*!
     * @brief Holds the acceleration of the rigid body during the last frame.
     */
    PhxVec3 m_acceleration_last_frame;

    /*!
     * @brief Stores the accumulated force on the rigid body. It's cleared after each integration step.
     */
    PhxVec3 m_accumulated_force;

    /*!
     * @brief Stores the accumulated torque on the rigid body. It's cleared after each integration step.
     */
    PhxVec3 m_accumulated_torque;

    /*!
     * @brief A body can be put to sleep to avoid unnecessary computations. A sleeping body is not simulated by the
     * physics.
     */
    bool m_is_awake;

    /*!
     * @brief Some bodies should never go to sleep
     */
    bool m_can_sleep;
};

} // namespace phx::rb

#endif // !PHX_RB_RIGID_BODY_HPP