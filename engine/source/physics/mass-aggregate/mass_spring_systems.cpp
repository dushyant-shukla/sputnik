#include "pch.h"
#include "mass_spring_systems.hpp"
#include "core/core.h"

namespace physics::mad
{

MassAggregateVolume::MassAggregateVolume(const unsigned& rows, const unsigned& cols, const unsigned& slices)
    : m_num_rows(rows)
    , m_num_cols(cols)
    , m_num_slices(slices)
{
    size_t grid_size = size_t(m_num_rows * m_num_cols * m_num_slices);
    m_masses.resize(grid_size);
    m_positions.resize(grid_size);
    m_velocities.resize(grid_size);
    m_accelerations.resize(grid_size);
    m_damping_values.resize(grid_size);
    m_inverse_masses.resize(grid_size);
    m_accumulated_forces.resize(grid_size);
    m_is_fixed.resize(grid_size);

    float dx = 1.0f / static_cast<float>(m_num_rows - 1);
    float dy = 1.0f / static_cast<float>(m_num_cols - 1);
    float dz = 1.0f / static_cast<float>(m_num_slices - 1);

    for(int slice_idx = 0; slice_idx < slices; ++slice_idx)
    {
        for(int row_idx = 0; row_idx < rows; ++row_idx)
        {
            for(int col_idx = 0; col_idx < cols; ++col_idx)
            {
                int index = getIndex(row_idx, col_idx, slice_idx);
                ENGINE_INFO("Row: {}, Column: {}, Slide: {}, Particle index: {}", row_idx, col_idx, slice_idx, index);
                m_positions[index] = vec3(dy * col_idx, dx * row_idx, dz * slice_idx);
            }
        }
    }

    // Setting springs
    for(int slice_idx = 0; slice_idx < slices; ++slice_idx)
    {
        for(int row_idx = 0; row_idx < rows; ++row_idx)
        {
            for(int col_idx = 0; col_idx < cols; ++col_idx)
            {
                int index = getIndex(row_idx, col_idx, slice_idx);

                // structural spring
                {
                    if(row_idx < rows - 1)
                    {
                        unsigned neighbour_index = getIndex(row_idx + 1, col_idx, slice_idx);
                        m_structural_spring.addMassPair(index, neighbour_index);
                    }
                    if(col_idx < cols - 1)
                    {
                        unsigned neighbour_index = getIndex(row_idx, col_idx + 1, slice_idx);
                        m_structural_spring.addMassPair(index, neighbour_index);
                    }
                    if(slice_idx < slices - 1)
                    {
                        unsigned neighbour_index = getIndex(row_idx, col_idx, slice_idx + 1);
                        m_structural_spring.addMassPair(index, neighbour_index);
                    }
                }

                // shear spring
                {
                    if(row_idx < rows - 1 && col_idx < cols)
                    {
                        unsigned neighbour_index = getIndex(row_idx + 1, col_idx, slice_idx);
                        m_shear_spring.addMassPair(index, neighbour_index);
                    }
                    if(row_idx < rows && col_idx < cols - 1)
                    {
                        unsigned neighbour_index = getIndex(row_idx, col_idx + 1, slice_idx);
                        m_shear_spring.addMassPair(index, neighbour_index);
                    }
                    if(row_idx < rows - 1 && col_idx < cols - 1)
                    {
                        unsigned neighbour_index = getIndex(row_idx + 1, col_idx + 1, slice_idx);
                        m_shear_spring.addMassPair(index, neighbour_index);
                    }
                }

                // bend spring
                {
                    if(row_idx < rows - 2)
                    {
                        unsigned neighbour_index = getIndex(row_idx + 2, col_idx, slice_idx);
                        m_bend_spring.addMassPair(index, neighbour_index);
                    }
                    if(col_idx < cols - 2)
                    {
                        unsigned neighbour_index = getIndex(row_idx, col_idx + 2, slice_idx);
                        m_bend_spring.addMassPair(index, neighbour_index);
                    }
                    if(slice_idx < slices - 2)
                    {
                        unsigned neighbour_index = getIndex(row_idx, col_idx, slice_idx + 2);
                        m_bend_spring.addMassPair(index, neighbour_index);
                    }
                }
            }
        }
    }
}

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

    vec3  half_scale = specification.scale * 0.5f;
    vec3  center     = specification.center_position + half_scale;
    float dx         = specification.scale.x / static_cast<float>(m_num_cols - 1);
    float dy         = specification.scale.y / static_cast<float>(m_num_rows - 1);
    float dz         = specification.scale.z / static_cast<float>(m_num_slices - 1);

    for(int slice_idx = 0; slice_idx < m_num_slices; ++slice_idx)
    {
        for(int row_idx = 0; row_idx < m_num_rows; ++row_idx)
        {
            for(int col_idx = 0; col_idx < m_num_cols; ++col_idx)
            {
                unsigned index = getIndex(row_idx, col_idx, slice_idx);
                ENGINE_INFO("Row: {}, Column: {}, Slide: {}, Particle index: {}", row_idx, col_idx, slice_idx, index);
                vec3 position(dy * col_idx, dx * row_idx, dz * slice_idx);
                position += specification.center_position - half_scale;
                m_positions[index] = position;
                m_masses[index]    = specification.mass;
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
                ENGINE_INFO("Calculating  structural spring for: Row: {}, Column: {}, Slide: {}, Particle index: {}",
                            row_idx,
                            col_idx,
                            slice_idx,
                            index);

                // structural spring
                {
                    if(row_idx > 0)
                    {
                        unsigned neighbour_index = getIndex(row_idx - 1, col_idx, slice_idx);
                        m_structural_spring.addMassPair(index, neighbour_index);
                    }
                    if(col_idx > 0)
                    {
                        unsigned neighbour_index = getIndex(row_idx, col_idx - 1, slice_idx);
                        m_structural_spring.addMassPair(index, neighbour_index);
                    }
                    if(slice_idx > 0)
                    {
                        unsigned neighbour_index = getIndex(row_idx, col_idx, slice_idx - 1);
                        m_structural_spring.addMassPair(index, neighbour_index);
                    }
                    if(row_idx < (m_num_rows - 1))
                    {
                        unsigned neighbour_index = getIndex(row_idx + 1, col_idx, slice_idx);
                        m_structural_spring.addMassPair(index, neighbour_index);
                    }
                    if(col_idx < (m_num_cols - 1))
                    {
                        unsigned neighbour_index = getIndex(row_idx, col_idx + 1, slice_idx);
                        m_structural_spring.addMassPair(index, neighbour_index);
                    }
                    if(slice_idx < (m_num_slices - 1))
                    {
                        unsigned neighbour_index = getIndex(row_idx, col_idx, slice_idx + 1);
                        m_structural_spring.addMassPair(index, neighbour_index);
                    }
                }

                // bend (flexion) spring
                {
                    if((row_idx - 2) >= 0)
                    {
                        unsigned neighbour_index = getIndex(row_idx - 2, col_idx, slice_idx);
                        m_bend_spring.addMassPair(index, neighbour_index);
                    }
                    if((col_idx - 2) >= 0)
                    {
                        unsigned neighbour_index = getIndex(row_idx, col_idx - 2, slice_idx);
                        m_bend_spring.addMassPair(index, neighbour_index);
                    }
                    if((slice_idx - 2) >= 0)
                    {
                        unsigned neighbour_index = getIndex(row_idx, col_idx, slice_idx - 2);
                        m_bend_spring.addMassPair(index, neighbour_index);
                    }
                    if((row_idx + 2) < m_num_rows)
                    {
                        unsigned neighbour_index = getIndex(row_idx + 2, col_idx, slice_idx);
                        m_bend_spring.addMassPair(index, neighbour_index);
                    }
                    if((col_idx + 2) < m_num_cols)
                    {
                        unsigned neighbour_index = getIndex(row_idx, col_idx + 2, slice_idx);
                        m_bend_spring.addMassPair(index, neighbour_index);
                    }
                    if((slice_idx + 2) < m_num_slices)
                    {
                        unsigned neighbour_index = getIndex(row_idx, col_idx, slice_idx + 2);
                        m_bend_spring.addMassPair(index, neighbour_index);
                    }
                }

                // shear springs
                {

                    // diagonal elements

                    // r, c, s

                    // r, c, s + 1 -> r -

                    // reduce the problem to 2D
                    // for [x,y]: [x-1, y-1], [x+1, y+1], [x-1, y+1], [x+1, y-1]

                    // diagonal elements in the same slice
                    // diagonal elements in the same row
                    // diagonal elements in the same column

                    // diagonal elements in the same slice
                    if(row_idx > 0 && col_idx > 0)
                    {
                        unsigned neighbour_index = getIndex(row_idx - 1, col_idx - 1, slice_idx);
                        m_shear_spring.addMassPair(index, neighbour_index);
                    }
                    if(row_idx < (m_num_rows - 1) && col_idx < (m_num_cols - 1))
                    {
                        unsigned neighbour_index = getIndex(row_idx + 1, col_idx + 1, slice_idx);
                        m_shear_spring.addMassPair(index, neighbour_index);
                    }
                    if(row_idx > 0 && col_idx < (m_num_cols - 1))
                    {
                        unsigned neighbour_index = getIndex(row_idx - 1, col_idx + 1, slice_idx);
                        m_shear_spring.addMassPair(index, neighbour_index);
                    }
                    if(row_idx < (m_num_rows - 1) && col_idx > 0)
                    {
                        unsigned neighbour_index = getIndex(row_idx + 1, col_idx - 1, slice_idx);
                        m_shear_spring.addMassPair(index, neighbour_index);
                    }

                    // diagonal elements in one slide before/after
                    if(row_idx > 0 && col_idx > 0 && slice_idx > 0)
                    {
                        unsigned neighbour_index = getIndex(row_idx - 1, col_idx - 1, slice_idx - 1);
                        m_shear_spring.addMassPair(index, neighbour_index);
                    }
                    if(row_idx < (m_num_rows - 1) && col_idx < (m_num_cols - 1) && slice_idx > 0)
                    {
                        unsigned neighbour_index = getIndex(row_idx + 1, col_idx + 1, slice_idx - 1);
                        m_shear_spring.addMassPair(index, neighbour_index);
                    }
                    if(row_idx > 0 && col_idx < (m_num_cols - 1) && slice_idx > 0)
                    {
                        unsigned neighbour_index = getIndex(row_idx - 1, col_idx + 1, slice_idx - 1);
                        m_shear_spring.addMassPair(index, neighbour_index);
                    }
                    if(row_idx < (m_num_rows - 1) && col_idx > 0 && slice_idx > 0)
                    {
                        unsigned neighbour_index = getIndex(row_idx + 1, col_idx - 1, slice_idx - 1);
                        m_shear_spring.addMassPair(index, neighbour_index);
                    }


                    // diagonal elements in one slide before/after
                    if(row_idx > 0 && col_idx > 0 && slice_idx < (m_num_slices - 1))
                    {
                        unsigned neighbour_index = getIndex(row_idx - 1, col_idx - 1, slice_idx + 1);
                        m_shear_spring.addMassPair(index, neighbour_index);
                    }
                    if (row_idx < (m_num_rows - 1) && col_idx < (m_num_cols - 1) && slice_idx < (m_num_slices - 1))
                    {
						unsigned neighbour_index = getIndex(row_idx + 1, col_idx + 1, slice_idx + 1);
						m_shear_spring.addMassPair(index, neighbour_index);
					}
                    if (row_idx > 0 && col_idx < (m_num_cols - 1) && slice_idx < (m_num_slices - 1))
                    {
                        unsigned neighbour_index = getIndex(row_idx - 1, col_idx + 1, slice_idx + 1);
                        m_shear_spring.addMassPair(index, neighbour_index);
                    }
                    if(row_idx < (m_num_rows - 1) && col_idx > 0 && slice_idx < (m_num_slices - 1))
                    {
                        unsigned neighbour_index = getIndex(row_idx + 1, col_idx - 1, slice_idx + 1);
                        m_shear_spring.addMassPair(index, neighbour_index);
                    }
                }
            }
        }
    }

    int a = 10;
}

void MassAggregateVolume::setStructuralSpring(const real& rest_length, const real& spring_constant) noexcept
{
    m_structural_spring.setRestLength(rest_length);
    m_structural_spring.setSpringConstant(spring_constant);
}

void MassAggregateVolume::setShearSpring(const real& rest_length, const real& spring_constant) noexcept
{
    m_shear_spring.setRestLength(rest_length);
    m_shear_spring.setSpringConstant(spring_constant);
}

void MassAggregateVolume::setBendSpring(const real& rest_length, const real& spring_constant) noexcept
{
    m_bend_spring.setRestLength(rest_length);
    m_bend_spring.setSpringConstant(spring_constant);
}

const SpringForceGenerator& MassAggregateVolume::getStructuralSpring() const noexcept
{
    return m_structural_spring;
}

const SpringForceGenerator& MassAggregateVolume::getShearSpring() const noexcept
{
    return m_shear_spring;
}

const SpringForceGenerator& MassAggregateVolume::getBendSpring() const noexcept
{
    return m_bend_spring;
}

unsigned MassAggregateVolume::getIndex(const int& row_idx, const int& col_idx, const int& slice_idx) const noexcept
{
    unsigned index = col_idx + m_num_cols * (row_idx + m_num_rows * slice_idx);
    return index;
}

uvec3 MassAggregateVolume::getLocalCoordinates(const unsigned& idx) const noexcept
{
    // unsigned col_idx   = idx % m_num_cols;
    // unsigned row_idx   = (idx / m_num_cols) % m_num_rows;
    // unsigned slice_idx = idx / (m_num_rows * m_num_cols);

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
    // update spring forces
}

} // namespace physics::mad
