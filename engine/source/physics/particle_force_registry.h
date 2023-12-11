#pragma once

#include "particle.h"
#include "particle_force_generator.h"

#include <vector>

namespace sputnik::physics
{

struct ParticleForceRegistration
{
    Particle*               m_particle;
    ParticleForceGenerator* m_fg;

    ParticleForceRegistration(Particle* particle, ParticleForceGenerator* fg) : m_particle(particle), m_fg(fg) {}
};

class ParticleForceRegistry
{

public:
    /*!
     * @brief Registers the given force generator to apply to the given particle.
     *
     * @param particle The particle to apply the force to.
     * @param fg The force generator that will be applied.
     */
    void add(Particle* particle, ParticleForceGenerator* fg);

    /*!
     * @brief Removes the given registered pair from the registry.
     *
     * If the pair is not registered, this method will have no effect.
     *
     * @param particle The particle to remove.
     * @param fg The force generator to remove.
     */
    void remove(Particle* particle, ParticleForceGenerator* fg);

    /*!
     * @brief Clears all registrations from the registry.
     */
    void clear();

    /*!
     * @brief Calls all the force generators to update the forces of their corresponding particles.
     *
     * @param duration The duration of the frame in seconds.
     */
    void updateForces(const real& duration);

protected:
    std::vector<ParticleForceRegistration> m_registrations;
};

} // namespace sputnik::physics
