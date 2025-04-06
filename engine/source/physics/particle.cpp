#include "pch.h"
#include "particle.h"

#include <assert.h>

namespace sputnik::physics
{

void Particle::integrate(const real& duration) noexcept
{
    // We don't integrate things with infinite mass.
    if(m_inv_mass <= Constants::EPSILON)
        return;

    assert(duration > 0.0f);

    //// Update linear position
    //m_position += m_velocity * duration;

    //// Work out the acceleration from the force. We'll add to this vector when we come to generate forces.
    //vec3 total_acceleration = m_acceleration;
    //total_acceleration += m_accumulated_force * m_inv_mass;

    //// Update linear velocity from the acceleration.
    //m_velocity += total_acceleration * duration;

    //// Impose drag
    //m_velocity *= real_pow(m_damping, duration);

    //////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////

    ///// damping experiment
    // Update linear position

    // https://gamedev.stackexchange.com/questions/169558/how-can-i-fix-my-velocity-damping-to-work-with-any-delta-frame-time
    m_position += m_velocity * (std::pow(m_damping, duration) - 1.0f) / std::log(m_damping);

    // Work out the acceleration from the force. We'll add to this vector when we come to generate forces.
    vec3 total_acceleration = m_acceleration;
    total_acceleration += m_accumulated_force * m_inv_mass;

    // Update linear velocity from the acceleration.
    m_velocity += total_acceleration * duration;

    // Impose drag
    m_velocity *= std::pow(m_damping, duration);


    // Clear the forces
    clearAccumulator();
}

void Particle::setMass(const real& mass) noexcept
{
    assert(mass > 0.0f);
    m_inv_mass = 1.0f / mass;
}

real Particle::getMass() const noexcept
{
    if(m_inv_mass == 0)
    {
        return kRealMax;
    }
    else
    {
        return 1.0f / m_inv_mass;
    }
}

void Particle::setInverseMass(const real& inverse_mass) noexcept
{
    m_inv_mass = inverse_mass;
}

const real& Particle::getInverseMass() const noexcept
{
    return m_inv_mass;
}

bool Particle::hasFiniteMass() const noexcept
{
    return m_inv_mass >= 0.0f;
}

void Particle::setDamping(const real& damping) noexcept
{
    m_damping = damping;
}

real Particle::getDamping() const noexcept
{
    return m_damping;
}

void Particle::setPosition(const vec3& position) noexcept
{
    m_position = position;
}

void Particle::setPosition(const real& x, const real& y, const real& z) noexcept
{
    m_position.x = x;
    m_position.y = y;
    m_position.z = z;
}

const vec3& Particle::getPosition() const noexcept
{
    return m_position;
}

void Particle::getPosition(vec3& position) const noexcept
{
    position = m_position;
}

void Particle::setVelocity(const vec3& velocity) noexcept
{
    m_velocity = velocity;
}

void Particle::setVelocity(const real& x, const real& y, const real& z) noexcept
{
    m_velocity.x = x;
    m_velocity.y = y;
    m_velocity.z = z;
}

const vec3& Particle::getVelocity() const noexcept
{
    return m_velocity;
}

void Particle::getVelocity(vec3& velocity) const noexcept
{
    velocity = m_velocity;
}

void Particle::setAcceleration(const vec3& acceleration) noexcept
{
    m_acceleration = acceleration;
}

void Particle::setAcceleration(const real& x, const real& y, const real& z) noexcept
{
    m_acceleration.x = x;
    m_acceleration.y = y;
    m_acceleration.z = z;
}

const vec3& Particle::getAcceleration() const noexcept
{
    return m_acceleration;
}

void Particle::getAcceleration(vec3& acceleration) const noexcept
{
    acceleration = m_acceleration;
}

void Particle::clearAccumulator() noexcept
{
    m_accumulated_force.clear();
}

void Particle::addForce(const vec3& force) noexcept
{
    m_accumulated_force += force;
}

} // namespace sputnik::physics