#include "pch.h"
#include "particle_constraints.h"

namespace sputnik::physics
{

real ParticleConstraint::currentLength() const noexcept
{
    vec3 relative_position = m_particles[0]->getPosition() - m_particles[1]->getPosition();
    return relative_position.length();
}

unsigned ParticleCable::addContact(ParticleContact* contact, const unsigned& limit) const noexcept
{
    real current_length = currentLength();

    // Check if the cable is over-extended
    if(current_length < m_max_length)
    {
        return 0;
    }

    // Otherwise return the contact
    contact->m_particles[0] = m_particles[0];
    contact->m_particles[1] = m_particles[1];

    // Calculate the normal
    vec3 normal = m_particles[1]->getPosition() - m_particles[0]->getPosition();
    normal.normalize();
    contact->m_contact_normal = normal;

    contact->m_penetration = current_length - m_max_length; // amount of extension
    contact->m_restitution = m_restitution;

    return 1;
}

unsigned ParticleRod::addContact(ParticleContact* contact, const unsigned& limit) const noexcept
{
    real current_length = currentLength();

    if(real_abs(current_length - m_length) < kEpsilon)
    {
        return 0;
    }

    contact->m_particles[0] = m_particles[0];
    contact->m_particles[1] = m_particles[1];

    vec3 normal = m_particles[1]->getPosition() - m_particles[0]->getPosition();
    normal.normalize();

    // Contact normal depends on whether the rod is compressed or extended
    if(current_length > m_length)
    {
        // extended
        contact->m_contact_normal = normal;
        contact->m_penetration    = current_length - m_length;
    }
    else
    {
        // compressed
        contact->m_contact_normal = normal * -1;
        contact->m_penetration    = m_length - current_length;
    }

    contact->m_restitution = 0; // rods don't generate bouncy contacts

    return 1;
}

real AnchoredParticleConstraint::currentLength() const noexcept
{
    vec3 relative_position = m_particle->getPosition() - m_anchor;
    return relative_position.length();
}

unsigned AnchoredParticleCable::addContact(ParticleContact* contact, const unsigned& limit) const noexcept
{
    real current_length = currentLength();

    // Check if the cable is over-extended
    if(current_length < m_max_length)
    {
        return 0;
    }

    contact->m_particles[0] = m_particle;
    contact->m_particles[1] = nullptr;

    // Calculate the normal
    vec3 normal = m_anchor - m_particle->getPosition();
    normal.normalize();
    contact->m_contact_normal = normal;

    contact->m_penetration = current_length - m_max_length; // amount of extension
    contact->m_restitution = m_restitution;

    return 1;
}

unsigned AnchoredParticleRod::addContact(ParticleContact* contact, const unsigned& limit) const noexcept
{
    real current_length = currentLength();

    if(real_abs(current_length - m_length) < kEpsilon)
    {
        return 0;
    }

    contact->m_particles[0] = m_particle;
    contact->m_particles[1] = nullptr;

    vec3 normal = m_anchor - m_particle->getPosition();
    normal.normalize();

    // Contact normal depends on whether the rod is compressed or extended
    if(current_length > m_length)
    {
        // extended
        contact->m_contact_normal = normal;
        contact->m_penetration    = current_length - m_length;
    }
    else
    {
        // compressed
        contact->m_contact_normal = normal * -1;
        contact->m_penetration    = m_length - current_length;
    }

    contact->m_restitution = 0; // rods don't generate bouncy contacts

    return 1;
}

} // namespace sputnik::physics
