#pragma once

#include <vector.hpp>

#include <vector>

namespace physics::mad
{

using namespace ramanujan;
using namespace ramanujan::experimental;

class MassAggregateSystem;

struct ForceGenerator
{
public:
    virtual void addForce(MassAggregateSystem* const owning_system) = 0;
};

struct SpringForceGenerator : public ForceGenerator
{
public:
    void addForce(MassAggregateSystem* const owning_system) override;

    void addMassPair(const unsigned& mass_a_idx, const unsigned& mass_b_idx) noexcept;
    void setRestLength(const real& rest_length) noexcept;
    void setSpringConstant(const real& spring_constant) noexcept;

protected:
    // a list of pairs of particles at the ends of the spring
    std::vector<std::pair<unsigned, unsigned>> m_mass_pairs;
    real                                       m_rest_length;
    real                                       m_spring_constant;
};

struct BungeeForceGenerator : public SpringForceGenerator
{
public:
    void addForce(MassAggregateSystem* const owning_system) override;
};

} // namespace physics::mad
