#include "pch.h"
#include "physics_system.h"

namespace sputnik::core::systems
{

using namespace sputnik::physics;

PhysicsSystem::PhysicsSystem() {}

PhysicsSystem::~PhysicsSystem() {}

PhysicsSystem* PhysicsSystem::getInstance()
{
    static PhysicsSystem instance;
    return &instance;
}

void PhysicsSystem::initParticleWorld(const u32& max_contacts, const u32& iterations) noexcept
{
    m_particle_world = std::make_shared<ParticleWorld>(max_contacts, iterations);

    //std::shared_ptr<GroundContactGenerator> ground_contact_generator = std::make_shared<GroundContactGenerator>();
    //ground_contact_generator->init(m_particle_world->getParticles());
    //m_particle_world->getContactGenerators().push_back(ground_contact_generator);
}

void PhysicsSystem::simulatePhysics(const real& timestep) noexcept
{
    if(m_particle_world)
    {
        m_particle_world->startFrame();
        m_particle_world->simulatePhysics(timestep);
    }
}

void PhysicsSystem::registerParticleForceGenerator(
    const std::shared_ptr<Particle>&               particle,
    const std::shared_ptr<ParticleForceGenerator>& particle_force_generator) noexcept
{
    if(m_particle_world)
    {
        m_particle_world->getForceRegistry().add(particle, particle_force_generator);
    }
}

std::vector<std::shared_ptr<Particle>>& PhysicsSystem::getParticles() noexcept
{
    SPUTNIK_ASSERT(m_particle_world, "Particle world is not initialized!");
    return m_particle_world->getParticles();
}

std::vector<std::shared_ptr<ParticleContactGenerator>>& PhysicsSystem::getContactGenerators() noexcept
{
    SPUTNIK_ASSERT(m_particle_world, "Particle world is not initialized!");
    return m_particle_world->getContactGenerators();
}

void PhysicsSystem::addContactGenerator(const std::shared_ptr<ParticleContactGenerator>& contact_generator) noexcept
{
    if(m_particle_world)
    {
        m_particle_world->getContactGenerators().push_back(contact_generator);
    }
}

void PhysicsSystem::addParticle(const std::shared_ptr<Particle>& particle) noexcept
{
    if(m_particle_world)
    {
        m_particle_world->getParticles().push_back(particle);
    }
}

} // namespace sputnik::core::systems