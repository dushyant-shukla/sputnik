#include "pch.h"
#include "force_generators.hpp"

#include <execution>

namespace physics::mad
{
void SpringForceGenerator::addForce(MassAggregateSystem* const owning_system) {}

void SpringForceGenerator::addMassPair(const unsigned& mass_a_idx, const unsigned& mass_b_idx) noexcept
{
    // add the pair if it doesn't exist since a spring connection affects both particles
    auto itr = std::find_if(m_mass_pairs.begin(),
                            m_mass_pairs.end(),
                            [&](const auto& pair)
                            {
                                return (pair.first == mass_a_idx && pair.second == mass_b_idx) ||
                                       (pair.first == mass_b_idx && pair.second == mass_a_idx);
                            });
    if(itr == m_mass_pairs.end())
    {
        m_mass_pairs.emplace_back(mass_a_idx, mass_b_idx);
    }
}

void SpringForceGenerator::setRestLength(const real& rest_length) noexcept
{
    m_rest_length = rest_length;
}

void SpringForceGenerator::setSpringConstant(const real& spring_constant) noexcept
{
    m_spring_constant = spring_constant;
}

const std::vector<std::pair<unsigned, unsigned>>& SpringForceGenerator::getMassPairs() const noexcept
{
    return m_mass_pairs;
}

void BungeeForceGenerator::addForce(MassAggregateSystem* const owning_system) {}

} // namespace physics::mad
