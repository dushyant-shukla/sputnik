#pragma once

#include <vector3.h>

namespace sputnink::physics
{

using namespace ramanujan;

/*!
 * @brief A particle is the simplest entity that can be simul ated in a physics engine.
 */
class Particle
{

public:
    /*!
     * @brief Integrates the particle forward in time by the given amount.This function uses a Newton-Euler integration
     * method, which is a linear approximation to the correct integral. For this reason it may be inaccurate in some
     * cases.
     *
     * @param duration The frame time delta.
     */
    void integrate(real duration);

protected:
    /*!
     * @brief Holds the linear position of the particle in world space
     */
    Vector3 m_position;

    /*!
     * @brief Holds the linear velocity of the particle in world space
     */
    Vector3 m_velocity;

    /*!
     * @brief Holds the acceleration of the particle. This value can be used to set acceleration due to gravity, or any
     * other constant acceleration.
     */
    Vector3 m_acceleration;

    /*!
     * @brief Holds the amount of dampig applied lineat motion. Damping is required to remove energy added through
     * numerical instablity in the integrator.
     */
    real m_damping;

    /*!
     * @brief Holds the inverse of the mass of the particle. It is more useful to hold the inverse mass because
     * integration is simpler, and because in real-time simulation it is more useful to have entities with infinite mass
     * (immovable) than zero mass (completely unstable in numerical simulation)
     */
    real m_inv_mass;
};
} // namespace sputnink::physics
