#include "mass_aggregate_body.hpp"

#include <execution>
#include <algorithm>

namespace phx::mad
{

////////////////////////////////////////////////////////////////
/////////////////// MassAggregateBody ////////////////////////
////////////////////////////////////////////////////////////////

MassAggregateBody::MassAggregateBody(const PhxUint& count, const MassAggregateBodySpec& spec) : m_spec(spec)
{
    m_masses.resize(count);
    m_positions.resize(count);
    m_velocities.resize(count);
    m_accelerations.resize(count);
    m_damping_values.resize(count);
    m_inverse_masses.resize(count);
    m_accumulated_forces.resize(count);
    m_is_fixed_values.resize(count);
    m_is_valid_values.resize(count);
}

PhxSize MassAggregateBody::getParticleCount() const noexcept
{
    return m_masses.size();
}

const PhxRealArray& MassAggregateBody::getMasses() const noexcept
{
    return m_masses;
}

const PhxVec3Array& MassAggregateBody::getPositions() const noexcept
{
    return m_positions;
}

const PhxVec3Array& MassAggregateBody::getVelocities() const noexcept
{
    return m_velocities;
}

const PhxVec3Array& MassAggregateBody::getAccelerations() const noexcept
{
    return m_accelerations;
}

const PhxRealArray& MassAggregateBody::getDampingValues() const noexcept
{
    return m_damping_values;
}

const PhxRealArray& MassAggregateBody::getInverseMasses() const noexcept
{
    return m_inverse_masses;
}

const PhxVec3Array& MassAggregateBody::getAccumulatedForces() const noexcept
{
    return m_accumulated_forces;
}

const PhxBoolArray& MassAggregateBody::getIsFixedValues() const noexcept
{
    return m_is_fixed_values;
}

const PhxBoolArray& MassAggregateBody::getIsValidValues() const noexcept
{
    return m_is_valid_values;
}

const PhxVec3& MassAggregateBody::getPosition(const PhxIndex& index) const noexcept
{
    return m_positions[index];
}

const PhxVec3& MassAggregateBody::getVelocity(const PhxIndex& index) const noexcept
{
    return m_velocities[index];
}

const PhxVec3& MassAggregateBody::getAcceleration(const PhxIndex& index) const noexcept
{
    return m_accelerations[index];
}

const PhxReal& MassAggregateBody::getDampingValue(const PhxIndex& index) const noexcept
{
    return m_damping_values[index];
}

const PhxReal& MassAggregateBody::getInverseMass(const PhxIndex& index) const noexcept
{
    return m_inverse_masses[index];
}

const PhxReal& MassAggregateBody::getMass(const PhxIndex& index) const noexcept
{
    return m_masses[index];
}

const PhxVec3& MassAggregateBody::getAccumulatedForce(const PhxIndex& index) const noexcept
{
    return m_accumulated_forces[index];
}

PhxBool MassAggregateBody::getIsFixed(const PhxIndex& index) const noexcept
{
    return m_is_fixed_values[index];
}

PhxBool MassAggregateBody::getIsValid(const PhxIndex& index) const noexcept
{
    return m_is_valid_values[index];
}

void MassAggregateBody::setPosition(const PhxIndex& index, const PhxVec3& position) noexcept
{
    m_positions[index] = position;
}

void MassAggregateBody::setVelocity(const PhxIndex& index, const PhxVec3& velocity) noexcept
{
    m_velocities[index] = velocity;
}

void MassAggregateBody::setAcceleration(const PhxIndex& index, const PhxVec3& acceleration) noexcept
{
    m_accelerations[index] = acceleration;
}

void MassAggregateBody::setDamping(const PhxIndex& index, const PhxReal& damping) noexcept
{
    m_damping_values[index] = damping;
}

void MassAggregateBody::setInverseMass(const PhxIndex& index, const PhxReal& inverse_mass) noexcept
{
    m_inverse_masses[index] = inverse_mass;
}

void MassAggregateBody::setMass(const PhxIndex& index, const PhxReal& mass) noexcept
{
    assert(mass > kPhxEpsilon);
    m_masses[index]         = mass;
    m_inverse_masses[index] = 1.0f / mass;
}

void MassAggregateBody::setAccumulatedForce(const PhxIndex& index, const PhxVec3& force) noexcept
{
    m_accumulated_forces[index] = force;
}

void MassAggregateBody::setIsFixed(const PhxIndex& index, const PhxBool& is_fixed) noexcept
{
    m_is_fixed_values[index] = is_fixed;
}

void MassAggregateBody::setIsValid(const PhxIndex& index, const PhxBool& is_valid) noexcept
{
    m_is_valid_values[index] = is_valid;
}

void MassAggregateBody::clearAccumulatedForces() noexcept
{
    std::for_each(std::execution::par_unseq,
                  m_accumulated_forces.begin(),
                  m_accumulated_forces.end(),
                  [this](auto& force) {
                      force = {0.0f, 0.0f, 0.0f};
                  });
}

void MassAggregateBody::addForce(const PhxIndex& index, const PhxVec3& force) noexcept
{
    m_accumulated_forces[index] += force;
}

void MassAggregateBody::registerForceGenerator(
    const std::function<void(MassAggregateBody* const)>& force_generator) noexcept
{
    m_force_generators.emplace_back(force_generator);
}

void MassAggregateBody::updateForces() noexcept
{
    // TODO: Implement this

    // for(const auto& force_generator : m_force_generators)
    //{
    //     force_generator(this);
    // }
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
///////////////////////////// MassAggregateVolume ////////////////////////
//////////////////////////////////////////////////////////////////////////

MassAggregateVolume::MassAggregateVolume(const PhxUint&               total_count,
                                         const PhxInt&                row_count,
                                         const PhxInt&                col_count,
                                         const PhxInt&                slice_count,
                                         const MassAggregateBodySpec& specification)
    : MassAggregateBody(total_count, specification)
{
    m_num_rows   = row_count;
    m_num_cols   = col_count;
    m_num_slices = slice_count;

    m_structural_spring_generator = std::make_shared<SpringForceGenerator>();
    m_shear_spring_generator      = std::make_shared<SpringForceGenerator>();
    m_flexion_spring_generator    = std::make_shared<SpringForceGenerator>();
    // m_torsion_spring_generator    = std::make_shared<SpringForceGenerator>();
}

// std::shared_ptr<SpringForceGenerator> MassAggregateVolume::getTorsionSpring() noexcept
//{
//     return m_torsion_spring_generator;
// }

PhxIndex
MassAggregateVolume::getIndex(const PhxInt& row_idx, const PhxInt& col_idx, const PhxInt& slice_idx) const noexcept
{
    return col_idx + m_num_cols * (row_idx + m_num_rows * slice_idx);
}

PhxUvec3 MassAggregateVolume::getLocalCoordinates(const PhxIndex& idx) const noexcept
{
    // NOTE: This is the reverse of the formula that is used in MassAggregateVolume::getIndex() function.
    unsigned index   = idx;
    unsigned col_idx = index % m_num_cols;  // reverse of (col_idx + m_num_cols) part of the formula above (getIndex())
    index = (index - col_idx) / m_num_cols; // index = (row_idx + m_num_rows * slice_idx) (see the getIndex())
    unsigned row_idx   = index % m_num_rows;
    unsigned slice_idx = index / m_num_rows;
    return {row_idx, col_idx, slice_idx};
}

const PhxVec3& MassAggregateVolume::getPosition(const PhxIndex& row_idx,
                                                const PhxIndex& col_idx,
                                                const PhxIndex& slice_idx) const noexcept
{
    PhxIndex index = getIndex(row_idx, col_idx, slice_idx);
    return m_positions[index];
}

const PhxVec3& MassAggregateVolume::getVelocity(const PhxIndex& row_idx,
                                                const PhxIndex& col_idx,
                                                const PhxIndex& slice_idx) const noexcept
{
    PhxIndex index = getIndex(row_idx, col_idx, slice_idx);
    return m_velocities[index];
}

const PhxVec3& MassAggregateVolume::getAcceleration(const PhxIndex& row_idx,
                                                    const PhxIndex& col_idx,
                                                    const PhxIndex& slice_idx) const noexcept
{
    PhxIndex index = getIndex(row_idx, col_idx, slice_idx);
    return m_accelerations[index];
}

const PhxVec3& MassAggregateVolume::getAccumulatedForce(const PhxIndex& row_idx,
                                                        const PhxIndex& col_idx,
                                                        const PhxIndex& slice_idx) const noexcept
{
    PhxIndex index = getIndex(row_idx, col_idx, slice_idx);
    return m_accumulated_forces[index];
}

const PhxReal& MassAggregateVolume::getDampingValue(const PhxIndex& row_idx,
                                                    const PhxIndex& col_idx,
                                                    const PhxIndex& slice_idx) const noexcept
{
    PhxIndex index = getIndex(row_idx, col_idx, slice_idx);
    return m_damping_values[index];
}

const PhxReal& MassAggregateVolume::getInverseMass(const PhxIndex& row_idx,
                                                   const PhxIndex& col_idx,
                                                   const PhxIndex& slice_idx) const noexcept
{
    PhxIndex index = getIndex(row_idx, col_idx, slice_idx);
    return m_inverse_masses[index];
}

const PhxReal&
MassAggregateVolume::getMass(const PhxIndex& row_idx, const PhxIndex& col_idx, const PhxIndex& slice_idx) const noexcept
{
    PhxIndex index = getIndex(row_idx, col_idx, slice_idx);
    return m_masses[index];
}

// PhxVec3
// MassAggregateVolume::getPosition(const PhxIndex& row_idx, const PhxIndex& col_idx, const PhxIndex& slice_idx)
// noexcept
//{
//     PhxIndex index = getIndex(row_idx, col_idx, slice_idx);
//     return m_positions[index];
// }
//
// PhxVec3
// MassAggregateVolume::getVelocity(const PhxIndex& row_idx, const PhxIndex& col_idx, const PhxIndex& slice_idx)
// noexcept
//{
//     PhxIndex index = getIndex(row_idx, col_idx, slice_idx);
//     return m_velocities[index];
// }
//
// PhxVec3 MassAggregateVolume::getAcceleration(const PhxIndex& row_idx,
//                                              const PhxIndex& col_idx,
//                                              const PhxIndex& slice_idx) noexcept
//{
//     PhxIndex index = getIndex(row_idx, col_idx, slice_idx);
//     return m_accelerations[index];
// }
//
// PhxVec3 MassAggregateVolume::getAccumulatedForce(const PhxIndex& row_idx,
//                                                  const PhxIndex& col_idx,
//                                                  const PhxIndex& slice_idx) noexcept
//{
//     PhxIndex index = getIndex(row_idx, col_idx, slice_idx);
//     return m_accumulated_forces[index];
// }
//
// PhxReal MassAggregateVolume::getDampingValue(const PhxIndex& row_idx,
//                                              const PhxIndex& col_idx,
//                                              const PhxIndex& slice_idx) noexcept
//{
//     PhxIndex index = getIndex(row_idx, col_idx, slice_idx);
//     return m_damping_values[index];
// }
//
// PhxReal MassAggregateVolume::getInverseMass(const PhxIndex& row_idx,
//                                             const PhxIndex& col_idx,
//                                             const PhxIndex& slice_idx) noexcept
//{
//     PhxIndex index = getIndex(row_idx, col_idx, slice_idx);
//     return m_inverse_masses[index];
// }
//
// PhxReal
// MassAggregateVolume::getMass(const PhxIndex& row_idx, const PhxIndex& col_idx, const PhxIndex& slice_idx) noexcept
//{
//     PhxIndex index = getIndex(row_idx, col_idx, slice_idx);
//     return m_masses[index];
// }

void MassAggregateVolume::updateForces() noexcept
{
    m_shear_spring_generator->updateForces(this);
    m_structural_spring_generator->updateForces(this);
    m_flexion_spring_generator->updateForces(this);
    // m_torsion_spring_generator->updateForces(this);
}

void MassAggregateVolume::addStructuralSpring(const PhxSpring& spring) noexcept
{
    m_structural_spring_generator->addSpring(spring);
}

void MassAggregateVolume::addShearSpring(const PhxSpring& spring) noexcept
{
    m_shear_spring_generator->addSpring(spring);
}

void MassAggregateVolume::addFlexionSpring(const PhxSpring& spring) noexcept
{
    m_flexion_spring_generator->addSpring(spring);
}

const PhxArray<PhxSpring>& MassAggregateVolume::getStructuralSprings() const noexcept
{
    return m_structural_spring_generator->getSprings();
}

const PhxArray<PhxSpring>& MassAggregateVolume::getShearSprings() const noexcept
{
    return m_shear_spring_generator->getSprings();
}

const PhxArray<PhxSpring>& MassAggregateVolume::getFlexionSprings() const noexcept
{
    return m_flexion_spring_generator->getSprings();
}

} // namespace phx::mad