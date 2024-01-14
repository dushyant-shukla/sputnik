#pragma once

#include "mass_aggregate_system.hpp"

#include <vector.hpp>

namespace physics::mad
{

using namespace ramanujan;
using namespace ramanujan::experimental;

struct SpringSpecification
{
    real rest_length;
    real stiffness;
};

struct MassAggregateBodySpecification
{
    vec3  scale{1, 1, 1};      // scale along x, y, z axis
    uvec3 resolution{1, 1, 1}; // particles count along x, y, z axis
    real  mass{1.0f};
    real  damping{1.0f}; // viscosity maybe?
    vec3  acceleration{0.0f, -9.8f, 0.0f};

    SpringSpecification spring_structural;
    SpringSpecification spring_shear;
    SpringSpecification spring_flexion;

    vec3 center_position{0.0f};
    vec3 initial_velocity;
    bool randomize_initial_velocity;
    bool make_unmovable;
};

class MassAggregateCurve : public MassAggregateSystem
{
public:
    MassAggregateCurve(const unsigned& length);

    void setSpring(const real& rest_length, const real& stiffness) noexcept;

protected:
    SpringForceGenerator m_spring;
};

class MassAggregateSurface : public MassAggregateSystem
{
public:
    MassAggregateSurface(const unsigned& rows, const unsigned& cols);

    void setStructuralSpring(const real& rest_length, const real& stiffness) noexcept;
    void setShearSpring(const real& rest_length, const real& stiffness) noexcept;
    void setBendSpring(const real& rest_length, const real& stiffness) noexcept;

    unsigned getIndex(const unsigned& row_idx, const unsigned& col_idx) const noexcept;
    uvec2    getLocalPosition(const unsigned& idx) const noexcept;

protected:
    SpringForceGenerator m_structural_SpringForceGenerator;
    SpringForceGenerator m_shear_SpringForceGenerator;
    SpringForceGenerator m_bend_SpringForceGenerator;
};

class MassAggregateVolume : public MassAggregateSystem
{
public:
    MassAggregateVolume(const MassAggregateBodySpecification& specification);

    virtual ~MassAggregateVolume() = default;

    void                        setStructuralSpring(const real& rest_length, const real& stiffness) noexcept;
    void                        setShearSpring(const real& rest_length, const real& stiffness) noexcept;
    void                        setBendSpring(const real& rest_length, const real& stiffness) noexcept;
    const SpringForceGenerator& getStructuralSprings() const noexcept;
    const SpringForceGenerator& getShearSprings() const noexcept;
    const SpringForceGenerator& getBendSprings() const noexcept;

    unsigned getIndex(const int& row_idx, const int& col_idx, const int& slice_idx) const noexcept;
    uvec3    getLocalCoordinates(const unsigned& idx) const noexcept;

    vec3 getPosition(const unsigned& row_idx, const unsigned& col_idx, const unsigned& slice_idx) const noexcept;
    vec3 getVelocity(const unsigned& row_idx, const unsigned& col_idx, const unsigned& slice_idx) const noexcept;
    vec3 getAcceleration(const unsigned& row_idx, const unsigned& col_idx, const unsigned& slice_idx) const noexcept;
    real getDampingValue(const unsigned& row_idx, const unsigned& col_idx, const unsigned& slice_idx) const noexcept;
    real getInverseMass(const unsigned& row_idx, const unsigned& col_idx, const unsigned& slice_idx) const noexcept;
    real getMass(const unsigned& row_idx, const unsigned& col_idx, const unsigned& slice_idx) const noexcept;
    vec3
    getAccumulatedForce(const unsigned& row_idx, const unsigned& col_idx, const unsigned& slice_idx) const noexcept;

    virtual void updateInternalForces(const real& t, const real& dt) noexcept override;

protected:
    SpringForceGenerator m_structural_springs;
    SpringForceGenerator m_shear_springs;
    SpringForceGenerator m_flexion_springs;

    int m_num_rows;
    int m_num_cols;
    int m_num_slices;
};

} // namespace physics::mad
