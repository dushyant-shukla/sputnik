#ifndef PHX_MAD_MASS_AGGREGATE_SYSTEM_HPP
#define PHX_MAD_MASS_AGGREGATE_SYSTEM_HPP

#include "../phx_core.hpp"
#include "../geometry.hpp"
#include "force_generator.hpp"

#include <functional>
#include <memory>

namespace phx::mad
{

struct SpringCoefficient
{
    PhxReal ks;
    PhxReal kd;
};

struct MassAggregateBodySpec
{
    PhxReal mass;
    PhxVec3 initial_velocity;
    PhxVec3 acceleration;
    PhxReal damping;
    PhxBool randomize_initial_velocity;

    PhxVec3 extent;
    PhxVec3 step;
    PhxUint nearest_neighbors = 10;

    SpringCoefficient structural_spring_coeffs;
    SpringCoefficient shear_spring_coeffs;
    SpringCoefficient flexion_spring_coeffs;
    SpringCoefficient torsion_spring_coeffs;
    SpringCoefficient surface_spring_coeffs;
    SpringCoefficient internal_spring_coeffs;
};

class MassAggregateBody
{

public:
    // MassAggregateBody() noexcept          = default;

    MassAggregateBody(const PhxUint& count, const MassAggregateBodySpec& spec);
    virtual ~MassAggregateBody() noexcept = default;

    [[nodiscard]] PhxSize getParticleCount() const noexcept;

    [[nodiscard]] const PhxRealArray& getMasses() const noexcept;
    [[nodiscard]] const PhxVec3Array& getPositions() const noexcept;
    [[nodiscard]] const PhxVec3Array& getVelocities() const noexcept;
    [[nodiscard]] const PhxVec3Array& getAccelerations() const noexcept;
    [[nodiscard]] const PhxRealArray& getDampingValues() const noexcept;
    [[nodiscard]] const PhxRealArray& getInverseMasses() const noexcept;
    [[nodiscard]] const PhxVec3Array& getAccumulatedForces() const noexcept;
    [[nodiscard]] const PhxBoolArray& getIsFixedValues() const noexcept;
    [[nodiscard]] const PhxBoolArray& getIsValidValues() const noexcept;

    [[nodiscard]] const PhxVec3& getPosition(const PhxIndex& index) const noexcept;
    [[nodiscard]] const PhxVec3& getVelocity(const PhxIndex& index) const noexcept;
    [[nodiscard]] const PhxVec3& getAcceleration(const PhxIndex& index) const noexcept;
    [[nodiscard]] const PhxReal& getDampingValue(const PhxIndex& index) const noexcept;
    [[nodiscard]] const PhxReal& getInverseMass(const PhxIndex& index) const noexcept;
    [[nodiscard]] const PhxReal& getMass(const PhxIndex& index) const noexcept;
    [[nodiscard]] const PhxVec3& getAccumulatedForce(const PhxIndex& index) const noexcept;
    [[nodiscard]] PhxBool        getIsFixed(const PhxIndex& index) const noexcept;
    [[nodiscard]] PhxBool        getIsValid(const PhxIndex& index) const noexcept;

    void setPosition(const PhxIndex& index, const PhxVec3& position) noexcept;
    void setVelocity(const PhxIndex& index, const PhxVec3& velocity) noexcept;
    void setAcceleration(const PhxIndex& index, const PhxVec3& acceleration) noexcept;
    void setDamping(const PhxIndex& index, const PhxReal& damping) noexcept;
    void setInverseMass(const PhxIndex& index, const PhxReal& inverse_mass) noexcept;
    void setMass(const PhxIndex& index, const PhxReal& mass) noexcept;
    void setAccumulatedForce(const PhxIndex& index, const PhxVec3& force) noexcept;
    void setIsFixed(const PhxIndex& index, const PhxBool& is_fixed) noexcept;
    void setIsValid(const PhxIndex& index, const PhxBool& is_valid) noexcept;

    void clearAccumulatedForces() noexcept;
    void addForce(const PhxIndex& index, const PhxVec3& force) noexcept;

    void registerForceGenerator(const std::function<void(MassAggregateBody* const)>& force_generator) noexcept;

    void updateForces() noexcept;
    void updateInternalForces(const PhxReal& total_time, const PhxReal& step_size) noexcept;

    void                          updateSurfaceParticleIndex() noexcept;
    [[nodiscard]] const PhxIndex& getSurfacePaticleIndex() const noexcept;

    [[nodiscard]] PhxIndex addSurfaceParticle(const PhxVec3& position) noexcept;

protected:
    PhxRealArray m_masses;
    PhxVec3Array m_positions;
    PhxVec3Array m_velocities;
    PhxVec3Array m_accelerations;
    PhxRealArray m_damping_values;
    PhxRealArray m_inverse_masses;
    PhxVec3Array m_accumulated_forces;
    PhxBoolArray m_is_fixed_values;
    PhxBoolArray m_is_valid_values;

    PhxIndex m_surface_particle_idx = 0;

    using ForceGeneratorList = std::vector<std::function<void(MassAggregateBody* const)>>;
    ForceGeneratorList m_force_generators;

    MassAggregateBodySpec m_spec;
};

class MassAggregateVolume : public MassAggregateBody
{

public:
    MassAggregateVolume(const PhxUint&               total_count,
                        const PhxInt&                row_count,
                        const PhxInt&                col_count,
                        const PhxInt&                slice_count,
                        const MassAggregateBodySpec& specification);

    virtual ~MassAggregateVolume() = default;

    [[nodiscard]] PhxIndex
    getIndex(const PhxInt& row_idx, const PhxInt& col_idx, const PhxInt& slice_idx) const noexcept;

    [[nodiscard]] PhxUvec3 getLocalCoordinates(const PhxIndex& idx) const noexcept;

    using MassAggregateBody::getPosition;
    [[nodiscard]] const PhxVec3&
    getPosition(const PhxIndex& row_idx, const PhxIndex& col_idx, const PhxIndex& slice_idx) const noexcept;

    using MassAggregateBody::getVelocity;
    [[nodiscard]] const PhxVec3&
    getVelocity(const PhxIndex& row_idx, const PhxIndex& col_idx, const PhxIndex& slice_idx) const noexcept;

    using MassAggregateBody::getAcceleration;
    [[nodiscard]] const PhxVec3&
    getAcceleration(const PhxIndex& row_idx, const PhxIndex& col_idx, const PhxIndex& slice_idx) const noexcept;

    using MassAggregateBody::getAccumulatedForce;
    [[nodiscard]] const PhxVec3&
    getAccumulatedForce(const PhxIndex& row_idx, const PhxIndex& col_idx, const PhxIndex& slice_idx) const noexcept;

    using MassAggregateBody::getDampingValue;
    [[nodiscard]] const PhxReal&
    getDampingValue(const PhxIndex& row_idx, const PhxIndex& col_idx, const PhxIndex& slice_idx) const noexcept;

    using MassAggregateBody::getInverseMass;
    [[nodiscard]] const PhxReal&
    getInverseMass(const PhxIndex& row_idx, const PhxIndex& col_idx, const PhxIndex& slice_idx) const noexcept;

    using MassAggregateBody::getMass;
    [[nodiscard]] const PhxReal&
    getMass(const PhxIndex& row_idx, const PhxIndex& col_idx, const PhxIndex& slice_idx) const noexcept;

    // PhxVec3 getPosition(const PhxIndex& row_idx, const PhxIndex& col_idx, const PhxIndex& slice_idx) noexcept;
    // PhxVec3 getVelocity(const PhxIndex& row_idx, const PhxIndex& col_idx, const PhxIndex& slice_idx) noexcept;
    // PhxVec3 getAcceleration(const PhxIndex& row_idx, const PhxIndex& col_idx, const PhxIndex& slice_idx) noexcept;
    // PhxVec3 getAccumulatedForce(const PhxIndex& row_idx, const PhxIndex& col_idx, const PhxIndex& slice_idx)
    // noexcept; PhxReal getDampingValue(const PhxIndex& row_idx, const PhxIndex& col_idx, const PhxIndex& slice_idx)
    // noexcept; PhxReal getInverseMass(const PhxIndex& row_idx, const PhxIndex& col_idx, const PhxIndex& slice_idx)
    // noexcept; PhxReal getMass(const PhxIndex& row_idx, const PhxIndex& col_idx, const PhxIndex& slice_idx) noexcept;

    void updateForces() noexcept;

    void addStructuralSpring(const PhxSpring& spring) noexcept;
    void addShearSpring(const PhxSpring& spring) noexcept;
    void addFlexionSpring(const PhxSpring& spring) noexcept;
    void addSurfaceSpring(const PhxSpring& spring) noexcept;
    void addInternalSpring(const PhxSpring& spring) noexcept;

    [[nodiscard]] const PhxArray<PhxSpring>& getStructuralSprings() const noexcept;
    [[nodiscard]] const PhxArray<PhxSpring>& getShearSprings() const noexcept;
    [[nodiscard]] const PhxArray<PhxSpring>& getFlexionSprings() const noexcept;
    [[nodiscard]] const PhxArray<PhxSpring>& getSurfaceSprings() const noexcept;
    [[nodiscard]] const PhxArray<PhxSpring>& getInternalSprings() const noexcept;


private:
    // SpringCoefficient m_structural_spring;
    // SpringCoefficient m_shear_spring;

    std::shared_ptr<SpringForceGenerator> m_structural_spring_generator;
    std::shared_ptr<SpringForceGenerator> m_shear_spring_generator;
    std::shared_ptr<SpringForceGenerator> m_flexion_spring_generator;
    std::shared_ptr<SpringForceGenerator> m_surface_spring_generator;
    std::shared_ptr<SpringForceGenerator> m_internal_spring_generator;
    // std::shared_ptr<SpringForceGenerator> m_torsion_spring_generator;

    PhxInt m_num_rows   = 0;
    PhxInt m_num_cols   = 0;
    PhxInt m_num_slices = 0;
};

} // namespace phx::mad

#endif
