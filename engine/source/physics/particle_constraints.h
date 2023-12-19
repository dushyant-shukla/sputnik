#pragma once

#include "particle_contact.h"

namespace sputnik::physics
{

/**
 * Links connect two particles. They generate a contact if they violate their constraints. Cables and rods can be
 * modeled using links. Links can also be used as a base class for springs with a limit to their extension.
 */
class ParticleConstraint : public ParticleContactGenerator
{
public:
    ParticleConstraint() noexcept          = default;
    virtual ~ParticleConstraint() noexcept = default;

    /*!
     * @brief Fills the given contact structure with the contacts needed to keep the link from violating its constraint.
     *
     * @param contact The contact to fill.
     * @param limit The maximum number of contacts that can be written.
     * @return The number of contacts that have been written.
     */
    // virtual unsigned addContact(ParticleContact* contact, const unsigned& limit) const noexcept = 0;
    virtual unsigned addContact(std::vector<ParticleContact*>& contacts,
                                const unsigned&                current_contact_index,
                                const unsigned&                limit) const noexcept = 0;

protected:
    /*!
     * @brief Returns the current length of the link.
     *
     * @return The current length of the link.
     */
    virtual real currentLength() const noexcept;

public:
    /**
     * 	 Holds the pair of particles that are connected by this link.
     */
    Particle* m_particles[2];
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Cables link a pair of particles, generating a contact if they stray too far apart.
 */
class ParticleCable : public ParticleConstraint
{
public:
    ParticleCable() noexcept          = default;
    virtual ~ParticleCable() noexcept = default;

    /*!
     * @brief Fills the given contact structure with the contact needed to keep the cable from over-extending.
     *
     * @param contact The contact to fill.
     * @param limit The maximum number of contacts that can be written.
     * @return The number of contacts that have been written.
     */
    // virtual unsigned addContact(ParticleContact* contact, const unsigned& limit) const noexcept override;
    virtual unsigned addContact(std::vector<ParticleContact*>& contacts,
                                const unsigned&                current_contact_index,
                                const unsigned&                limit) const noexcept override;

public:
    /**
     * Holds the maximum length of the cable.
     */
    real m_max_length;

    /**
     * Holds the restitution (bounciness) of the cable.
     */
    real m_restitution;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Rods link a pair of particles, generating a contact if they stray too far apart or too close.
 */
class ParticleRod : public ParticleConstraint
{
public:
    ParticleRod() noexcept          = default;
    virtual ~ParticleRod() noexcept = default;

    /*!
     * @brief Fills the given contact structure with the contact needed to keep the rod from extending or compressing.
     *
     * @param contact The contact to fill.
     * @param limit The maximum number of contacts that can be written.
     * @return The number of contacts that have been written.
     */
    // virtual unsigned addContact(ParticleContact* contact, const unsigned& limit) const noexcept override;
    virtual unsigned addContact(std::vector<ParticleContact*>& contacts,
                                const unsigned&                current_contact_index,
                                const unsigned&                limit) const noexcept override;

public:
    /**
     * Holds the length of the rod.
     */
    real m_length{0.0f};
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Constraints are just like links, except they connect a particle to an immovable anchor point.
 */
class AnchoredParticleConstraint : public ParticleContactGenerator
{
public:
    AnchoredParticleConstraint() noexcept          = default;
    virtual ~AnchoredParticleConstraint() noexcept = default;

    /*!
     * @brief Fills the given contact structure with the contact needed to keep the constraint from being violated.
     *
     * @param contact The contact to fill.
     * @param limit The maximum number of contacts that can be written.
     * @return The number of contacts that have been written.
     */
    // virtual unsigned addContact(ParticleContact* contact, const unsigned& limit) const noexcept = 0;
    virtual unsigned addContact(std::vector<ParticleContact*>& contacts,
                                const unsigned&                current_contact_index,
                                const unsigned&                limit) const noexcept = 0;

protected:
    /*!
     * @brief Returns the current length of the link.
     *
     * @return The current length of the link.
     */
    virtual real currentLength() const noexcept;

public:
    /**
     * Holds the particle at connected to the anchor.
     */
    Particle* m_particle{nullptr};

    /**
     * Holds the point of connection of the constraint to the other particle.
     */
    vec3 m_anchor{0.0f};
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Cables link a particle to an anchor point, generating a contact if they stray too far apart.
 */
class AnchoredParticleCable : public AnchoredParticleConstraint
{
public:
    AnchoredParticleCable() noexcept          = default;
    virtual ~AnchoredParticleCable() noexcept = default;

    /*!
     * @brief Fills the given contact structure with the contact needed to keep the cable from over-extending.
     *
     * @param contact The contact to fill.
     * @param limit The maximum number of contacts that can be written.
     * @return The number of contacts that have been written.
     */
    // virtual unsigned addContact(ParticleContact* contact, const unsigned& limit) const noexcept override;
    virtual unsigned addContact(std::vector<ParticleContact*>& contacts,
                                const unsigned&                current_contact_index,
                                const unsigned&                limit) const noexcept override;

public:
    /**
     * Holds the maximum length of the cable.
     */
    real m_max_length{0.0f};

    /**
     * Holds the restitution (bounciness) of the cable.
     */
    real m_restitution{0.0f};
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Rods link a particle to an anchor point, generating a contact if they stray too far apart or too close.
 */
class AnchoredParticleRod : public AnchoredParticleConstraint
{
public:
    AnchoredParticleRod() noexcept          = default;
    virtual ~AnchoredParticleRod() noexcept = default;

    /*!
     * @brief Fills the given contact structure with the contact needed to keep the rod from extending or compressing.
     *
     * @param contact The contact to fill.
     * @param limit The maximum number of contacts that can be written.
     * @return The number of contacts that have been written.
     */
    // virtual unsigned addContact(ParticleContact* contact, const unsigned& limit) const noexcept override;
    virtual unsigned addContact(std::vector<ParticleContact*>& contacts,
                                const unsigned&                current_contact_index,
                                const unsigned&                limit) const noexcept override;

public:
    /**
     * Holds the length of the rod.
     */
    real m_length;
};

} // namespace sputnik::physics
