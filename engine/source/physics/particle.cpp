#include "pch.h"
#include "particle.h"

namespace sputnink::physics
{

void Particle::integrate(real duration)
{
    // We don't integrate things with infinite mass.
    if(m_inv_mass <= Constants::EPSILON)
        return;

    // Update linear position
    m_position = addScaledVector(m_position, m_velocity, duration);

    // Work out the acceleration from the force. We'll add to this vector when we come to generate forces.
    Vector3 total_acceleration = m_acceleration;

    m_velocity = addScaledVector(m_velocity, total_acceleration, duration);

    m_velocity = m_velocity * real_pow(m_damping, duration);

     
}

} // namespace sputnink::physics