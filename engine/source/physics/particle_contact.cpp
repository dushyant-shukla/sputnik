#include "pch.h"
#include "particle_contact.h"

namespace sputnik::physics
{

void ParticleContact::resolve(const real& duration) noexcept
{
    resolveVelocity(duration);
    resolveInterpenetration(duration);
}

real ParticleContact::calculateSeparatingVelocity() const noexcept
{
    vec3 relative_velocity = m_particles[0]->getVelocity();
    if(m_particles[1]) // if the second particle exists
    {
        relative_velocity -= m_particles[1]->getVelocity();
    }
    return relative_velocity.dot(m_contact_normal);
}

void ParticleContact::resolveVelocity(const real& duration) noexcept
{
    // Find the velocity in the direction of the contact
    real separating_velocity = calculateSeparatingVelocity();

    // Check if it needs to be resolved
    if(separating_velocity > 0)
    {
        // The contact is either separating or stationary - there's no impulse required.
        return;
    }

    // Calculate the new separating velocity
    real new_separating_velocity = -separating_velocity * m_restitution;

    real delta_velocity = new_separating_velocity - separating_velocity;

    // We apply the change in velocity to each object in proportion to its inverse mass (i.e., those with lower inverse
    // mass [higher actual mass] get less change in velocity)
    real total_inverse_mass = m_particles[0]->getInverseMass();
    if(m_particles[1])
    {
        total_inverse_mass += m_particles[1]->getInverseMass();
    }

    // If all particles have infinite mass, then impulses have no effect
    // if(total_inverse_mass <= 0)
    if(total_inverse_mass <= kEpsilon)
    {
        return;
    }

    // Calculate the impulse to apply
    real total_impulse =
        delta_velocity / total_inverse_mass; // impulse = change in velocity * mass, mass = 1 / inverse_mass

    // Find the amount of impulse per unit of inverse mass
    // TODO:: Need to understand this better
    vec3 impulse = m_contact_normal * total_impulse;

    // Apply impulses: they are applied in the direction of the contact, and are proportional to the inverse mass
    m_particles[0]->setVelocity(m_particles[0]->getVelocity() + impulse * m_particles[0]->getInverseMass());

    if(m_particles[1])
    {
        // Particle 1 goes in the opposite direction
        m_particles[1]->setVelocity(m_particles[1]->getVelocity() + impulse * -m_particles[1]->getInverseMass());
    }
}

void ParticleContact::resolveInterpenetration(const real& duration) noexcept
{
    // If we don't have any penetration, we don't have anything to resolve
    if(m_penetration < kEpsilon)
    {
        return;
    }

    // Movement of each object is inversely proportional to its mass
    real total_inverse_mass = m_particles[0]->getInverseMass();
    if(m_particles[1])
    {
        total_inverse_mass += m_particles[1]->getInverseMass();
    }

    // The system has infinite mass - can't resolve the penetration
    if(total_inverse_mass < kEpsilon)
    {
        return;
    }

    // Find the amount of penetration resolution movement per unit of inverse mass
    vec3 move_per_inverse_mass = m_contact_normal * (m_penetration / total_inverse_mass);

    // Calculate the movement amounts
    m_particle_movement[0] = move_per_inverse_mass * m_particles[0]->getInverseMass();
    if(m_particles[1])
    {
        m_particle_movement[1] = move_per_inverse_mass * -m_particles[1]->getInverseMass();
    }
    else
    {
        m_particle_movement[1].clear();
    }

    // Apply the penetration resolution
    m_particles[0]->setPosition(m_particles[0]->getPosition() + m_particle_movement[0]);
    if(m_particles[1])
    {
        m_particles[1]->setPosition(m_particles[1]->getPosition() + m_particle_movement[1]);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ParticleContactResolver::ParticleContactResolver(const unsigned& iterations) noexcept
    : m_iterations(iterations)
    , m_iterations_used(0)
{
}

void ParticleContactResolver::setIterations(const unsigned& iterations) noexcept
{
    m_iterations = iterations;
}

void ParticleContactResolver::resolveContacts(std::vector<std::shared_ptr<ParticleContact>>& contact_array,
                                              const unsigned&                                num_contacts,
                                              const real&                                    duration) noexcept
{
    // TODO:: Need to understand contact resolution better

    m_iterations_used = 0;
    while(m_iterations_used < m_iterations)
    {
        // Find the contact with the largest closing velocity;
        real     max       = kRealMax;
        unsigned max_index = num_contacts;
        for(unsigned i = 0; i < num_contacts; ++i)
        {
            // calculate the separating velocity of the contact
            real separating_velocity = contact_array[i]->calculateSeparatingVelocity();
            if(separating_velocity < max && (separating_velocity < 0 || contact_array[i]->m_penetration > 0))
            {
                max       = separating_velocity;
                max_index = i;
            }
        }

        // Do we have anything worth resolving?
        if(max_index == num_contacts)
        {
            break;
        }

        // Resolve this contact
        contact_array[max_index]->resolve(duration);

        // Update the interpenetrations for all particles
        vec3* movement = contact_array[max_index]->m_particle_movement;
        for(unsigned i = 0; i < num_contacts; ++i)
        {
            if(contact_array[i]->m_particles[0] == contact_array[max_index]->m_particles[0])
            {
                contact_array[i]->m_penetration -= movement[0].dot(contact_array[i]->m_contact_normal);
            }
            else if(contact_array[i]->m_particles[0] == contact_array[max_index]->m_particles[1])
            {
                contact_array[i]->m_penetration -= movement[1].dot(contact_array[i]->m_contact_normal);
            }

            if(contact_array[i]->m_particles[1])
            {
                if(contact_array[i]->m_particles[1] == contact_array[max_index]->m_particles[0])
                {
                    contact_array[i]->m_penetration += movement[0].dot(contact_array[i]->m_contact_normal);
                }
                else if(contact_array[i]->m_particles[1] == contact_array[max_index]->m_particles[1])
                {
                    contact_array[i]->m_penetration += movement[1].dot(contact_array[i]->m_contact_normal);
                }
            }
        }

        ++m_iterations_used;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} // namespace sputnik::physics
