#ifndef PHX_FORCE_GENERATOR
#define PHX_FORCE_GENERATOR

#include "../phx_core.hpp"

namespace phx::mad
{

class MassAggregateBody;

class ForceGenerator
{
public:
    ForceGenerator() noexcept          = default;
    virtual ~ForceGenerator() noexcept = default;

    virtual void updateForces(MassAggregateBody* const owning_system) noexcept             = 0;
    virtual void updateSatisfyConstraints(MassAggregateBody* const owning_system) noexcept = 0;
};

struct PhxSpring
{
    PhxIndex mass_a_idx;
    PhxIndex mass_b_idx;
    PhxReal  rest_length;
    PhxReal  ks;
    PhxReal  kd;
};

class SpringForceGenerator : public ForceGenerator
{
public:
    SpringForceGenerator() noexcept          = default;
    virtual ~SpringForceGenerator() noexcept = default;

    virtual void               updateForces(MassAggregateBody* const owning_system) noexcept override;
    virtual void               updateSatisfyConstraints(MassAggregateBody* const owning_system) noexcept override;
    void                       addSpring(const PhxSpring& spring) noexcept;
    const PhxArray<PhxSpring>& getSprings() const noexcept;

protected:
    PhxArray<PhxSpring> m_springs;
};

} // namespace phx::mad

#endif // !PHX_FORCE_GENERATOR
