#pragma once

#include "particle.h"

#include <precision.h>
#include <vector.hpp>

#include <memory>

namespace sputnik::physics
{

using namespace ramanujan;

class ParticleForceGenerator
{

public:
    ParticleForceGenerator() noexcept          = default;
    virtual ~ParticleForceGenerator() noexcept = default;

    /*!
     * @brief Overload this in implementations of the interface to calculate and update the force applied to the given
     * particle.
     *
     * @param particle The particle to apply the force to.
     * @param duration The duration of the frame in seconds.
     */
    virtual void updateForce(Particle& particle, const real& duration) = 0;
};

/**
 * @brief Gravity force generator for particles.
 */
class ParticleGravityForceGenerator : public ParticleForceGenerator
{
public:
    ParticleGravityForceGenerator(const vec3& gravity) noexcept;
    virtual ~ParticleGravityForceGenerator() noexcept = default;

    /*!
     * @brief This method applies the force of gravity to the given particle.
     *
     * @param particle The particle to apply the force to.
     * @param duration The duration of the frame in seconds.
     */
    virtual void updateForce(Particle& particle, const real& duration) override;

private:
    /**
     * Holds the acceleration due to gravity.
     */
    vec3 m_gravity;
};

/**
 * @brief Drag force generator for particles.
 */
class ParticleDragForceGenerator : public ParticleForceGenerator
{
public:
    ParticleDragForceGenerator(const real& k1, const real& k2) noexcept;
    virtual ~ParticleDragForceGenerator() noexcept = default;

    /*!
     * @brief This method applies the drag force to the given particles.
     *
     * @param particle The particle to apply the force to.
     * @param duration The duration of the frame in seconds.
     */
    virtual void updateForce(Particle& particle, const real& duration) override;

private:
    /**
     * Velocity drag coefficients. These constants categorise how strong the drag force is.
     */
    real m_k1;

    /**
     * Velocity squarred drag coefficients. The drag force is proportional to the square of the velocity of the
     * particle. When a drag has a non-zero velocity squared drag coefficient, the drag will grow faster at higher
     * speeds. For every doubling of the velocity, the drag almost quadruples.
     */
    real m_k2;
};

} // namespace sputnik::physics
