#pragma once

#include <vector.hpp>

namespace sputnik::physics
{

using namespace ramanujan;
using namespace ramanujan::experimental;

/*!
 * @brief A particle is the simplest entity that can be simulated in a physics engine.
 */
class Particle
{

public:
    Particle() noexcept = default;

    virtual ~Particle() noexcept = default;

    /*!
     * @brief Integrates the particle forward in time by the given amount.This function uses a Newton-Euler integration
     * method, which is a linear approximation to the correct integral. For this reason it may be inaccurate in some
     * cases.
     *
     * @param duration The frame time delta.
     */
    void integrate(const real& duration) noexcept;

    /*!
     * @brief Sets the mass of the particle using the given parameter.
     *
     * The mass cannot be zero. Small masses can produce unstable rigid bodies under simulation.
     *
     * This invalidates internal data for the particle. Either an integration function, or the calculateInternals
     * function should be called before trying to get any settings from the particle.
     *
     *
     * @param mass The new mass of the body.
     */
    void setMass(const real& mass) noexcept;

    /*!
     * @brief Gets the mass of the particle.
     *
     * @return The current mass of the particle.
     */
    real getMass() const noexcept;

    /*!
     * @brief .
     *
     * @param inverse_mass
     * @return
     */
    void setInverseMass(const real& inverse_mass) noexcept;

    /*!
     * @brief .
     *
     * @return
     */
    const real& getInverseMass() const noexcept;

    /*!
     * @brief Use this method to check if the particle has a finite mass.
     *
     * @return True if the mass of the particle is not-infinite.
     */
    bool hasFiniteMass() const noexcept;

    /*!
     * @brief .
     *
     * @param damping
     */
    void setDamping(const real& damping) noexcept;

    /*!
     * @brief .
     *
     * @return
     */
    real getDamping() const noexcept;

    /*!
     * @brief .
     *
     * @param position
     */
    void setPosition(const vec3& position) noexcept;

    void setPosition(const real& x, const real& y, const real& z) noexcept;

    const vec3& getPosition() const noexcept;

    void getPosition(vec3& position) const noexcept;

    void setVelocity(const vec3& velocity) noexcept;

    void setVelocity(const real& x, const real& y, const real& z) noexcept;

    const vec3& getVelocity() const noexcept;

    void getVelocity(vec3& velocity) const noexcept;

    void setAcceleration(const vec3& acceleration) noexcept;

    void setAcceleration(const real& x, const real& y, const real& z) noexcept;

    const vec3& getAcceleration() const noexcept;

    void getAcceleration(vec3& acceleration) const noexcept;

    /*!
     * @brief .
     *
     * @param force
     */
    void addForce(const vec3& force) noexcept;

    void clearAccumulator() noexcept;

protected:

protected:
    /*
     * @brief Holds the linear position of the particle in world space
     */
    vec3 m_position;

    /*
     * @brief Holds the linear velocity of the particle in world space
     */
    vec3 m_velocity;

    /*
     * @brief Holds the acceleration of the particle. This value can be used to set acceleration due to gravity, or any
     * other constant acceleration.
     */
    vec3 m_acceleration;

    /*
     * @brief The accumulated force to be applied at the next simulation iteration only. This value is zeroed at each
     * integration step.
     */
    vec3 m_accumulated_force;

    /*
     * @brief Holds the amount of dampig applied lineat motion. Damping is required to remove energy added through
     * numerical instablity in the integrator.
     */
    real m_damping;

    /*
     * @brief Holds the inverse of the mass of the particle. It is more useful to hold the inverse mass because
     * integration is simpler, and because in real-time simulation it is more useful to have entities with infinite mass
     * (immovable) than zero mass (completely unstable in numerical simulation)
     */
    real m_inv_mass;
};
} // namespace sputnik::physics
