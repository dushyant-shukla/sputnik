#include "pch.h"

#include "particle_force_generator.h"

namespace sputnik::physics
{

////////////// Gravity Force Generator //////////////////

ParticleGravityForceGenerator::ParticleGravityForceGenerator(const vec3& gravity) noexcept : m_gravity(gravity) {}

void ParticleGravityForceGenerator::updateForce(Particle* particle, const real& duration)
{
    if(!particle->hasFiniteMass())
    {
        return;
    }

    particle->addForce(m_gravity * particle->getMass());
}

////////////// Gravity Force Generator Ends //////////////////

////////////// Drag Force Generator //////////////////

/**
 * Drag is a force that acts on a particle and depends on its velocity. It is a force that is opposite to the velocity
 * of the particle.
 */
ParticleDragForceGenerator::ParticleDragForceGenerator(const real& k1, const real& k2) noexcept : m_k1(k1), m_k2(k2) {}

void ParticleDragForceGenerator::updateForce(Particle* particle, const real& duration)
{
    vec3 velocity;
    particle->getVelocity(velocity);

    real velotiy_magnitude = velocity.magnitude();
    real total_force       = m_k1 * velotiy_magnitude + m_k2 * velotiy_magnitude * velotiy_magnitude;

    velocity.normalize();
    velocity *= -total_force;
    particle->addForce(velocity);
}

///////////////// Drag Force Generator Ends //////////////////

////////////// Spring Force Generator //////////////////

ParticleSpringForceGenerator::ParticleSpringForceGenerator(Particle*   other_particle,
                                                           const real& spring_constant,
                                                           const real& rest_length) noexcept
    : m_other_particle(other_particle)
    , m_spring_constant(spring_constant)
    , m_rest_length(rest_length)
{
}

void ParticleSpringForceGenerator::updateForce(Particle* particle, const real& duration)
{
    // spring force is calculated as F = -k (|d| - l) (d / |d|)
    // where,
    // k = spring constant
    // d = (Xa - Xb) [distance vector between the two particles]
    // Xa and Xb are the current positions of the current and other particles respectively

    vec3 current_particle_position;
    particle->getPosition(current_particle_position);

    vec3 other_particle_position;
    m_other_particle->getPosition(other_particle_position);

    vec3 distance_vector       = current_particle_position - other_particle_position;
    real current_spring_length = distance_vector.length();
    real delta_spring_length   = real_abs(current_spring_length - m_rest_length);

    vec3 force = -m_spring_constant * delta_spring_length * distance_vector.normalize();
    particle->addForce(force);
}

////////////// Spring Force Generator Ends //////////////////

////////////// Anchored Spring Force Generator //////////////////

ParticleAnchoredSpringForceGenerator::ParticleAnchoredSpringForceGenerator(const vec3& anchor,
                                                                           const real& spring_constant,
                                                                           const real& rest_length) noexcept
    : m_anchor(anchor)
    , m_spring_constant(spring_constant)
    , m_rest_length(rest_length)
{
}

void ParticleAnchoredSpringForceGenerator::updateForce(Particle* particle, const real& duration)
{
    // spring force is calculated as F = -k (|d| - l) (d / |d|)
    // where,
    // k = spring constant
    // d = (Xa - Xb) [distance vector between the two particles]
    // Xa and Xb are the current positions of the current particle and the anchored position respectively

    vec3 current_particle_position;
    particle->getPosition(current_particle_position);

    vec3 distance_vector       = current_particle_position - m_anchor;
    real current_spring_length = distance_vector.length();
    real delta_spring_length   = real_abs(current_spring_length - m_rest_length);

    vec3 force = -m_spring_constant * delta_spring_length * distance_vector.normalize();
    particle->addForce(force);
}

void ParticleAnchoredSpringForceGenerator::setAnchor(const vec3& anchor) noexcept
{
	m_anchor = anchor;
}

////////////// Anchored Spring Force Generator Ends //////////////////

////////////// Bungee Force Generator //////////////////

ParticleBungeeForceGenerator::ParticleBungeeForceGenerator(Particle*   other,
                                                           const real& spring_constant,
                                                           const real& rest_length) noexcept
    : m_other_particle(other)
    , m_spring_constant(spring_constant)
    , m_rest_length(rest_length)
{
}

void ParticleBungeeForceGenerator::updateForce(Particle* particle, const real& duration)
{
    // spring force is calculated as F = -k (|d| - l) (d / |d|)
    // where,
    // k = spring constant
    // d = (Xa - Xb) [distance vector between the two particles]
    // Xa and Xb are the current positions of the current and other particles respectively

    vec3 current_particle_position;
    particle->getPosition(current_particle_position);

    vec3 other_particle_position;
    m_other_particle->getPosition(other_particle_position);

    vec3 distance_vector       = current_particle_position - other_particle_position;
    real current_spring_length = distance_vector.length();

    // if the current spring length is less than the rest length, then the spring is not stretched and hence no force
    if(current_spring_length <= m_rest_length)
    {
        return;
    }

    vec3 force = -m_spring_constant * (current_spring_length - m_rest_length) * distance_vector.normalize();
    particle->addForce(force);
}

////////////// Bungee Force Generator Ends //////////////////

////////////// Anchored Bungee Force Generator //////////////////

ParticleAnchoredBungeeForceGenerator::ParticleAnchoredBungeeForceGenerator(const vec3& anchor,
    const real& spring_constant,
    const real& rest_length) noexcept
    : ParticleAnchoredSpringForceGenerator(anchor, spring_constant, rest_length)
{
}

void ParticleAnchoredBungeeForceGenerator::updateForce(Particle* particle, const real& duration)
{
    // spring force is calculated as F = -k (|d| - l) (d / |d|)
    // where,
    // k = spring constant
    // d = (Xa - Xb) [distance vector between the two particles]
    // Xa and Xb are the current positions of the current particle and the anchored position respectively

    vec3 current_particle_position;
    particle->getPosition(current_particle_position);

    vec3 distance_vector       = current_particle_position - m_anchor;
    real current_spring_length = distance_vector.length();

    // if the current spring length is less than the rest length, then the spring is not stretched and hence no force
    if(current_spring_length <= m_rest_length)
    {
        return;
    }

    vec3 force = -m_spring_constant * (current_spring_length - m_rest_length) * distance_vector.normalized();
    particle->addForce(force);
}

////////////// Anchored Bungee Force Generator Ends //////////////////

////////////// Buoyancy Force Generator //////////////////

ParticleBuoyancyForceGenerator::ParticleBuoyancyForceGenerator(const real& max_depth,
                                                               const real& volume,
                                                               const real& liquid_height,
                                                               const real& liquid_density) noexcept
    : m_max_depth(max_depth)
    , m_volume(volume)
    , m_liquid_height(liquid_height)
    , m_liquid_density(liquid_density)
{
}

void ParticleBuoyancyForceGenerator::updateForce(Particle* particle, const real& duration)
{
    // In real life, force of buoyancy is calculated as Fb = -pVg,
    // where,
    // p is the density of the liquid,
    // V is the volume of the object, and
    // g is the gravity vector

    // Force of buoyancy is equivalent to the weight of the liquid displaced by the object.
    // However, we can simulate this force using spring-like force as an approximation.
    // The force is proportional to the depth to which the object is submerged in the liquid, just as the spring force
    // is proportional to the extension or compression of the spring.

    // The force of buoyancy is given by:
    // Fb = 0, if d <= 0
    // Fb = pv, if d >= 1
    // Fb = dpv, otherwise
    // where,
    // p is the density of the liquid
    // v is the volume of the object
    // d is the depth to which the object is submerged in the liquid, calculated as a proportion of its maximum depth
    // given by, d = (y - h - s) / (2 * s), where,
    // y is the height of the object
    // h is the height of the liquid surface
    // s is the maximum submersion depth of the object
    // d is clamped to the range [0 (fully out of the liquid), 1 (fully submerged)],

    // NOTE: The calculation assumes the plane of the liquid is parallel to the XZ plane.

    real current_depth = particle->getPosition().y;

    // if the particle is above the liquid, then no force
    if(current_depth >= m_liquid_height + m_max_depth)
    {
        return;
    }

    vec3 force{};

    // if the particle is completely submerged in the liquid
    if(current_depth <= m_liquid_height - m_max_depth)
    {
        force.y = m_liquid_density * m_volume;
    }
    else
    {
        // if the particle is partially submerged in the liquid
        force.y = m_liquid_density * m_volume * (current_depth - m_max_depth - m_liquid_height) / 2 * m_max_depth;
        particle->addForce(force);
    }
    particle->addForce(force);
}

////////////// Buoyancy Force Generator Ends //////////////////

} // namespace sputnik::physics
