#pragma once

#include <precision.h>
#include "particle.h"

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
    virtual void updateForce(Particle* particle, const real& duration) = 0;
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
    virtual void updateForce(Particle* particle, const real& duration) override;

private:
    /**
     * @brief Holds the acceleration due to gravity.
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
    virtual void updateForce(Particle* particle, const real& duration) override;

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

/**
 * @brief Spring force generator for a pair of particles, each attached to either end of the spring.
 */
class ParticleSpringForceGenerator : public ParticleForceGenerator
{
public:
    ParticleSpringForceGenerator(Particle* other, const real& spring_constant, const real& rest_length) noexcept;

    virtual ~ParticleSpringForceGenerator() noexcept = default;

    /*!
     * @brief This method applies the spring force to the given particles.
     *
     * @param particle The particle to apply the force to.
     * @param duration The duration of the frame in seconds.
     */
    virtual void updateForce(Particle* particle, const real& duration) override;

protected:
    /**
     *  The particle at the other end of the sprinf.
     */
    Particle* m_other_particle;

    /**
     * Holds the spring constant (gives the spring its stiffness).
     */
    real m_spring_constant;

    /**
     * Holds the rest length of the spring.
     */
    real m_rest_length;
};

/**
 * @brief An anchored spring force generator for a particle attached to a fixed point in space via a spring.
 */
class ParticleAnchoredSpringForceGenerator : public ParticleForceGenerator
{
public:
    ParticleAnchoredSpringForceGenerator(const vec3& anchor,
                                         const real& spring_constant,
                                         const real& rest_length) noexcept;

    virtual ~ParticleAnchoredSpringForceGenerator() noexcept = default;

    /*!
     * @brief This method applies the spring force to the given particle.
     *
     * @param particle The particle to apply the force to.
     * @param duration The duration of the frame in seconds.
     */
    virtual void updateForce(Particle* particle, const real& duration) override;

    virtual void setAnchor(const vec3& anchor) noexcept;

protected:
    /**
     * Holds the location of the anchored end of the spring.
     */
    vec3 m_anchor;

    /**
     * Holds the spring constant (gives the spring its stiffness).
     */
    real m_spring_constant;

    /**
     * Holds the rest length of the spring.
     */
    real m_rest_length;
};

/**
 * @brief A bungee force generator for a pair of particles, each attached to either end of the bungee. The bungee acts
 * as a spring only when the length of the bungee is greater than the rest length.
 */
class ParticleBungeeForceGenerator : public ParticleForceGenerator
{
public:
    ParticleBungeeForceGenerator(Particle* other, const real& spring_constant, const real& rest_length) noexcept;

    virtual ~ParticleBungeeForceGenerator() noexcept = default;

    /*!
     * @brief This method applies the bungee force to the given particles.
     *
     * @param particle The particle to apply the force to.
     * @param duration The duration of the frame in seconds.
     */
    virtual void updateForce(Particle* particle, const real& duration) override;

protected:
    /*
     * The particle at the other end of the bungee.
     */
    Particle* m_other_particle;

    /*
     * Holds the spring constant (gives the spring its stiffness).
     */
    real m_spring_constant;

    /*
     * Holds the rest length of the spring.
     */
    real m_rest_length;
};

class ParticleAnchoredBungeeForceGenerator : public ParticleAnchoredSpringForceGenerator
{
public:
    ParticleAnchoredBungeeForceGenerator(const vec3& anchor,
                                         const real& spring_constant,
                                         const real& rest_length) noexcept;

    virtual ~ParticleAnchoredBungeeForceGenerator() noexcept = default;

    /*!
     * @brief This method applies the spring force to the given particle.
     *
     * @param particle The particle to apply the force to.
     * @param duration The duration of the frame in seconds.
     */
    virtual void updateForce(Particle* particle, const real& duration) override;
};

/**
 * @brief A force generator that applies a buoyancy force for a plane of liquid parallel to XY plane.
 */
class ParticleBuoyancyForceGenerator : public ParticleForceGenerator
{
public:
    ParticleBuoyancyForceGenerator(const real& max_depth,
                                   const real& volume,
                                   const real& liquid_height,
                                   const real& liquid_density = 1000.0f) noexcept;

    virtual ~ParticleBuoyancyForceGenerator() noexcept = default;

    /*!
     * @brief This method applies the buoyancy force to the given particle.
     *
     * @param particle The particle to apply the force to.
     * @param duration The duration of the frame in seconds.
     */
    virtual void updateForce(Particle* particle, const real& duration) override;

protected:
    /**
     * @brief Holds the maximum submersion depth of the object before it generates its maximum buoyancy force.
     *
     * Particles have zero volume. Therefore, we cannot determine the submersion depth of a particle. However, we get
     * around this by using a fixed depth. When calculating the buoyancy force, we give a depth at which the object is
     * considered fully submerged. Beyond this depth, the buoyancy force will not increase any more.
     */
    real m_max_depth;

    /**
     * @brief Holds the volume of the object.
     */
    real m_volume;

    /**
     * @brief Holds the height of the liquid plane above y = 0. The plane will be parallel to the XZ plane.
     */
    real m_liquid_height;

    /**
     * @brief Holds the density of the liquid. Pure water has a density of 1000 kg per cubic meter.
     */
    real m_liquid_density;
};

} // namespace sputnik::physics
