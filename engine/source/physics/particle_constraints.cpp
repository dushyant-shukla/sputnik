#include "pch.h"
#include "particle_constraints.h"

namespace sputnik::physics
{

real ParticleConstraint::currentLength() const noexcept
{
    vec3 relative_position = m_particles[0]->getPosition() - m_particles[1]->getPosition();
    return relative_position.length();
}

unsigned ParticleCable::addContact(std::vector<std::shared_ptr<ParticleContact>>& contacts,
                                   const unsigned&                                current_contact_index,
                                   const unsigned&                                limit) noexcept
{
    real current_length = currentLength();

    // Check if the cable is over-extended
    if(current_length < m_max_length)
    {
        return 0;
    }

    // Otherwise return the contact
    contacts[current_contact_index]->m_particles[0] = m_particles[0];
    contacts[current_contact_index]->m_particles[1] = m_particles[1];

    // Calculate the normal
    vec3 normal = m_particles[1]->getPosition() - m_particles[0]->getPosition();
    normal.normalize();
    contacts[current_contact_index]->m_contact_normal = normal;

    contacts[current_contact_index]->m_penetration = current_length - m_max_length; // amount of extension
    contacts[current_contact_index]->m_restitution = m_restitution;

    return 1;
}

unsigned ParticleRod::addContact(std::vector<std::shared_ptr<ParticleContact>>& contacts,
                                 const unsigned&                                current_contact_index,
                                 const unsigned&                                limit) noexcept
{
    real current_length = currentLength();

    if(real_abs(current_length - m_length) < kEpsilon)
    {
        return 0;
    }

    contacts[current_contact_index]->m_particles[0] = m_particles[0];
    contacts[current_contact_index]->m_particles[1] = m_particles[1];

    vec3 normal = m_particles[1]->getPosition() - m_particles[0]->getPosition();
    normal.normalize();

    // Contact normal depends on whether the rod is compressed or extended
    if(current_length > m_length)
    {
        // extended
        contacts[current_contact_index]->m_contact_normal = normal;
        contacts[current_contact_index]->m_penetration    = current_length - m_length;
    }
    else
    {
        // compressed
        contacts[current_contact_index]->m_contact_normal = normal * -1;
        contacts[current_contact_index]->m_penetration    = m_length - current_length;
    }

    contacts[current_contact_index]->m_restitution = 0; // rods don't generate bouncy contacts

    return 1;
}

real AnchoredParticleConstraint::currentLength() const noexcept
{
    vec3 relative_position = m_particle->getPosition() - m_anchor;
    return relative_position.length();
}

unsigned AnchoredParticleCable::addContact(std::vector<std::shared_ptr<ParticleContact>>& contacts,
                                           const unsigned&                                current_contact_index,
                                           const unsigned&                                limit) noexcept
{
    real current_length = currentLength();

    // Check if the cable is over-extended
    if(current_length < m_max_length)
    {
        return 0;
    }

    contacts[current_contact_index]->m_particles[0] = m_particle;
    contacts[current_contact_index]->m_particles[1] = nullptr;

    // Calculate the normal
    vec3 normal = m_anchor - m_particle->getPosition();
    normal.normalize();
    contacts[current_contact_index]->m_contact_normal = normal;

    contacts[current_contact_index]->m_penetration = current_length - m_max_length; // amount of extension
    contacts[current_contact_index]->m_restitution = m_restitution;

    return 1;
}

unsigned AnchoredParticleRod::addContact(std::vector<std::shared_ptr<ParticleContact>>& contacts,
                                         const unsigned&                                current_contact_index,
                                         const unsigned&                                limit) noexcept
{
    real current_length = currentLength();

    if(real_abs(current_length - m_length) < kEpsilon)
    {
        return 0;
    }

    contacts[current_contact_index]->m_particles[0] = m_particle;
    contacts[current_contact_index]->m_particles[1] = nullptr;

    vec3 normal = m_anchor - m_particle->getPosition();
    normal.normalize();

    // Contact normal depends on whether the rod is compressed or extended
    if(current_length > m_length)
    {
        // extended
        contacts[current_contact_index]->m_contact_normal = normal;
        contacts[current_contact_index]->m_penetration    = current_length - m_length;
    }
    else
    {
        // compressed
        contacts[current_contact_index]->m_contact_normal = normal * -1;
        contacts[current_contact_index]->m_penetration    = m_length - current_length;
    }

    contacts[current_contact_index]->m_restitution = 0; // rods don't generate bouncy contacts

    return 1;
}

} // namespace sputnik::physics
