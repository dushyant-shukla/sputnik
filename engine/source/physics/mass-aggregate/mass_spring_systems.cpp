#include "pch.h"
#include "mass_spring_systems.hpp"
#include "core/core.h"

namespace physics::mad
{

//////////////////////////////////// Mass Aggregate Curve ///////////////////////////////////////

MassAggregateCurve::MassAggregateCurve(const MassAggregateBodySpecification& specification)
    : m_num_masses{static_cast<int>(specification.resolution.x)}
{
    m_masses.resize(m_num_masses);
    m_positions.resize(m_num_masses);
    m_velocities.resize(m_num_masses);
    m_accelerations.resize(m_num_masses);
    m_damping_values.resize(m_num_masses);
    m_inverse_masses.resize(m_num_masses);
    m_accumulated_forces.resize(m_num_masses);
    m_is_fixed.resize(m_num_masses);

    vec3  half_scale = specification.scale * 0.5f;
    float dx         = specification.scale.x / static_cast<float>(m_num_masses - 1);

    for(int i = 0; i < m_num_masses; ++i)
    {
        vec3 position(dx * i, 0.0f, 0.0f);
        position += specification.center_position - half_scale;
        m_positions[i] = position;
        setMass(i, specification.mass); // using setMass() to calculate inverse mass as well
        m_accelerations[i]  = specification.acceleration;
        m_damping_values[i] = specification.damping;
        if(i == 0)
        {
            m_is_fixed[i] = true;
        }
    }

    // setting springs
    {
        for(int i = 0; i < m_num_masses; ++i)
        {
            if(i < (m_num_masses - 1))
            {
                Spring spring{.mass_a_idx            = static_cast<unsigned>(i),
                              .mass_b_idx            = static_cast<unsigned>(i + 1),
                              .rest_length           = (m_positions[i] - m_positions[i + 1]).length(),
                              .stiffness_coefficient = specification.spring_structural.stiffness_coefficient,
                              .damping_coefficient   = specification.spring_structural.damping_coefficient};
                m_structural_springs.addSpring(spring);
            }

            if(i < (m_num_masses - 2))
            {
                Spring spring{.mass_a_idx            = static_cast<unsigned>(i),
                              .mass_b_idx            = static_cast<unsigned>(i + 2),
                              .rest_length           = (m_positions[i] - m_positions[i + 2]).length(),
                              .stiffness_coefficient = specification.spring_flexion.stiffness_coefficient,
                              .damping_coefficient   = specification.spring_flexion.damping_coefficient};
                m_flexion_springs.addSpring(spring);
            }
        }
    }
    int a = 10;
}

void MassAggregateCurve::setStructuralSpring(const SpringSpecification& spring_spec) noexcept {}

void MassAggregateCurve::setFlexionSpring(const SpringSpecification& spring_spec) noexcept {}

const SpringForceGenerator& MassAggregateCurve::getStructuralSprings() const noexcept
{
    return m_structural_springs;
}

const SpringForceGenerator& MassAggregateCurve::getFlexionSprings() const noexcept
{
    return m_flexion_springs;
}

void MassAggregateCurve::updateInternalForces(const real& t, const real& dt) noexcept
{
    m_structural_springs.addForce(this);
    m_flexion_springs.addForce(this);
}

//////////////////////////////////// Mass Aggregate Voume ///////////////////////////////////////

MassAggregateVolume::MassAggregateVolume(const MassAggregateBodySpecification& specification)
    : m_num_rows(specification.resolution.y)
    , m_num_cols(specification.resolution.x)
    , m_num_slices(specification.resolution.z)
{

    size_t total_count = size_t(m_num_rows * m_num_cols * m_num_slices);
    m_masses.resize(total_count);
    m_positions.resize(total_count);
    m_velocities.resize(total_count);
    m_accelerations.resize(total_count);
    m_damping_values.resize(total_count);
    m_inverse_masses.resize(total_count);
    m_accumulated_forces.resize(total_count);
    m_is_fixed.resize(total_count);

    vec3 half_scale = specification.scale * 0.5f;
    // vec3  center     = specification.center_position + half_scale;
    float dx = specification.scale.x / static_cast<float>(m_num_cols - 1);
    float dy = specification.scale.y / static_cast<float>(m_num_rows - 1);
    float dz = specification.scale.z / static_cast<float>(m_num_slices - 1);

    for(int slice_idx = 0; slice_idx < m_num_slices; ++slice_idx)
    {
        for(int row_idx = 0; row_idx < m_num_rows; ++row_idx)
        {
            for(int col_idx = 0; col_idx < m_num_cols; ++col_idx)
            {
                unsigned index = getIndex(row_idx, col_idx, slice_idx);
                // ENGINE_INFO("Row: {}, Column: {}, Slide: {}, Particle index: {}", row_idx, col_idx, slice_idx,
                // index);
                vec3 position(dy * col_idx, dx * row_idx, dz * slice_idx);
                position += specification.center_position - half_scale;
                m_positions[index] = position;
                setMass(index, specification.mass); // using setMass() to calculate inverse mass as well
                m_accelerations[index]  = specification.acceleration;
                m_damping_values[index] = specification.damping;
            }
        }
    }

    // Setting springs
    for(int slice_idx = 0; slice_idx < m_num_slices; ++slice_idx)
    {
        for(int row_idx = 0; row_idx < m_num_rows; ++row_idx)
        {
            for(int col_idx = 0; col_idx < m_num_cols; ++col_idx)
            {
                unsigned index = getIndex(row_idx, col_idx, slice_idx);
                // ENGINE_INFO("Calculating  structural spring for: Row: {}, Column: {}, Slide: {}, Particle index: {}",
                //             row_idx,
                //             col_idx,
                //             slice_idx,
                //             index);

                // structural spring
                {
                    if(row_idx > 0)
                    {
                        unsigned neighbour_index = getIndex(row_idx - 1, col_idx, slice_idx);
                        Spring   spring{.mass_a_idx  = index,
                                        .mass_b_idx  = neighbour_index,
                                        .rest_length = (m_positions[index] - m_positions[neighbour_index]).length(),
                                        .stiffness_coefficient = specification.spring_structural.stiffness_coefficient,
                                        .damping_coefficient   = specification.spring_structural.damping_coefficient};
                        m_structural_springs.addSpring(spring);
                    }
                    if(col_idx > 0)
                    {
                        unsigned neighbour_index = getIndex(row_idx, col_idx - 1, slice_idx);
                        Spring   spring{.mass_a_idx  = index,
                                        .mass_b_idx  = neighbour_index,
                                        .rest_length = (m_positions[index] - m_positions[neighbour_index]).length(),
                                        .stiffness_coefficient = specification.spring_structural.stiffness_coefficient,
                                        .damping_coefficient   = specification.spring_structural.damping_coefficient};
                        m_structural_springs.addSpring(spring);
                    }
                    if(slice_idx > 0)
                    {
                        unsigned neighbour_index = getIndex(row_idx, col_idx, slice_idx - 1);
                        Spring   spring{.mass_a_idx  = index,
                                        .mass_b_idx  = neighbour_index,
                                        .rest_length = (m_positions[index] - m_positions[neighbour_index]).length(),
                                        .stiffness_coefficient = specification.spring_structural.stiffness_coefficient,
                                        .damping_coefficient   = specification.spring_structural.damping_coefficient};
                        m_structural_springs.addSpring(spring);
                    }
                    if(row_idx < (m_num_rows - 1))
                    {
                        unsigned neighbour_index = getIndex(row_idx + 1, col_idx, slice_idx);
                        Spring   spring{.mass_a_idx  = index,
                                        .mass_b_idx  = neighbour_index,
                                        .rest_length = (m_positions[index] - m_positions[neighbour_index]).length(),
                                        .stiffness_coefficient = specification.spring_structural.stiffness_coefficient,
                                        .damping_coefficient   = specification.spring_structural.damping_coefficient};
                        m_structural_springs.addSpring(spring);
                    }
                    if(col_idx < (m_num_cols - 1))
                    {
                        unsigned neighbour_index = getIndex(row_idx, col_idx + 1, slice_idx);
                        Spring   spring{.mass_a_idx  = index,
                                        .mass_b_idx  = neighbour_index,
                                        .rest_length = (m_positions[index] - m_positions[neighbour_index]).length(),
                                        .stiffness_coefficient = specification.spring_structural.stiffness_coefficient,
                                        .damping_coefficient   = specification.spring_structural.damping_coefficient};
                        m_structural_springs.addSpring(spring);
                    }
                    if(slice_idx < (m_num_slices - 1))
                    {
                        unsigned neighbour_index = getIndex(row_idx, col_idx, slice_idx + 1);
                        Spring   spring{.mass_a_idx  = index,
                                        .mass_b_idx  = neighbour_index,
                                        .rest_length = (m_positions[index] - m_positions[neighbour_index]).length(),
                                        .stiffness_coefficient = specification.spring_structural.stiffness_coefficient,
                                        .damping_coefficient   = specification.spring_structural.damping_coefficient};
                        m_structural_springs.addSpring(spring);
                    }
                }

                // bend (flexion) spring
                {
                    if((row_idx - 2) >= 0)
                    {
                        unsigned neighbour_index = getIndex(row_idx - 2, col_idx, slice_idx);
                        Spring   spring{.mass_a_idx  = index,
                                        .mass_b_idx  = neighbour_index,
                                        .rest_length = (m_positions[index] - m_positions[neighbour_index]).length(),
                                        .stiffness_coefficient = specification.spring_flexion.stiffness_coefficient,
                                        .damping_coefficient   = specification.spring_flexion.damping_coefficient};
                        m_flexion_springs.addSpring(spring);
                    }
                    if((col_idx - 2) >= 0)
                    {
                        unsigned neighbour_index = getIndex(row_idx, col_idx - 2, slice_idx);
                        Spring   spring{.mass_a_idx  = index,
                                        .mass_b_idx  = neighbour_index,
                                        .rest_length = (m_positions[index] - m_positions[neighbour_index]).length(),
                                        .stiffness_coefficient = specification.spring_flexion.stiffness_coefficient,
                                        .damping_coefficient   = specification.spring_flexion.damping_coefficient};
                        m_flexion_springs.addSpring(spring);
                    }
                    if((slice_idx - 2) >= 0)
                    {
                        unsigned neighbour_index = getIndex(row_idx, col_idx, slice_idx - 2);
                        Spring   spring{.mass_a_idx  = index,
                                        .mass_b_idx  = neighbour_index,
                                        .rest_length = (m_positions[index] - m_positions[neighbour_index]).length(),
                                        .stiffness_coefficient = specification.spring_flexion.stiffness_coefficient,
                                        .damping_coefficient   = specification.spring_flexion.damping_coefficient};
                        m_flexion_springs.addSpring(spring);
                    }
                    if((row_idx + 2) < m_num_rows)
                    {
                        unsigned neighbour_index = getIndex(row_idx + 2, col_idx, slice_idx);
                        Spring   spring{.mass_a_idx  = index,
                                        .mass_b_idx  = neighbour_index,
                                        .rest_length = (m_positions[index] - m_positions[neighbour_index]).length(),
                                        .stiffness_coefficient = specification.spring_flexion.stiffness_coefficient,
                                        .damping_coefficient   = specification.spring_flexion.damping_coefficient};
                        m_flexion_springs.addSpring(spring);
                    }
                    if((col_idx + 2) < m_num_cols)
                    {
                        unsigned neighbour_index = getIndex(row_idx, col_idx + 2, slice_idx);
                        Spring   spring{.mass_a_idx  = index,
                                        .mass_b_idx  = neighbour_index,
                                        .rest_length = (m_positions[index] - m_positions[neighbour_index]).length(),
                                        .stiffness_coefficient = specification.spring_flexion.stiffness_coefficient,
                                        .damping_coefficient   = specification.spring_flexion.damping_coefficient};
                        m_flexion_springs.addSpring(spring);
                    }
                    if((slice_idx + 2) < m_num_slices)
                    {
                        unsigned neighbour_index = getIndex(row_idx, col_idx, slice_idx + 2);
                        Spring   spring{.mass_a_idx  = index,
                                        .mass_b_idx  = neighbour_index,
                                        .rest_length = (m_positions[index] - m_positions[neighbour_index]).length(),
                                        .stiffness_coefficient = specification.spring_flexion.stiffness_coefficient,
                                        .damping_coefficient   = specification.spring_flexion.damping_coefficient};
                        m_flexion_springs.addSpring(spring);
                    }
                }

                // shear springs
                {
                    // diagonal elements in the same slice
                    unsigned neighbour_index = -1;
                    if(row_idx > 0 && col_idx > 0)
                    {
                        unsigned neighbour_index = getIndex(row_idx - 1, col_idx - 1, slice_idx);
                        Spring   spring{.mass_a_idx  = index,
                                        .mass_b_idx  = neighbour_index,
                                        .rest_length = (m_positions[index] - m_positions[neighbour_index]).length(),
                                        .stiffness_coefficient = specification.spring_shear.stiffness_coefficient,
                                        .damping_coefficient   = specification.spring_shear.damping_coefficient};
                        m_shear_springs.addSpring(spring);
                    }
                    if(row_idx < (m_num_rows - 1) && col_idx < (m_num_cols - 1))
                    {
                        unsigned neighbour_index = getIndex(row_idx + 1, col_idx + 1, slice_idx);
                        Spring   spring{.mass_a_idx  = index,
                                        .mass_b_idx  = neighbour_index,
                                        .rest_length = (m_positions[index] - m_positions[neighbour_index]).length(),
                                        .stiffness_coefficient = specification.spring_shear.stiffness_coefficient,
                                        .damping_coefficient   = specification.spring_shear.damping_coefficient};
                        m_shear_springs.addSpring(spring);
                    }
                    if(row_idx > 0 && col_idx < (m_num_cols - 1))
                    {
                        unsigned neighbour_index = getIndex(row_idx - 1, col_idx + 1, slice_idx);
                        Spring   spring{.mass_a_idx  = index,
                                        .mass_b_idx  = neighbour_index,
                                        .rest_length = (m_positions[index] - m_positions[neighbour_index]).length(),
                                        .stiffness_coefficient = specification.spring_shear.stiffness_coefficient,
                                        .damping_coefficient   = specification.spring_shear.damping_coefficient};
                        m_shear_springs.addSpring(spring);
                    }
                    if(row_idx < (m_num_rows - 1) && col_idx > 0)
                    {
                        unsigned neighbour_index = getIndex(row_idx + 1, col_idx - 1, slice_idx);
                        Spring   spring{.mass_a_idx  = index,
                                        .mass_b_idx  = neighbour_index,
                                        .rest_length = (m_positions[index] - m_positions[neighbour_index]).length(),
                                        .stiffness_coefficient = specification.spring_shear.stiffness_coefficient,
                                        .damping_coefficient   = specification.spring_shear.damping_coefficient};
                        m_shear_springs.addSpring(spring);
                    }

                    // diagonal elements in one slide before/after
                    if(row_idx > 0 && col_idx > 0 && slice_idx > 0)
                    {
                        unsigned neighbour_index = getIndex(row_idx - 1, col_idx - 1, slice_idx - 1);
                        Spring   spring{.mass_a_idx  = index,
                                        .mass_b_idx  = neighbour_index,
                                        .rest_length = (m_positions[index] - m_positions[neighbour_index]).length(),
                                        .stiffness_coefficient = specification.spring_shear.stiffness_coefficient,
                                        .damping_coefficient   = specification.spring_shear.damping_coefficient};
                        m_shear_springs.addSpring(spring);
                    }
                    if(row_idx < (m_num_rows - 1) && col_idx < (m_num_cols - 1) && slice_idx > 0)
                    {
                        unsigned neighbour_index = getIndex(row_idx + 1, col_idx + 1, slice_idx - 1);
                        Spring   spring{.mass_a_idx  = index,
                                        .mass_b_idx  = neighbour_index,
                                        .rest_length = (m_positions[index] - m_positions[neighbour_index]).length(),
                                        .stiffness_coefficient = specification.spring_shear.stiffness_coefficient,
                                        .damping_coefficient   = specification.spring_shear.damping_coefficient};
                        m_shear_springs.addSpring(spring);
                    }
                    if(row_idx > 0 && col_idx < (m_num_cols - 1) && slice_idx > 0)
                    {
                        unsigned neighbour_index = getIndex(row_idx - 1, col_idx + 1, slice_idx - 1);
                        Spring   spring{.mass_a_idx  = index,
                                        .mass_b_idx  = neighbour_index,
                                        .rest_length = (m_positions[index] - m_positions[neighbour_index]).length(),
                                        .stiffness_coefficient = specification.spring_shear.stiffness_coefficient,
                                        .damping_coefficient   = specification.spring_shear.damping_coefficient};
                        m_shear_springs.addSpring(spring);
                    }
                    if(row_idx < (m_num_rows - 1) && col_idx > 0 && slice_idx > 0)
                    {
                        unsigned neighbour_index = getIndex(row_idx + 1, col_idx - 1, slice_idx - 1);
                        Spring   spring{.mass_a_idx  = index,
                                        .mass_b_idx  = neighbour_index,
                                        .rest_length = (m_positions[index] - m_positions[neighbour_index]).length(),
                                        .stiffness_coefficient = specification.spring_shear.stiffness_coefficient,
                                        .damping_coefficient   = specification.spring_shear.damping_coefficient};
                        m_shear_springs.addSpring(spring);
                    }

                    // diagonal elements in one slide before/after
                    if(row_idx > 0 && col_idx > 0 && slice_idx < (m_num_slices - 1))
                    {
                        unsigned neighbour_index = getIndex(row_idx - 1, col_idx - 1, slice_idx + 1);
                        Spring   spring{.mass_a_idx  = index,
                                        .mass_b_idx  = neighbour_index,
                                        .rest_length = (m_positions[index] - m_positions[neighbour_index]).length(),
                                        .stiffness_coefficient = specification.spring_shear.stiffness_coefficient,
                                        .damping_coefficient   = specification.spring_shear.damping_coefficient};
                        m_shear_springs.addSpring(spring);
                    }
                    if(row_idx < (m_num_rows - 1) && col_idx < (m_num_cols - 1) && slice_idx < (m_num_slices - 1))
                    {
                        unsigned neighbour_index = getIndex(row_idx + 1, col_idx + 1, slice_idx + 1);
                        Spring   spring{.mass_a_idx  = index,
                                        .mass_b_idx  = neighbour_index,
                                        .rest_length = (m_positions[index] - m_positions[neighbour_index]).length(),
                                        .stiffness_coefficient = specification.spring_shear.stiffness_coefficient,
                                        .damping_coefficient   = specification.spring_shear.damping_coefficient};
                        m_shear_springs.addSpring(spring);
                    }
                    if(row_idx > 0 && col_idx < (m_num_cols - 1) && slice_idx < (m_num_slices - 1))
                    {
                        unsigned neighbour_index = getIndex(row_idx - 1, col_idx + 1, slice_idx + 1);
                        Spring   spring{.mass_a_idx  = index,
                                        .mass_b_idx  = neighbour_index,
                                        .rest_length = (m_positions[index] - m_positions[neighbour_index]).length(),
                                        .stiffness_coefficient = specification.spring_shear.stiffness_coefficient,
                                        .damping_coefficient   = specification.spring_shear.damping_coefficient};
                        m_shear_springs.addSpring(spring);
                    }
                    if(row_idx < (m_num_rows - 1) && col_idx > 0 && slice_idx < (m_num_slices - 1))
                    {
                        unsigned neighbour_index = getIndex(row_idx + 1, col_idx - 1, slice_idx + 1);
                        Spring   spring{.mass_a_idx  = index,
                                        .mass_b_idx  = neighbour_index,
                                        .rest_length = (m_positions[index] - m_positions[neighbour_index]).length(),
                                        .stiffness_coefficient = specification.spring_shear.stiffness_coefficient,
                                        .damping_coefficient   = specification.spring_shear.damping_coefficient};
                        m_shear_springs.addSpring(spring);
                    }
                }
            }
        }
    }

    int a = 10;
}

void MassAggregateVolume::setStructuralSpring(const SpringSpecification& spring_spec) noexcept
{
    // m_structural_spring.setRestLength(rest_length);
    // m_structural_spring.setSpringConstant(spring_constant);
}

void MassAggregateVolume::setShearSpring(const SpringSpecification& spring_spec) noexcept
{
    // m_shear_spring.setRestLength(rest_length);
    // m_shear_spring.setSpringConstant(spring_constant);
}

void MassAggregateVolume::setBendSpring(const SpringSpecification& spring_spec) noexcept
{
    // m_bend_spring.setRestLength(rest_length);
    // m_bend_spring.setSpringConstant(spring_constant);
}

const SpringForceGenerator& MassAggregateVolume::getStructuralSprings() const noexcept
{
    return m_structural_springs;
}

const SpringForceGenerator& MassAggregateVolume::getShearSprings() const noexcept
{
    return m_shear_springs;
}

const SpringForceGenerator& MassAggregateVolume::getBendSprings() const noexcept
{
    return m_flexion_springs;
}

unsigned MassAggregateVolume::getIndex(const int& row_idx, const int& col_idx, const int& slice_idx) const noexcept
{
    unsigned index = col_idx + m_num_cols * (row_idx + m_num_rows * slice_idx);
    return index;
}

uvec3 MassAggregateVolume::getLocalCoordinates(const unsigned& idx) const noexcept
{
    // NOTE: This is the reverse of the formula that is used in MassAggregateVolume::getIndex() function.
    unsigned index   = idx;
    unsigned col_idx = index % m_num_cols;  // reverse of (col_idx + m_num_cols) part of the formula above (getIndex())
    index = (index - col_idx) / m_num_cols; // index = (row_idx + m_num_rows * slice_idx) (see the getIndex())
    unsigned row_idx   = index % m_num_rows;
    unsigned slice_idx = index / m_num_rows;

    return {row_idx, col_idx, slice_idx};
}

vec3 MassAggregateVolume::getPosition(const unsigned& row_idx,
                                      const unsigned& col_idx,
                                      const unsigned& slice_idx) const noexcept
{
    unsigned idx = getIndex(row_idx, col_idx, slice_idx);
    return m_positions[idx];
}

vec3 MassAggregateVolume::getVelocity(const unsigned& row_idx,
                                      const unsigned& col_idx,
                                      const unsigned& slice_idx) const noexcept
{
    unsigned idx = getIndex(row_idx, col_idx, slice_idx);
    return m_velocities[idx];
}

vec3 MassAggregateVolume::getAcceleration(const unsigned& row_idx,
                                          const unsigned& col_idx,
                                          const unsigned& slice_idx) const noexcept
{
    unsigned idx = getIndex(row_idx, col_idx, slice_idx);
    return m_accelerations[idx];
}

real MassAggregateVolume::getDampingValue(const unsigned& row_idx,
                                          const unsigned& col_idx,
                                          const unsigned& slice_idx) const noexcept
{
    unsigned idx = getIndex(row_idx, col_idx, slice_idx);
    return m_damping_values[idx];
}

real MassAggregateVolume::getInverseMass(const unsigned& row_idx,
                                         const unsigned& col_idx,
                                         const unsigned& slice_idx) const noexcept
{
    unsigned idx = getIndex(row_idx, col_idx, slice_idx);
    return m_inverse_masses[idx];
}

real MassAggregateVolume::getMass(const unsigned& row_idx,
                                  const unsigned& col_idx,
                                  const unsigned& slice_idx) const noexcept
{
    unsigned idx = getIndex(row_idx, col_idx, slice_idx);
    return m_masses[idx];
}

vec3 MassAggregateVolume::getAccumulatedForce(const unsigned& row_idx,
                                              const unsigned& col_idx,
                                              const unsigned& slice_idx) const noexcept
{
    unsigned idx = getIndex(row_idx, col_idx, slice_idx);
    return m_accumulated_forces[idx];
}

void MassAggregateVolume::updateInternalForces(const real& t, const real& dt) noexcept
{
    m_structural_springs.addForce(this);
    m_shear_springs.addForce(this);
    m_flexion_springs.addForce(this);
}

} // namespace physics::mad
