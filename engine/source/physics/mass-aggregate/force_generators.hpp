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

struct Spring
{
    unsigned mass_a_idx;
    unsigned mass_b_idx;
    real     rest_length;
    real     stiffness_coefficient;
    real     damping_coefficient;
};

struct SpringForceGenerator : public ForceGenerator
{
public:
    void                addForce(MassAggregateSystem* const owning_system) override;
    void                addSpring(const Spring& spring) noexcept;
    std::vector<Spring> getSprings() const noexcept;

protected:
    std::vector<Spring> m_springs;
};

struct BungeeForceGenerator : public SpringForceGenerator
{
public:
    void addForce(MassAggregateSystem* const owning_system) override;
};

} // namespace physics::mad
