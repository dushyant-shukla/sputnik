#include "phx_rb_force_generator.hpp"

namespace phx::rb
{

////////////////////////////// Gravity Force Generator //////////////////////////////

PhxRbGravityForceGen::PhxRbGravityForceGen(const PhxVec3& gravity) noexcept : m_gravity(gravity) {}

void PhxRbGravityForceGen::updateForces(PhxRigidBody* const rigid_body, const PhxReal& duration) noexcept
{
    if(!rigid_body->hasFiniteMass())
    {
        return;
    }

    rigid_body->addForce(m_gravity * rigid_body->getMass());
}

////////////////////////////// Spring Force Generator //////////////////////////////

PhxRbSpringForceGen::PhxRbSpringForceGen(const PhxVec3& connection_point,
                                         PhxRigidBody*  other_body,
                                         const PhxVec3& other_connection_point,
                                         PhxReal        spring_constant,
                                         PhxReal        rest_length) noexcept
{
}

void PhxRbSpringForceGen::updateForces(PhxRigidBody* const rigid_body, const PhxReal& duration) noexcept {}

//////////////////////////// Force Registry /////////////////////////////////////

void PhxRbForceRegistry::add(PhxRigidBody* body, PhxRbForceGenerator* fgen)
{
    PhxRbForceRegistration registration{};
    registration.body      = body;
    registration.generator = fgen;
    m_registrations.push_back(registration);
}

void PhxRbForceRegistry::remove(PhxRigidBody* body, PhxRbForceGenerator* fgen) {}

void PhxRbForceRegistry::clear() {}

void PhxRbForceRegistry::updateForces(PhxReal duration)
{
    for(auto itr = m_registrations.begin(); itr != m_registrations.end(); ++itr)
    {
        itr->generator->updateForces(itr->body, duration);
    }
}

} // namespace phx::rb