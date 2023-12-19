#pragma once

#include "particle.h"

namespace sputnik::physics
{

using namespace ramanujan;
using namespace ramanujan::experimental;

class ParticleContactResolver;

/**
 * A particle contact represents two particles in contact. Resolving a contact removes their interpenetration, and
 * applies sufficient impulse to keep them apart. Colliding bodies may also rebound.
 */
class ParticleContact
{

    friend class ParticleContactResolver;

public:
    ~ParticleContact() noexcept = default;

    /*!
     * @brief Resolves the contact for both velocity and interpenetration.
     *
     * @param duration
     */
    void resolve(const real& duration) noexcept;

    /*!
     * @brief Calculates the separating velocity at this contact.
     *
     * @return
     */
    real calculateSeparatingVelocity() const noexcept;

private:
    /*!
     * @brief Handles the impulse calculations for this collision.
     *
     * @param duration
     */
    void resolveVelocity(const real& duration) noexcept;

    /*!
     * @brief .
     *
     * @param duration
     */
    void resolveInterpenetration(const real& duration) noexcept;

public:
    /**
     * Holds the particles involved in the contact. The second of these can be NULL, for contacts with the scenery.
     */
    Particle* m_particles[2];

    /**
     * Holds the coefficient of restitution at the contact.
     */
    real m_restitution;

    /**
     * Holds the depth of penetration at the contact.
     */
    real m_penetration;

    /**
     * Holds the direction of contact normal in world space coordinates (from the first particle's perspectives).
     */
    vec3 m_contact_normal;

    /**
     * Holds the amount each particle is moved by during interpenetration resolution.
     */
    vec3 m_particle_movement[2];
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ParticleContactResolver
{

public:
    /*!
     * @brief Creates a new particle contact resolver.
     *
     * @param iterations The number of iterations through the resolver.
     */
    ParticleContactResolver(const unsigned& iterations) noexcept;

    /*!
     * @brief Sets the number of iterations that can be used.
     *
     * @param iterations The number of iterations that can be used.
     */
    void setIterations(const unsigned& iterations) noexcept;

    /*!
     * @brief Resolves a set of particle contacts for both penetration and velocity.
     *
     * @param contact_array The array of contacts.
     * @param num_contacts The number of contacts in the array to resolve.
     * @param duration The duration of the prevuious integration step. This is used to compensate for forces applied.
     */
    void resolveContacts(std::vector<ParticleContact*> contact_array,
                         const unsigned&               num_contacts,
                         const real&                   duration) noexcept;

protected:
    /**
     * Holds the number of iterations allowed.
     */
    unsigned m_iterations;

    /**
     * This is a performance tracking value - we keep a record of the actual number of iterations used.
     */
    unsigned m_iterations_used;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * This is the basic polymorphic interface for contact generators applying to particles.
 */
class ParticleContactGenerator
{
public:
    virtual ~ParticleContactGenerator() noexcept = default;

    /*!
     * @brief Fills the given contact structures with the generated contact. The contact pointer should point to the
     * first available contact in a contact array, where limit is the maximum number of contacts in the array that can
     * be written to. The method returns the number of contacts that have been written.
     *
     * @param contact The contact to fill.
     * @param limit The maximum number of contacts that can be written.
     * @return The number of contacts that have been written.
     */
    // virtual unsigned addContact(ParticleContact* contact, const unsigned& limit) const noexcept = 0;
    virtual unsigned addContact(std::vector<ParticleContact*>& contacts,
                                const unsigned&                current_contact_index,
                                const unsigned&                limit) const noexcept = 0;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} // namespace sputnik::physics