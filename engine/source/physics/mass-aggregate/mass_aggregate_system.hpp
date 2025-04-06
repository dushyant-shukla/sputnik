#pragma once

#include "physics/physics_core.h"
#include "force_generators.hpp"

#include <precision.h>
#include <vector.hpp>

#include <vector>
#include <functional>

namespace physics::mad
{

using namespace ramanujan;
using namespace ramanujan::experimental;

enum class IntegrationMethod
{
    ExplicitEuler,
    SemiImplicitEuler,
    Verlet,
    RK4
};

class MassAggregateSystem
{
public:
    MassAggregateSystem() noexcept  = default;
    ~MassAggregateSystem() noexcept = default;

    [[nodiscard]] size_t getParticleCount() const noexcept;

    [[nodiscard]] const std::vector<real>& getMasses() const noexcept;
    [[nodiscard]] const std::vector<vec3>& getPositions() const noexcept;
    [[nodiscard]] const std::vector<vec3>& getVelocities() const noexcept;
    [[nodiscard]] const std::vector<vec3>& getAccelerations() const noexcept;
    [[nodiscard]] const std::vector<real>& getDampingValues() const noexcept;
    [[nodiscard]] const std::vector<real>& getInverseMasses() const noexcept;
    [[nodiscard]] const std::vector<vec3>& getAccumulatedForces() const noexcept;
    [[nodiscard]] const std::vector<bool>& getIsFixedValues() const noexcept;

    [[nodiscard]] const vec3& getPosition(const unsigned int& index) const noexcept;
    [[nodiscard]] const vec3& getVelocity(const unsigned int& index) const noexcept;
    [[nodiscard]] const vec3& getAcceleration(const unsigned int& index) const noexcept;
    [[nodiscard]] const real& getDampingValue(const unsigned int& index) const noexcept;
    [[nodiscard]] const real& getInverseMass(const unsigned int& index) const noexcept;
    [[nodiscard]] const real& getMass(const unsigned int& index) const noexcept;
    [[nodiscard]] const vec3& getAccumulatedForce(const unsigned int& index) const noexcept;
    [[nodiscard]] const bool& getIsFixed(const unsigned int& index) const noexcept;

    void setPosition(const unsigned int& index, const vec3& position) noexcept;
    void setVelocity(const unsigned int& index, const vec3& velocity) noexcept;
    void setAcceleration(const unsigned int& index, const vec3& acceleration) noexcept;
    void setDampingValue(const unsigned int& index, const real& damping) noexcept;
    void setInverseMass(const unsigned int& index, const real& inverse_mass) noexcept;
    void setMass(const unsigned int& index, const real& mass) noexcept;
    void setAccumulatedForce(const unsigned int& index, const vec3& force) noexcept;

    void clearAccumulatedForces() noexcept;
    void addForce(const unsigned int& index, const vec3& force) noexcept;

    void update(const real& dt) noexcept;

    void                                   setIntegrationMethod(const IntegrationMethod& method) noexcept;
    [[nodiscard]] const IntegrationMethod& getIntegrationMethod() const noexcept;

    virtual void updateInternalForces(const real& t, const real& dt) noexcept = 0;

    // register callbacks for the force generators
    void registerForceGenerator(const std::function<void(MassAggregateSystem* const)>& force_generator) noexcept;

protected:
    void integrateExplicitEuler(const real& dt) noexcept;
    void integrateSemiImplicitEuler(const real& dt) noexcept;
    void integrateVerlet(const real& dt) noexcept;
    void integrateRK4(const real& dt) noexcept;

protected:
    // Point mass(es) data
    std::vector<real> m_masses;
    std::vector<vec3> m_positions;
    std::vector<vec3> m_velocities;
    std::vector<vec3> m_accelerations;
    std::vector<real> m_damping_values;
    std::vector<real> m_inverse_masses;
    std::vector<vec3> m_accumulated_forces;
    std::vector<bool> m_is_fixed;

    IntegrationMethod m_active_integration_method{IntegrationMethod::SemiImplicitEuler};

    // a list of springs to apply forces to the particles
    // maybe have a super class called force generators this will be subclassed by springs, winds, etc.

    std::vector<std::function<void(MassAggregateSystem* const)>> m_force_generators;
};

} // namespace physics::mad
