#include "pch.h"

#include "particle_force_generator.h"

namespace sputnik::physics
{

////////////// Gravity Force Generator //////////////////

ParticleGravityForceGenerator::ParticleGravityForceGenerator(const vec3& gravity) noexcept : m_gravity(gravity) {}

void ParticleGravityForceGenerator::updateForce(Particle& particle, const real& duration)
{
    if(!particle.hasFiniteMass())
    {
        return;
    }

    particle.addForce(m_gravity * particle.getMass());
}

////////////// Gravity Force Generator Ends //////////////////

////////////// Drag Force Generator //////////////////

/**
 * Drag is a force that acts on a particle and depends on its velocity. It is a force that is opposite to the velocity
 * of the particle.
 */
ParticleDragForceGenerator::ParticleDragForceGenerator(const real& k1, const real& k2) noexcept : m_k1(k1), m_k2(k2) {}

void ParticleDragForceGenerator::updateForce(Particle& particle, const real& duration)
{
    vec3 velocity;
    particle.getVelocity(velocity);

    real velotiy_magnitude = velocity.magnitude();
    real total_force       = m_k1 * velotiy_magnitude + m_k2 * velotiy_magnitude * velotiy_magnitude;

    velocity.normalize();
    velocity *= -total_force;
    particle.addForce(velocity);
}

///////////////// Drag Force Generator Ends //////////////////

} // namespace sputnik::physics
