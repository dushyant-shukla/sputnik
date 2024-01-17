#include "pch.h"
#include "force_generators.hpp"
#include "mass_aggregate_system.hpp"

#include <ranges>
#include <execution>

namespace physics::mad
{
void SpringForceGenerator::addSpring(const Spring& spring) noexcept
{
    auto itr = std::find_if(m_springs.begin(),
                            m_springs.end(),
                            [&](const auto& s)
                            {
                                return (s.mass_a_idx == spring.mass_a_idx && s.mass_b_idx == spring.mass_b_idx) ||
                                       (s.mass_a_idx == spring.mass_b_idx && s.mass_b_idx == spring.mass_a_idx);
                            });
    if(itr == m_springs.end())
    {
        m_springs.emplace_back(spring);
    }
}

std::vector<Spring> SpringForceGenerator::getSprings() const noexcept
{
    return m_springs;
}

void SpringForceGenerator::addForce(MassAggregateSystem* const owning_system)
{
    for(const auto& spring : m_springs)
    {
        {
            const auto& position_a = owning_system->getPosition(spring.mass_a_idx);
            const auto& position_b = owning_system->getPosition(spring.mass_b_idx);
            const auto& velocity_a = owning_system->getVelocity(spring.mass_a_idx);
            const auto& velocity_b = owning_system->getVelocity(spring.mass_b_idx);

            auto       distance_vector       = position_a - position_b;
            const auto current_spring_length = distance_vector.magnitude();
            const vec3 force_direction       = distance_vector.normalized();
            const auto delta_length          = current_spring_length - spring.rest_length;

            const auto spring_force = -spring.stiffness_coefficient * delta_length * force_direction;
            const auto damping_force =
                -spring.damping_coefficient * (velocity_a - velocity_b).dot(force_direction) * force_direction;

            const auto force_a = spring_force + damping_force;
            const auto force_b = -1.0f * force_a;
            owning_system->addForce(spring.mass_a_idx, force_a);
            owning_system->addForce(spring.mass_b_idx, force_b);
        }
    }
}

void BungeeForceGenerator::addForce(MassAggregateSystem* const owning_system) {}

} // namespace physics::mad
