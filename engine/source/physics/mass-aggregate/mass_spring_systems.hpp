#pragma once

#include "mass_aggregate_system.hpp"

#include <vector.hpp>

namespace physics::mad
{

using namespace ramanujan;
using namespace ramanujan::experimental;

struct MassAggregateBodySpecification
{
    vec3  scale{1, 1, 1};      // scale along x, y, z axis
    uvec3 resolution{1, 1, 1}; // particles count along x, y, z axis
    real  mass;
    real  damping; // viscosity maybe?

    real spring_stiffness_structural;
    real spring_rest_length_structural;
    real spring_stiffness_shear;
    real spring_rest_length_shear;
    real spring_stiffness_bend;
    real spring_rest_length_bend;

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
    MassAggregateVolume(const unsigned& rows, const unsigned& cols, const unsigned& slices);
    MassAggregateVolume(const MassAggregateBodySpecification& specification);

    virtual ~MassAggregateVolume() = default;

    void                        setStructuralSpring(const real& rest_length, const real& stiffness) noexcept;
    void                        setShearSpring(const real& rest_length, const real& stiffness) noexcept;
    void                        setBendSpring(const real& rest_length, const real& stiffness) noexcept;
    const SpringForceGenerator& getStructuralSpring() const noexcept;
    const SpringForceGenerator& getShearSpring() const noexcept;
    const SpringForceGenerator& getBendSpring() const noexcept;

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
    SpringForceGenerator m_structural_spring;
    SpringForceGenerator m_shear_spring;
    SpringForceGenerator m_bend_spring;

    int m_num_rows;
    int m_num_cols;
    int m_num_slices;
};

} // namespace physics::mad
