#include "force_generator.hpp"
#include "mass_aggregate_body.hpp"
#include "../math_utils.hpp"

namespace phx::mad
{

void SpringForceGenerator::updateForces(MassAggregateBody* const owning_body) noexcept
{
    for(const auto& spring : m_springs)
    {
        {
            const auto& position_a = owning_body->getPositions()[spring.mass_a_idx];
            const auto& position_b = owning_body->getPositions()[spring.mass_b_idx];
            const auto& velocity_a = owning_body->getVelocities()[spring.mass_a_idx];
            const auto& velocity_b = owning_body->getVelocities()[spring.mass_b_idx];

            auto       distance_vector       = position_a - position_b;
            const auto current_spring_length = phx_magnitude(distance_vector);
            const auto force_direction       = phx_normalize(distance_vector);
            const auto delta_length          = current_spring_length - spring.rest_length;

            const auto spring_force = -spring.ks * delta_length * force_direction;
            const auto damping_force =
                -spring.kd * phx_dot((velocity_a - velocity_b), force_direction) * force_direction;

            const auto force_a = spring_force + damping_force;
            const auto force_b = -1.0f * force_a;
            owning_body->addForce(spring.mass_a_idx, force_a);
            owning_body->addForce(spring.mass_b_idx, force_b);
        }
    }
}

void SpringForceGenerator::addSpring(const PhxSpring& spring) noexcept
{
    const auto& itr =
        std::find_if(m_springs.begin(),
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

const PhxArray<PhxSpring>& SpringForceGenerator::getSprings() const noexcept
{
    return m_springs;
}

} // namespace phx::mad