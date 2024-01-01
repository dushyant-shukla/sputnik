#include "pch.h"
#include "particle_world.h"
#include "physics_core.h"

namespace sputnik::physics
{

ParticleWorld::ParticleWorld(unsigned max_contacts, unsigned iterations)
    : m_max_contacts(max_contacts)
    , m_contact_resolver(iterations)
{
    // m_contacts             = new ParticleContact[m_max_contacts];
    m_contacts.resize(m_max_contacts);
    for(unsigned i = 0; i < m_max_contacts; ++i)
    {
        // m_contacts[i] = new ParticleContact();
        m_contacts[i] = std::make_shared<ParticleContact>();
    }

    // if iterations is 0, then the resolver will calculate the number of iterations required at each frame
    m_calculate_iterations = (iterations == 0);
}

ParticleWorld::~ParticleWorld()
{
    // for(ParticleContact* contact : m_contacts)
    //{
    //     delete contact;
    // }
    //  delete[] m_contacts;
}

void ParticleWorld::startFrame() noexcept
{
    for(auto& particle : m_particles)
    {
        particle->clearAccumulator();
    }
}

void ParticleWorld::simulatePhysics(real duration) noexcept
{
    m_force_registry.updateForces(duration);

    integrate(duration);

    unsigned total_contacts = generateContacts();
    if(total_contacts)
    {
        if(m_calculate_iterations)
        {
            m_contact_resolver.setIterations(total_contacts * 2);
        }
        m_contact_resolver.resolveContacts(m_contacts, total_contacts, duration);
    }
}

std::vector<std::shared_ptr<Particle>>& ParticleWorld::getParticles() noexcept
{
    return m_particles;
}

std::vector<std::shared_ptr<ParticleContactGenerator>>& ParticleWorld::getContactGenerators() noexcept
{
    return m_contact_generators;
}

ParticleForceRegistry& ParticleWorld::getForceRegistry() noexcept
{
    return m_force_registry;
}

void ParticleWorld::integrate(real duration) noexcept
{
    for(auto& particle : m_particles)
    {
        particle->integrate(duration);
    }
}

unsigned ParticleWorld::generateContacts() noexcept
{
    unsigned limit = m_max_contacts;
    // ParticleContact* next_contact = m_contacts;
    unsigned next_contact = 0;

    for(auto& generator : m_contact_generators)
    {
        // unsigned used = generator->addContact(m_contacts, limit);
        // unsigned used = generator->addContact(m_contacts[next_contact], limit);
        unsigned used = generator->addContact(m_contacts, next_contact, limit);
        limit -= used;
        next_contact += used;

        // We've run out of contacts to fill. This means we're missing contacts.
        if(limit <= 0)
        {
            break;
        }
    }

    return m_max_contacts - limit; // Return the number of contacts used.
}

void GroundContactGenerator::init(const std::vector<std::shared_ptr<Particle>>& particles) noexcept
{
    m_particles = particles;
}

unsigned GroundContactGenerator::addContact(std::vector<std::shared_ptr<ParticleContact>>& contacts,
                                            const unsigned&                                current_contact_index,
                                            const unsigned&                                limit) noexcept
{
    unsigned count = 0;
    for(auto& particle : m_particles)
    {
        real y = particle->getPosition().y;
        if(y < kEpsilon)
        {
            contacts[count]->m_contact_normal = kUp; // count should be replaced with current_contact_index TODO??
            contacts[count]->m_particles[0]   = particle;
            contacts[count]->m_particles[1]   = nullptr;
            contacts[count]->m_penetration    = -y;
            // contacts[count]->m_restitution    = real(0.2);
            contacts[count]->m_restitution = real(0.75);

            //++contact;
            ++count;
        }

        if(count >= limit)
        {
            return count;
        }
    }
    return count;
}

// unsigned GroundContactGenerator::addContact(ParticleContact* contact, const unsigned& limit) const noexcept
//{
//    unsigned count = 0;
//    for(Particle* particle : m_particles)
//    {
//        real y = particle->getPosition().y;
//        if(y < kEpsilon)
//        {
//            contact->m_contact_normal = kUp;
//            contact->m_particles[0]   = particle;
//            contact->m_particles[1]   = nullptr;
//            contact->m_penetration    = -y;
//            contact->m_restitution    = real(0.2);
//
//            ++contact;
//            ++count;
//        }
//
//        if(count >= limit)
//        {
//            return count;
//        }
//    }
//    return count;
//}

} // namespace sputnik::physics
