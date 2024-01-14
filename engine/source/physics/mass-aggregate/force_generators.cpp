#include "pch.h"
#include "force_generators.hpp"
#include "mass_aggregate_system.hpp"

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
        // const auto& mass_a = owning_system->getMass(spring.mass_a_idx);
        // const auto& mass_b = owning_system->getMass(spring.mass_b_idx);
        // if(owning_system->getPosition(spring.mass_a_idx).y <= kEpsilon ||
        //    owning_system->getPosition(spring.mass_b_idx).y <= kEpsilon)
        //{
        //     int a = 10;
        // }
        // const auto force =
        //     owning_system->getPosition(spring.mass_a_idx) - owning_system->getPosition(spring.mass_b_idx);
        // const auto magnitude       = force.magnitude();
        // const auto extension       = magnitude - spring.rest_length;
        // const auto direction       = force.normalized();
        // const auto force_magnitude = -spring.spring_constant * extension;
        // const auto force_a         = direction * force_magnitude;
        // const auto force_b         = -1.0f * force_a;
        // owning_system->addForce(spring.mass_a_idx, force_a);
        // owning_system->addForce(spring.mass_b_idx, force_b);

        ////////////////////////////////////////////////////////////////

        const auto& position_a = owning_system->getPosition(spring.mass_a_idx);
        const auto& position_b = owning_system->getPosition(spring.mass_b_idx);
        if(position_a.y <= kEpsilon)
        {
            int a = 10;
        }
        auto       distance_vector       = position_a - position_b;
        const auto current_spring_length = distance_vector.magnitude();
        const auto delta_length          = current_spring_length - spring.rest_length;
        const auto force_a               = -spring.spring_constant * delta_length * distance_vector.normalize();
        const auto force_b               = -1.0f * force_a;
        owning_system->addForce(spring.mass_a_idx, force_a);
        owning_system->addForce(spring.mass_b_idx, force_b);
    }
}

void BungeeForceGenerator::addForce(MassAggregateSystem* const owning_system) {}

} // namespace physics::mad
