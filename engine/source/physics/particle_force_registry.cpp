#include "pch.h"

#include "particle_force_registry.h"

namespace sputnik::physics
{

void ParticleForceRegistry::add(Particle* particle, ParticleForceGenerator* fg)
{
    // ParticleForceRegistration registration(particle, fg);
    m_registrations.emplace_back(particle, fg);
}

void ParticleForceRegistry::remove(Particle* particle, ParticleForceGenerator* fg)
{
    for(auto it = m_registrations.begin(); it != m_registrations.end(); ++it)
    {
        // TODO: Check if this works
        if(it->m_particle == particle && it->m_fg == fg)
        {
            m_registrations.erase(it);
            break;
        }
    }
}

void ParticleForceRegistry::clear()
{
    m_registrations.clear();
}

void ParticleForceRegistry::updateForces(const real& duration)
{
    for(auto& registration : m_registrations)
    {
        registration.m_fg->updateForce(registration.m_particle, duration);
    }
}

} // namespace sputnik::physics
