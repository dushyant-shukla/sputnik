#include "pch.h"
#include "force_generators.hpp"

namespace physics::mad
{
void SpringForceGenerator::addForce(MassAggregateSystem* const owning_system) {}

void SpringForceGenerator::addMassPair(const unsigned& mass_a_idx, const unsigned& mass_b_idx) noexcept {}

void SpringForceGenerator::setRestLength(const real& rest_length) noexcept {}

void SpringForceGenerator::setSpringConstant(const real& spring_constant) noexcept {}

void BungeeForceGenerator::addForce(MassAggregateSystem* const owning_system) {}

} // namespace physics::mad
