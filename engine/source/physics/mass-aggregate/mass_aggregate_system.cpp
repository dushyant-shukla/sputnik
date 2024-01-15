#include "pch.h"
#include "mass_aggregate_system.hpp"

#include <algorithm>
#include <execution>
#include <ranges>

namespace physics::mad
{

const vec3& MassAggregateSystem::getPosition(const unsigned int& index) const noexcept
{
    return m_positions[index];
}

const vec3& MassAggregateSystem::getVelocity(const unsigned int& index) const noexcept
{
    return m_velocities[index];
}

const vec3& MassAggregateSystem::getAcceleration(const unsigned int& index) const noexcept
{
    return m_accelerations[index];
}

const real& MassAggregateSystem::getDampingValue(const unsigned int& index) const noexcept
{
    return m_damping_values[index];
}

const real& MassAggregateSystem::getInverseMass(const unsigned int& index) const noexcept
{
    return m_inverse_masses[index];
}

const real& MassAggregateSystem::getMass(const unsigned int& index) const noexcept
{
    return m_masses[index];
}

const vec3& MassAggregateSystem::getAccumulatedForce(const unsigned int& index) const noexcept
{
    return m_accumulated_forces[index];
}

const bool& MassAggregateSystem::getIsFixed(const unsigned int& index) const noexcept
{
    return m_is_fixed[index];
}

void MassAggregateSystem::setPosition(const unsigned int& index, const vec3& position) noexcept
{
    m_positions[index] = position;
}

void MassAggregateSystem::setVelocity(const unsigned int& index, const vec3& velocity) noexcept
{
    m_velocities[index] = velocity;
}

void MassAggregateSystem::setAcceleration(const unsigned int& index, const vec3& acceleration) noexcept
{
    m_accelerations[index] = acceleration;
}

void MassAggregateSystem::setDampingValue(const unsigned int& index, const real& damping) noexcept
{
    m_damping_values[index] = damping;
}

void MassAggregateSystem::setInverseMass(const unsigned int& index, const real& inverse_mass) noexcept
{
    m_inverse_masses[index] = inverse_mass;
}

void MassAggregateSystem::setMass(const unsigned int& index, const real& mass) noexcept
{
    assert(mass > kEpsilon);
    m_masses[index]         = mass;
    m_inverse_masses[index] = 1.0f / mass;
}

void MassAggregateSystem::setAccumulatedForce(const unsigned int& index, const vec3& force) noexcept
{
    m_accumulated_forces[index] = force;
}

void MassAggregateSystem::clearAccumulatedForces() noexcept
{
    for(auto& force : m_accumulated_forces)
    {
        force = {0.0f, 0.0f, 0.0f};
    }
}

void MassAggregateSystem::addForce(const unsigned int& index, const vec3& force) noexcept
{
    m_accumulated_forces[index] += force;
}

void MassAggregateSystem::update(const real& dt) noexcept
{
    // update spring forces
    updateInternalForces(0.0f, dt);

    // integrate
    switch(m_active_integration_method)
    {
    case IntegrationMethod::ExplicitEuler:
        integrateExplicitEuler(dt);
        break;
    case IntegrationMethod::SemiImplicitEuler:
        integrateSemiImplicitEuler(dt);
        break;
    case IntegrationMethod::Verlet:
        integrateVerlet(dt);
        break;
    case IntegrationMethod::RK4:
        integrateRK4(dt);
        break;
    default:
        break;
    }

    // collision detection/resolution
}

void MassAggregateSystem::setIntegrationMethod(const IntegrationMethod& method) noexcept
{
    m_active_integration_method = method;
}

const IntegrationMethod& MassAggregateSystem::getIntegrationMethod() const noexcept
{
    return m_active_integration_method;
}

void MassAggregateSystem::registerForceGenerator(
    const std::function<void(MassAggregateSystem* const)>& force_generator) noexcept
{
    m_force_generators.push_back(force_generator);
}

void MassAggregateSystem::integrateExplicitEuler(const real& dt) noexcept
{
    size_t num_particles = m_masses.size();
    for(size_t i = 0; i < num_particles; i++)
    {
        if(m_inverse_masses[i] <= kEpsilon)
        {
            continue;
        }

        m_positions[i] += m_velocities[i] * (std::pow(m_damping_values[i], dt) - 1.0f) / std::log(m_damping_values[i]);
        vec3 total_acceleration = m_accelerations[i];
        total_acceleration += m_accumulated_forces[i] * m_inverse_masses[i];
        m_velocities[i] += total_acceleration * dt;
        m_velocities[i] *= std::pow(m_damping_values[i], dt);
        m_accumulated_forces[i] = {0.0f, 0.0f, 0.0f};
        if(m_positions[i].y < kEpsilon)
        {
            m_positions[i].y = 0.0f;
        }
    }
}

void MassAggregateSystem::integrateSemiImplicitEuler(const real& dt) noexcept
{
    // size_t num_particles = m_masses.size();
    // for(size_t i = 0; i < num_particles; i++)
    //{
    //     if(m_inverse_masses[i] <= kEpsilon)
    //     {
    //         continue;
    //     }

    //    //
    //    https://gamedev.stackexchange.com/questions/169558/how-can-i-fix-my-velocity-damping-to-work-with-any-delta-frame-time
    //    vec3 total_acceleration = m_accelerations[i];
    //    total_acceleration += m_accumulated_forces[i] * m_inverse_masses[i];
    //    m_velocities[i] += total_acceleration * dt;
    //    m_velocities[i] *= std::pow(m_damping_values[i], dt);
    //    m_positions[i] += m_velocities[i] * (std::pow(m_damping_values[i], dt) - 1.0f) /
    //    std::log(m_damping_values[i]); m_accumulated_forces[i] = {0.0f, 0.0f, 0.0f}; if(m_positions[i].y < kEpsilon)
    //    {
    //        m_positions[i].y = 0.0f;
    //    }
    //}

    std::ranges::iota_view indexes((size_t)0, m_positions.size());
    std::for_each(std::execution::par_unseq,
                  indexes.begin(),
                  indexes.end(),
                  [&](const auto& index)
                  {
                      if(m_inverse_masses[index] > kEpsilon && !m_is_fixed[index])
                      {
                          // https://gamedev.stackexchange.com/questions/169558/how-can-i-fix-my-velocity-damping-to-work-with-any-delta-frame-time
                          vec3 total_acceleration = m_accelerations[index];
                          total_acceleration += m_accumulated_forces[index] * m_inverse_masses[index];
                          m_velocities[index] += total_acceleration * dt;
                          m_velocities[index] *= std::pow(m_damping_values[index], dt);
                          m_positions[index] += m_velocities[index] * (std::pow(m_damping_values[index], dt) - 1.0f) /
                                                std::log(m_damping_values[index]);
                          m_accumulated_forces[index] = {0.0f, 0.0f, 0.0f};

                          // only temporary until collision detection/resolution is implemented
                          if(m_positions[index].y < kEpsilon)
                          {
                              m_positions[index].y = 0.0f;
                          }
                      }
                  });
}

void MassAggregateSystem::integrateVerlet(const real& dt) noexcept {}

void MassAggregateSystem::integrateRK4(const real& dt) noexcept {}

size_t MassAggregateSystem::getParticleCount() const noexcept
{
    return m_masses.size();
}

const std::vector<real>& MassAggregateSystem::getMasses() const noexcept
{
    return m_masses;
}

const std::vector<vec3>& MassAggregateSystem::getPositions() const noexcept
{
    return m_positions;
}

const std::vector<vec3>& MassAggregateSystem::getVelocities() const noexcept
{
    return m_velocities;
}

const std::vector<vec3>& MassAggregateSystem::getAccelerations() const noexcept
{
    return m_accelerations;
}

const std::vector<real>& MassAggregateSystem::getDampingValues() const noexcept
{
    return m_damping_values;
}

const std::vector<real>& MassAggregateSystem::getInverseMasses() const noexcept
{
    return m_inverse_masses;
}

const std::vector<vec3>& MassAggregateSystem::getAccumulatedForces() const noexcept
{
    return m_accumulated_forces;
}

const std::vector<bool>& MassAggregateSystem::getIsFixedValues() const noexcept
{
    return m_is_fixed;
}

} // namespace physics::mad
