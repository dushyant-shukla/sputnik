#pragma once

#include <precision.h>
#include <physics/particle.h>
#include <vector.hpp>
#include <physics/random.h>

#include <vector>
#include <memory>
#include <optional>
#include <cstdlib>
#include <ctime>

static sputnik::physics::Random s_random;

namespace sputnik::demos
{

using namespace ramanujan;
using namespace ramanujan::experimental;

/**
 * @brief Fireworks are particles with additional data for rendering and evolution.
 */
class Firework : public sputnik::physics::Particle
{

public:
    Firework() = default;

    virtual ~Firework() = default;

    bool update(const real& duration) noexcept
    {
        integrate(duration);

        m_age -= duration;
        return (m_age < 0) || (m_position.y < 0);
    }

public:
    /*
     * Firework type.
     */
    unsigned int m_type{0};

    /**
     * The age of the firework determines when it detonates. Age gradually decreases; when it passes zero the firework
     * delivers its payload. Think of age as fuse left.
     */
    real m_age{0.0f};
};

/**
 * A firework rule controls the length of a firework's fuse, and the particles it should evolve into.
 */
struct FireworkRule
{
    /**
     * The type of firework that is managed by this rule.
     */
    unsigned int m_type;

    /**
     * The minimum length of the fuse.
     */
    real m_min_age;

    /**
     * The maximum length of the fuse.
     */
    real m_max_age;

    /**
     * The minimum relative velocity of this firework.
     */
    vec3 m_min_velocity;

    /**
     * The damping of this firework type.
     */
    vec3 m_max_velocity;

    /**
     * The damping of this firework type.
     */
    real m_damping;

    struct Payload
    {
        /*
         * The type of the particles in this payload.
         */
        unsigned int m_type;

        /*
         * The number of particles in this payload.
         */
        unsigned int m_count;

        Payload(unsigned int type, unsigned int count) noexcept : m_type(type), m_count(count) {}
    };

    // void Init(unsigned int m_payload_count) { m_payloads.reserve(m_payload_count); }

    void SetRuleParameters(unsigned int type,
                           const real&  min_age,
                           const real&  max_age,
                           const vec3&  min_velocity,
                           const vec3&  max_velocity,
                           const real&  damping)
    {
        m_type         = type;
        m_min_age      = min_age;
        m_max_age      = max_age;
        m_min_velocity = min_velocity;
        m_max_velocity = max_velocity;
        m_damping      = damping;
    }

    void Create(Firework& firework, const std::optional<Firework>& parent)
    {
        firework.m_type = m_type;
        firework.m_age = s_random.randomReal(m_min_age + m_max_age);

        vec3 velocity;
        if(parent.has_value())
        {
            const Firework& p = parent.value();
            firework.setPosition(p.getPosition());
            velocity += p.getVelocity();
        }
        else
        {
            vec3 start;
            int  x  = (int)s_random.randomInt(3) - 1;
            start.x = 5.0f * real(x);
            firework.setPosition(start);
        }

        velocity += s_random.randomVector(m_min_velocity, m_max_velocity); // Todo:: This must be random
        firework.setVelocity(velocity);

        firework.setMass(1.0f);
        firework.setDamping(m_damping);
        firework.setAcceleration({0.0f, -9.81f, 0.0f}); // gravity
        firework.clearAccumulator();
    }

    /*
     * The set of payloads that make up this firework rule.
     */
    std::vector<Payload> m_payloads;
};

} // namespace sputnik::demos
