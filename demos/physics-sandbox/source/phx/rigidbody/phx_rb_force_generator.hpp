#ifndef PHX_RB_FORCE_GENERATOR_HPP
#define PHX_RB_FORCE_GENERATOR_HPP

#include "phx_rigid_body.hpp"

#include <vector>

namespace phx::rb
{

/*!
 * @brief Interface for rigidbody force generator.
 */
class PhxRbForceGenerator
{
public:
    PhxRbForceGenerator() noexcept          = default;
    virtual ~PhxRbForceGenerator() noexcept = default;

    virtual void updateForces(PhxRigidBody* const rigid_body, const PhxReal& duration) noexcept = 0;
};

class PhxRbGravityForceGen : public PhxRbForceGenerator
{
public:
    PhxRbGravityForceGen(const PhxVec3& gravity) noexcept;
    virtual ~PhxRbGravityForceGen() noexcept = default;

    virtual void updateForces(PhxRigidBody* const rigid_body, const PhxReal& duration) noexcept override;

private:

    PhxVec3 m_gravity;
};

class PhxRbSpringForceGen : public PhxRbForceGenerator
{
public:
    PhxRbSpringForceGen(const PhxVec3& connection_point,
                        PhxRigidBody*  other_body,
                        const PhxVec3& other_connection_point,
                        PhxReal        spring_constant,
                        PhxReal        rest_length) noexcept;
    virtual ~PhxRbSpringForceGen() noexcept = default;
    virtual void updateForces(PhxRigidBody* const rigid_body, const PhxReal& duration) noexcept override;

protected:
    /*!
     * @brief The point the spring is connected to on the body. The point is given in local space.
     */
    PhxVec3 m_connection_point;

    /*!
     * @brief The body at the other end of the spring.
     */
    PhxRigidBody* m_other_body;

    /*!
     * @brief The point the other end of the spring is connected to on the second body. The point is given in local
     * space.
     */
    PhxVec3 m_other_connection_point;

    /*!
     * @brief The spring constant.
     */
    PhxReal m_spring_constant;

    /*!
     * @brief Rest length of the spring
     */
    PhxReal m_rest_length;
};

class PhxRbForceRegistry
{
public:
    void add(PhxRigidBody* body, PhxRbForceGenerator* fgen);

    void remove(PhxRigidBody* body, PhxRbForceGenerator* fgen);

    void clear();

    void updateForces(PhxReal duration);

protected:
    struct PhxRbForceRegistration
    {
        PhxRigidBody*        body;
        PhxRbForceGenerator* generator;
    };

    using Registry = std::vector<PhxRbForceRegistration>;
    Registry m_registrations;
};

} // namespace phx::rb

#endif // !PHX_RB_FORCE_GENERATOR_HPP