#pragma once

#include "core/core.h"
#include "physics/physics_core.h"
#include "physics/particle.h"
#include "physics/particle_world.h"
#include "physics/particle_constraints.h"
#include "physics/particle_force_registry.h"
#include "physics/particle_force_generator.h"

namespace sputnik::core::systems
{

using namespace ramanujan;
using namespace ramanujan::experimental;
using namespace sputnik::physics;

class PhysicsSystem
{

public:
    static PhysicsSystem* getInstance();

    virtual ~PhysicsSystem();

    void simulatePhysics(const real& timestep) noexcept;

    void initParticleWorld(const u32& max_contacts, const u32& iterations = 0) noexcept;

    void registerParticleForceGenerator(const std::shared_ptr<Particle>&                particle,
                                        const std::shared_ptr<ParticleForceGenerator>& particle_force_generator) noexcept;

    std::vector<std::shared_ptr<Particle>>& getParticles() noexcept;

    std::vector<std::shared_ptr<ParticleContactGenerator>>& getContactGenerators() noexcept;

    void addContactGenerator(const std::shared_ptr<ParticleContactGenerator>& contact_generator) noexcept;

    void addParticle(const std::shared_ptr<Particle>& particle) noexcept;

protected:
    // protected methods

    PhysicsSystem();

private:
    // private methods

private:
    // private data

    std::shared_ptr<ParticleWorld> m_particle_world;
};

} // namespace sputnik::core::systems