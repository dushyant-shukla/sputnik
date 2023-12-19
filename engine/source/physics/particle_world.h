#pragma once

#include "particle_force_registry.h"
#include "particle_contact.h"

namespace sputnik::physics
{

/**
 * Keeps track of a set of particles, and provides the means to update them all.
 */
class ParticleWorld
{
public:
    // using Particles         = std::vector<Particle*>;
    // using ContactGenerators = std::vector<ParticleContactGenerator*>;

    ParticleWorld(unsigned max_contacts, unsigned iterations = 0);

    virtual ~ParticleWorld();

    /*!
     * @brief Initializes the world for a simulation frame. This clears the force accumulators for particles in the
     * world. After calling this, the particles can have their forces for this frame added.
     */
    void startFrame() noexcept;

    /*!
     * @brief Processes all the physics for the particle world.
     *
     * @param duration The duration over which to integrate.
     */
    void simulatePhysics(real duration) noexcept;

    /*!
     * @brief Returns the list of particles.
     *
     * @return The list of particles.
     */
    std::vector<Particle*>& getParticles() noexcept;

    /*!
     * @brief Returns the list of contact generators.
     *
     * @return The list of contact generators.
     */
    std::vector<ParticleContactGenerator*>& getContactGenerators() noexcept;

    /*!
     * @brief Returns the force registry.
     *
     * @return The force registry.
     */
    ParticleForceRegistry& getForceRegistry() noexcept;

protected:
    /*!
     * @brief Integrates all the particles in this world forward in time by the given duration.
     *
     * @param duration The duration over which to integrate.
     */
    void integrate(real duration) noexcept;

    /*!
     * @brief Calls each of the registered contact generators to report their contacts. Returns the number of generated
     * contacts.
     *
     * @return The number of generated contacts.
     */
    unsigned generateContacts() noexcept;

protected:
    /**
     * Holds the particles.
     */
    std::vector<Particle*> m_particles;

    /**
     * Holds the force generators for the particles in this world.
     */
    ParticleForceRegistry m_force_registry;

    /**
     * Holds the resolver for contacts.
     */
    ParticleContactResolver m_contact_resolver;

    /**
     * Contact generators.
     */
    std::vector<ParticleContactGenerator*> m_contact_generators;

    /**
     * Holds the list of contacts.
     */
    std::vector<ParticleContact*> m_contacts;
    //ParticleContact* m_contacts;

    /**
     * Holds the maximum number of contacts allowed (i.e. the size of the contacts array).
     */
    unsigned m_max_contacts;

    /**
     * True if the physics world should calculate the number of iterations to give the contact resolver at each frame.
     */
    bool m_calculate_iterations;
};

class GroundContactGenerator : public ParticleContactGenerator
{
public:
    GroundContactGenerator()  = default;
    virtual ~GroundContactGenerator() = default;

    void init(const std::vector<Particle*>& particles) noexcept;

    //virtual unsigned addContact(ParticleContact* contact, const unsigned& limit) const noexcept override;
    virtual unsigned addContact(std::vector<ParticleContact*>& contacts,
                                const unsigned&                current_contact_index,
                                const unsigned&                limit) const noexcept override;

public:
    std::vector<Particle*> m_particles;
};

} // namespace sputnik::physics
