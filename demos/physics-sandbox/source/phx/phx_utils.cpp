#include "phx_utils.hpp"
#include "geometry_queries.hpp"

#include "core/logging/logging_core.h"

namespace phx
{

std::shared_ptr<PhxTriangleMesh> phxCookTriangleMesh(const PhxVec3Array&  input_vertices,
                                                     const PhxIndexArray& input_indices)
{
    auto mesh = std::make_shared<phx::PhxTriangleMesh>(static_cast<uint32_t>(input_indices.size() / 3));
    for(PhxIndex i = 0; i < input_indices.size(); i += 3)
    {
        PhxIndex         index = input_indices[i];
        phx::PhxTriangle triangle{
            .a = glm::vec3(input_vertices[index].x, input_vertices[index].y, input_vertices[index].z),
            .b = glm::vec3(input_vertices[index + 1].x, input_vertices[index + 1].y, input_vertices[index + 1].z),
            .c = glm::vec3(input_vertices[index + 2].x, input_vertices[index + 2].y, input_vertices[index + 2].z)};
        triangle.calculateCentroid();
        mesh->addTriangle(triangle);
    }
    mesh->buildAccelerationStructure();
    return mesh;
}

static mad::PhxSpring phxCreateSpring(const PhxIndex& mass_a,
                                      const PhxIndex& mass_b,
                                      const PhxReal&  rest_length,
                                      const PhxReal&  stiffness,
                                      const PhxReal&  damping)
{
    mad::PhxSpring spring{.mass_a_idx      = mass_a,
                          .mass_b_idx      = mass_b,
                          .rest_length     = rest_length,
                          .ks = stiffness,
                          .kd   = damping};
    return spring;
}

std::shared_ptr<mad::MassAggregateVolume> phxCookMassAggregateVolume(const std::shared_ptr<PhxTriangleMesh>& input_mesh,
                                                                     const mad::MassAggregateBodySpec&       spec,
                                                                     const PhxBool& randomize_sampling_direction,
                                                                     const PhxVec3& sampling_direction)
{
    PhxAABB root_aabb  = input_mesh->getBvh()->getNodes()[0].aabb;
    PhxVec3 min_bounds = root_aabb.min;
    PhxVec3 max_bounds = root_aabb.max;

    PhxInt num_rows = 0;
    for(float x = min_bounds.x; x <= max_bounds.x; x += spec.step.x)
    {
        ++num_rows;
    }

    PhxInt num_cols = 0;
    for(float y = min_bounds.y; y <= max_bounds.y; y += spec.step.y)
    {
        ++num_cols;
    }

    PhxInt num_slices = 0;
    for(float z = min_bounds.z; z <= max_bounds.z; z += spec.step.z)
    {
        ++num_slices;
    }

    PhxUint candidate_points = num_rows * num_cols * num_slices;
    PhxUint mesh_points      = 0;

    // PhxUint candidate_points = 0;
    // for(float x = min_bounds.x; x <= max_bounds.x; x += spec.step.x)
    //{
    //     for(float y = min_bounds.y; y <= max_bounds.y; y += spec.step.y)
    //     {
    //         for(float z = min_bounds.z; z <= max_bounds.z; z += spec.step.z)
    //         {
    //             ++candidate_points;
    //         }
    //     }
    // }

    std::shared_ptr<mad::MassAggregateVolume> body =
        std::make_shared<mad::MassAggregateVolume>(candidate_points, num_rows, num_cols, num_slices, spec);

    // Find the points that are inside the mesh
    {
        PhxInt i = 0;
        for(float x = min_bounds.x; x <= max_bounds.x; x += spec.step.x, ++i)
        {
            PhxInt j = 0;
            for(float y = min_bounds.y; y <= max_bounds.y; y += spec.step.y, ++j)
            {
                PhxInt k = 0;
                for(float z = min_bounds.z; z <= max_bounds.z; z += spec.step.z, ++k)
                {
                    PhxRay ray = {};
                    ray.origin = PhxVec3(x, y, z);

                    if(randomize_sampling_direction)
                    {
                        ray.direction = phxGenerateRandomUnitVector();
                    }
                    else
                    {
                        ray.direction = sampling_direction;
                    }

                    PhxArray<phx::PhxRaycastResult> results;
                    PhxIndex                        mass_index = body->getIndex(i, j, k);
                    if(phxRaycast(ray, *input_mesh.get(), results, phx::PhxQueryMode::AllHits) &&
                       (results.size() % 2 != 0))
                    {
                        ++mesh_points;
                        body->setPosition(mass_index, PhxVec3(x, y, z));
                        body->setAcceleration(mass_index, spec.acceleration);
                        body->setMass(mass_index, spec.mass);
                        body->setDamping(mass_index, spec.damping);
                        body->setIsValid(mass_index, true);

                        if(spec.randomize_initial_velocity)
                        {
                            // Todo:: Maybe randomize the magnitude of the velocity also
                            body->setVelocity(mass_index, phxGenerateRandomUnitVector() * spec.initial_velocity);
                        }
                        else
                        {
                            body->setVelocity(mass_index, spec.initial_velocity);
                        }
                    }
                    else
                    {
                        body->setIsValid(mass_index, false);
                        body->setPosition(mass_index, PhxVec3(x, y, z));
                    }
                }
            }
        }
    }

    for(int row_idx = 0; row_idx < num_rows; ++row_idx)
    {
        for(int col_idx = 0; col_idx < num_cols; ++col_idx)
        {
            for(int slice_idx = 0; slice_idx < num_slices; ++slice_idx)
            {
                PhxIndex mass_index = body->getIndex(row_idx, col_idx, slice_idx);
                if(body->getIsValid(mass_index))
                {
                    // structure springs
                    {
                        if(row_idx > 0)
                        {
                            unsigned neighbour_index = body->getIndex(row_idx - 1, col_idx, slice_idx);
                            if(body->getIsValid(neighbour_index))
                            {
                                PhxReal rest_length =
                                    phx_length(body->getPosition(mass_index) - body->getPosition(neighbour_index));
                                body->addStructuralSpring(phxCreateSpring(mass_index,
                                                                          neighbour_index,
                                                                          rest_length,
                                                                          spec.structural_spring_coeffs.ks,
                                                                          spec.structural_spring_coeffs.kd));
                            }
                        }

                        if(col_idx > 0)
                        {
                            unsigned neighbour_index = body->getIndex(row_idx, col_idx - 1, slice_idx);
                            if(body->getIsValid(neighbour_index))
                            {
                                PhxReal rest_length =
                                    phx_length(body->getPosition(mass_index) - body->getPosition(neighbour_index));
                                body->addStructuralSpring(phxCreateSpring(mass_index,
                                                                          neighbour_index,
                                                                          rest_length,
                                                                          spec.structural_spring_coeffs.ks,
                                                                          spec.structural_spring_coeffs.kd));
                            }
                        }

                        if(slice_idx > 0)
                        {
                            unsigned neighbour_index = body->getIndex(row_idx, col_idx, slice_idx - 1);
                            if(body->getIsValid(neighbour_index))
                            {
                                PhxReal rest_length =
                                    phx_length(body->getPosition(mass_index) - body->getPosition(neighbour_index));
                                body->addStructuralSpring(phxCreateSpring(mass_index,
                                                                          neighbour_index,
                                                                          rest_length,
                                                                          spec.structural_spring_coeffs.ks,
                                                                          spec.structural_spring_coeffs.kd));
                            }
                        }

                        if(row_idx < (num_rows - 1))
                        {
                            unsigned neighbour_index = body->getIndex(row_idx + 1, col_idx, slice_idx);
                            if(body->getIsValid(neighbour_index))
                            {
                                PhxReal rest_length =
                                    phx_length(body->getPosition(mass_index) - body->getPosition(neighbour_index));
                                body->addStructuralSpring(phxCreateSpring(mass_index,
                                                                          neighbour_index,
                                                                          rest_length,
                                                                          spec.structural_spring_coeffs.ks,
                                                                          spec.structural_spring_coeffs.kd));
                            }
                        }

                        if(col_idx < (num_cols - 1))
                        {
                            unsigned neighbour_index = body->getIndex(row_idx, col_idx + 1, slice_idx);
                            if(body->getIsValid(neighbour_index))
                            {
                                PhxReal rest_length =
                                    phx_length(body->getPosition(mass_index) - body->getPosition(neighbour_index));
                                body->addStructuralSpring(phxCreateSpring(mass_index,
                                                                          neighbour_index,
                                                                          rest_length,
                                                                          spec.structural_spring_coeffs.ks,
                                                                          spec.structural_spring_coeffs.kd));
                            }
                        }

                        if(slice_idx < (num_slices - 1))
                        {
                            unsigned neighbour_index = body->getIndex(row_idx, col_idx, slice_idx + 1);
                            if(body->getIsValid(neighbour_index))
                            {
                                PhxReal rest_length =
                                    phx_length(body->getPosition(mass_index) - body->getPosition(neighbour_index));
                                body->addStructuralSpring(phxCreateSpring(mass_index,
                                                                          neighbour_index,
                                                                          rest_length,
                                                                          spec.structural_spring_coeffs.ks,
                                                                          spec.structural_spring_coeffs.kd));
                            }
                        }
                    }

                    // bend (flexion) spring
                    {
                        if((row_idx - 2) >= 0)
                        {
                            PhxIndex neighbour_index = body->getIndex(row_idx - 2, col_idx, slice_idx);
                            if(body->getIsValid(neighbour_index))
                            {
                                PhxReal rest_length =
                                    phx_length(body->getPosition(mass_index) - body->getPosition(neighbour_index));
                                body->addFlexionSpring(phxCreateSpring(mass_index,
                                                                       neighbour_index,
                                                                       rest_length,
                                                                       spec.flexion_spring_coeffs.ks,
                                                                       spec.flexion_spring_coeffs.kd));
                            }
                        }
                        if((col_idx - 2) >= 0)
                        {
                            PhxIndex neighbour_index = body->getIndex(row_idx, col_idx - 2, slice_idx);
                            if(body->getIsValid(neighbour_index))
                            {
                                PhxReal rest_length =
                                    phx_length(body->getPosition(mass_index) - body->getPosition(neighbour_index));
                                body->addFlexionSpring(phxCreateSpring(mass_index,
                                                                       neighbour_index,
                                                                       rest_length,
                                                                       spec.flexion_spring_coeffs.ks,
                                                                       spec.flexion_spring_coeffs.kd));
                            }
                        }
                        if((slice_idx - 2) >= 0)
                        {
                            PhxIndex neighbour_index = body->getIndex(row_idx, col_idx, slice_idx - 2);
                            if(body->getIsValid(neighbour_index))
                            {
                                PhxReal rest_length =
                                    phx_length(body->getPosition(mass_index) - body->getPosition(neighbour_index));
                                body->addFlexionSpring(phxCreateSpring(mass_index,
                                                                       neighbour_index,
                                                                       rest_length,
                                                                       spec.flexion_spring_coeffs.ks,
                                                                       spec.flexion_spring_coeffs.kd));
                            }
                        }
                        if((row_idx + 2) < num_rows)
                        {
                            PhxIndex neighbour_index = body->getIndex(row_idx + 2, col_idx, slice_idx);
                            if(body->getIsValid(neighbour_index))
                            {
                                PhxReal rest_length =
                                    phx_length(body->getPosition(mass_index) - body->getPosition(neighbour_index));
                                body->addFlexionSpring(phxCreateSpring(mass_index,
                                                                       neighbour_index,
                                                                       rest_length,
                                                                       spec.flexion_spring_coeffs.ks,
                                                                       spec.flexion_spring_coeffs.kd));
                            }
                        }
                        if((col_idx + 2) < num_cols)
                        {
                            PhxIndex neighbour_index = body->getIndex(row_idx, col_idx + 2, slice_idx);
                            if(body->getIsValid(neighbour_index))
                            {
                                PhxReal rest_length =
                                    phx_length(body->getPosition(mass_index) - body->getPosition(neighbour_index));
                                body->addFlexionSpring(phxCreateSpring(mass_index,
                                                                       neighbour_index,
                                                                       rest_length,
                                                                       spec.flexion_spring_coeffs.ks,
                                                                       spec.flexion_spring_coeffs.kd));
                            }
                        }
                        if((slice_idx + 2) < num_slices)
                        {
                            PhxIndex neighbour_index = body->getIndex(row_idx, col_idx, slice_idx + 2);
                            if(body->getIsValid(neighbour_index))
                            {
                                PhxReal rest_length =
                                    phx_length(body->getPosition(mass_index) - body->getPosition(neighbour_index));
                                body->addFlexionSpring(phxCreateSpring(mass_index,
                                                                       neighbour_index,
                                                                       rest_length,
                                                                       spec.flexion_spring_coeffs.ks,
                                                                       spec.flexion_spring_coeffs.kd));
                            }
                        }
                    }

                    // shear springs
                    {
                        // diagonal elements in the same slice
                        if(row_idx > 0 && col_idx > 0)
                        {
                            PhxIndex neighbour_index = body->getIndex(row_idx - 1, col_idx - 1, slice_idx);
                            if(body->getIsValid(neighbour_index))
                            {
                                PhxReal rest_length =
                                    phx_length(body->getPosition(mass_index) - body->getPosition(neighbour_index));
                                body->addShearSpring(phxCreateSpring(mass_index,
                                                                     neighbour_index,
                                                                     rest_length,
                                                                     spec.shear_spring_coeffs.ks,
                                                                     spec.shear_spring_coeffs.kd));
                            }
                        }
                        if(row_idx < (num_rows - 1) && col_idx < (num_cols - 1))
                        {
                            PhxIndex neighbour_index = body->getIndex(row_idx + 1, col_idx + 1, slice_idx);
                            if(body->getIsValid(neighbour_index))
                            {
                                PhxReal rest_length =
                                    phx_length(body->getPosition(mass_index) - body->getPosition(neighbour_index));
                                body->addShearSpring(phxCreateSpring(mass_index,
                                                                     neighbour_index,
                                                                     rest_length,
                                                                     spec.shear_spring_coeffs.ks,
                                                                     spec.shear_spring_coeffs.kd));
                            }
                        }
                        if(row_idx > 0 && col_idx < (num_cols - 1))
                        {
                            PhxIndex neighbour_index = body->getIndex(row_idx - 1, col_idx + 1, slice_idx);
                            if(body->getIsValid(neighbour_index))
                            {
                                PhxReal rest_length =
                                    phx_length(body->getPosition(mass_index) - body->getPosition(neighbour_index));
                                body->addShearSpring(phxCreateSpring(mass_index,
                                                                     neighbour_index,
                                                                     rest_length,
                                                                     spec.shear_spring_coeffs.ks,
                                                                     spec.shear_spring_coeffs.kd));
                            }
                        }
                        if(row_idx < (num_rows - 1) && col_idx > 0)
                        {
                            PhxIndex neighbour_index = body->getIndex(row_idx + 1, col_idx - 1, slice_idx);
                            if(body->getIsValid(neighbour_index))
                            {
                                PhxReal rest_length =
                                    phx_length(body->getPosition(mass_index) - body->getPosition(neighbour_index));
                                body->addShearSpring(phxCreateSpring(mass_index,
                                                                     neighbour_index,
                                                                     rest_length,
                                                                     spec.shear_spring_coeffs.ks,
                                                                     spec.shear_spring_coeffs.kd));
                            }
                        }

                        // diagonal elements in one slide before/after
                        if(row_idx > 0 && col_idx > 0 && slice_idx > 0)
                        {
                            PhxIndex neighbour_index = body->getIndex(row_idx - 1, col_idx - 1, slice_idx - 1);
                            if(body->getIsValid(neighbour_index))
                            {
                                PhxReal rest_length =
                                    phx_length(body->getPosition(mass_index) - body->getPosition(neighbour_index));
                                body->addShearSpring(phxCreateSpring(mass_index,
                                                                     neighbour_index,
                                                                     rest_length,
                                                                     spec.shear_spring_coeffs.ks,
                                                                     spec.shear_spring_coeffs.kd));
                            }
                        }
                        if(row_idx < (num_rows - 1) && col_idx < (num_cols - 1) && slice_idx > 0)
                        {
                            PhxIndex neighbour_index = body->getIndex(row_idx + 1, col_idx + 1, slice_idx - 1);
                            if(body->getIsValid(neighbour_index))
                            {
                                PhxReal rest_length =
                                    phx_length(body->getPosition(mass_index) - body->getPosition(neighbour_index));
                                body->addShearSpring(phxCreateSpring(mass_index,
                                                                     neighbour_index,
                                                                     rest_length,
                                                                     spec.shear_spring_coeffs.ks,
                                                                     spec.shear_spring_coeffs.kd));
                            }
                        }
                        if(row_idx > 0 && col_idx < (num_cols - 1) && slice_idx > 0)
                        {
                            PhxIndex neighbour_index = body->getIndex(row_idx - 1, col_idx + 1, slice_idx - 1);
                            if(body->getIsValid(neighbour_index))
                            {
                                PhxReal rest_length =
                                    phx_length(body->getPosition(mass_index) - body->getPosition(neighbour_index));
                                body->addShearSpring(phxCreateSpring(mass_index,
                                                                     neighbour_index,
                                                                     rest_length,
                                                                     spec.shear_spring_coeffs.ks,
                                                                     spec.shear_spring_coeffs.kd));
                            }
                        }
                        if(row_idx < (num_rows - 1) && col_idx > 0 && slice_idx > 0)
                        {
                            PhxIndex neighbour_index = body->getIndex(row_idx + 1, col_idx - 1, slice_idx - 1);
                            if(body->getIsValid(neighbour_index))
                            {
                                PhxReal rest_length =
                                    phx_length(body->getPosition(mass_index) - body->getPosition(neighbour_index));
                                body->addShearSpring(phxCreateSpring(mass_index,
                                                                     neighbour_index,
                                                                     rest_length,
                                                                     spec.shear_spring_coeffs.ks,
                                                                     spec.shear_spring_coeffs.kd));
                            }
                        }

                        // diagonal elements in one slide before/after
                        if(row_idx > 0 && col_idx > 0 && slice_idx < (num_slices - 1))
                        {
                            PhxIndex neighbour_index = body->getIndex(row_idx - 1, col_idx - 1, slice_idx + 1);
                            if(body->getIsValid(neighbour_index))
                            {
                                PhxReal rest_length =
                                    phx_length(body->getPosition(mass_index) - body->getPosition(neighbour_index));
                                body->addShearSpring(phxCreateSpring(mass_index,
                                                                     neighbour_index,
                                                                     rest_length,
                                                                     spec.shear_spring_coeffs.ks,
                                                                     spec.shear_spring_coeffs.kd));
                            }
                        }
                        if(row_idx < (num_rows - 1) && col_idx < (num_cols - 1) && slice_idx < (num_slices - 1))
                        {
                            PhxIndex neighbour_index = body->getIndex(row_idx + 1, col_idx + 1, slice_idx + 1);
                            if(body->getIsValid(neighbour_index))
                            {
                                PhxReal rest_length =
                                    phx_length(body->getPosition(mass_index) - body->getPosition(neighbour_index));
                                body->addShearSpring(phxCreateSpring(mass_index,
                                                                     neighbour_index,
                                                                     rest_length,
                                                                     spec.shear_spring_coeffs.ks,
                                                                     spec.shear_spring_coeffs.kd));
                            }
                        }
                        if(row_idx > 0 && col_idx < (num_cols - 1) && slice_idx < (num_slices - 1))
                        {
                            PhxIndex neighbour_index = body->getIndex(row_idx - 1, col_idx + 1, slice_idx + 1);
                            if(body->getIsValid(neighbour_index))
                            {
                                PhxReal rest_length =
                                    phx_length(body->getPosition(mass_index) - body->getPosition(neighbour_index));
                                body->addShearSpring(phxCreateSpring(mass_index,
                                                                     neighbour_index,
                                                                     rest_length,
                                                                     spec.shear_spring_coeffs.ks,
                                                                     spec.shear_spring_coeffs.kd));
                            }
                        }
                        if(row_idx < (num_rows - 1) && col_idx > 0 && slice_idx < (num_slices - 1))
                        {
                            PhxIndex neighbour_index = body->getIndex(row_idx + 1, col_idx - 1, slice_idx + 1);
                            if(body->getIsValid(neighbour_index))
                            {
                                PhxReal rest_length =
                                    phx_length(body->getPosition(mass_index) - body->getPosition(neighbour_index));
                                body->addShearSpring(phxCreateSpring(mass_index,
                                                                     neighbour_index,
                                                                     rest_length,
                                                                     spec.shear_spring_coeffs.ks,
                                                                     spec.shear_spring_coeffs.kd));
                            }
                        }
                    }
                }
            }
        }
    }

    APPLICATION_INFO("Mass Aggregate Body Result: candidate points: {}", candidate_points);
    APPLICATION_INFO("Mass Aggregate Body Result: mesh points: {}", mesh_points);

    return body;
}

} // namespace phx
