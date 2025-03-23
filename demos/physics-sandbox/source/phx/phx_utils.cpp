#include "phx_utils.hpp"
#include "phx_geometry_queries.hpp"

#include "core/logging/logging_core.h"

namespace phx
{

std::shared_ptr<PhxTriangleMesh> phxCookTriangleMesh(const PhxVec3Array&  input_vertices,
                                                     const PhxIndexArray& input_indices)
{
    // auto mesh = std::make_shared<phx::PhxTriangleMesh>(static_cast<uint32_t>(input_indices.size() / 3));
    // for(PhxSize i = 0; i < input_indices.size(); i += 3)
    //{
    //     // PhxIndex         index = input_indices[i];
    //     // phx::PhxTriangle triangle{
    //     //     .a = glm::vec3(input_vertices[index].x, input_vertices[index].y, input_vertices[index].z),
    //     //     .b = glm::vec3(input_vertices[index + 1].x, input_vertices[index + 1].y, input_vertices[index + 1].z),
    //     //     .c = glm::vec3(input_vertices[index + 2].x, input_vertices[index + 2].y, input_vertices[index +
    //     2].z)};

    //    PhxIndex         i0 = input_indices[i];
    //    PhxIndex         i1 = input_indices[i + 1];
    //    PhxIndex         i2 = input_indices[i + 2];
    //    phx::PhxTriangle triangle{.a = glm::vec3(input_vertices[i0].x, input_vertices[i0].y, input_vertices[i0].z),
    //                              .b = glm::vec3(input_vertices[i1].x, input_vertices[i1].y, input_vertices[i1].z),
    //                              .c = glm::vec3(input_vertices[i2].x, input_vertices[i2].y, input_vertices[i2].z)};
    //    triangle.calculateCentroid();
    //    mesh->addTriangle(triangle);
    //}
    // mesh->buildAccelerationStructure();
    // return mesh;

    auto mesh = std::make_shared<phx::PhxTriangleMesh>(input_vertices, input_indices);
    return mesh;
}

static mad::PhxSpring phxCreateSpring(const PhxIndex& mass_a,
                                      const PhxIndex& mass_b,
                                      const PhxReal&  rest_length,
                                      const PhxReal&  stiffness,
                                      const PhxReal&  damping)
{
    mad::PhxSpring spring{.mass_a_idx  = mass_a,
                          .mass_b_idx  = mass_b,
                          .rest_length = rest_length,
                          .ks          = stiffness,
                          .kd          = damping};
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

    // Add surface particles
    APPLICATION_INFO("Surface particle index: {}", body->getSurfacePaticleIndex());
    body->updateSurfaceParticleIndex();
    APPLICATION_INFO("Surface particle index post update: {}", body->getSurfacePaticleIndex());

    PhxVec3Array vertices;
    input_mesh->fetchVertices(vertices);

    for(PhxSize i = 0; i < vertices.size(); i++)
    {
        PhxIndex new_idx = body->addSurfaceParticle(vertices[i]);
        // APPLICATION_INFO("New particle index: {}", new_idx);
        body->setAcceleration(new_idx, spec.acceleration);
        body->setMass(new_idx, spec.mass);
        body->setDamping(new_idx, spec.damping);
        body->setIsValid(new_idx, true);
        body->setIsFixed(new_idx, false);
    }

    const auto& surface_point_index = body->getSurfacePaticleIndex();
    const auto& mesh_indices        = input_mesh->getIndices();

    // const auto& edges               = phxExtractEdges(mesh_indices);
    // for(const auto& edge : edges)
    //{
    //     PhxIndex index1      = surface_point_index + mesh_indices[edge.first];
    //     PhxIndex index2      = surface_point_index + mesh_indices[edge.second];
    //     PhxReal  rest_length = phx_length(body->getPosition(index1) - body->getPosition(index2));
    //     body->addSurfaceSpring(phxCreateSpring(index1,
    //                                            index2,
    //                                            rest_length,
    //                                            spec.structural_spring_coeffs.ks,
    //                                            spec.structural_spring_coeffs.kd));
    // }

    const auto&                                                 triangles     = input_mesh->getTriangles();
    std::unordered_set<std::pair<PhxIndex, PhxIndex>, EdgeHash> edges         = {};
    auto                                                        spring_coeffs = spec.surface_spring_coeffs;
    for(size_t i = 0; i < mesh_indices.size(); i += 3)
    {
        PhxIndex i0 = surface_point_index + mesh_indices[i];
        PhxIndex i1 = surface_point_index + mesh_indices[i + 1];
        PhxIndex i2 = surface_point_index + mesh_indices[i + 2];

        Edge edge1 = {std::min(i0, i1), std::max(i0, i1)};
        if(!edges.contains(edge1))
        {
            PhxReal rest_length = phx_length(body->getPosition(i0) - body->getPosition(i1));
            body->addSurfaceSpring(phxCreateSpring(i0, i1, rest_length, spring_coeffs.ks, spring_coeffs.kd));
            edges.insert(edge1);
        }

        Edge edge2 = {std::min(i1, i2), std::max(i1, i2)};
        if(!edges.contains(edge2))
        {
            PhxReal rest_length = phx_length(body->getPosition(i1) - body->getPosition(i2));
            body->addSurfaceSpring(phxCreateSpring(i1, i2, rest_length, spring_coeffs.ks, spring_coeffs.kd));
            edges.insert(edge2);
        }

        Edge edge3 = {std::min(i2, i0), std::max(i2, i0)};
        if(!edges.contains(edge3))
        {
            PhxReal rest_length = phx_length(body->getPosition(i2) - body->getPosition(i0));
            body->addSurfaceSpring(phxCreateSpring(i2, i0, rest_length, spring_coeffs.ks, spring_coeffs.kd));
            edges.insert(edge3);
        }
    }

    spring_coeffs              = spec.internal_spring_coeffs;
    const PhxSize total_points = body->getParticleCount();
    for(PhxIndex surface_idx = surface_point_index; surface_idx < total_points; ++surface_idx)
    {
        if(body->getIsValid(surface_idx))
        {
            PhxVec3                 current_position = body->getPosition(surface_idx);
            PhxArray<PointDistance> distances;

            // search for closest internal points
            for(PhxIndex neighbor_index = 0; neighbor_index < surface_point_index; neighbor_index++)
            {
                if(/*surface_idx == neighbor_index || */ !body->getIsValid(neighbor_index))
                {
                    // Skip the current point and the points that are not valid
                    continue;
                }

                PhxVec3 neighbor_position = body->getPosition(neighbor_index);

                distances.push_back({neighbor_index, phx_magnitude_sq(current_position - neighbor_position)});
            }

            if(!distances.empty())
            {
                std::sort(distances.begin(),
                          distances.end(),
                          [](const PointDistance& a, const PointDistance& b) { return a.distance < b.distance; });

                const PhxIndex neighbor_count = static_cast<PhxIndex>(distances.size());
                const PhxIndex max_neighbors =
                    neighbor_count < spec.nearest_neighbors ? neighbor_count : spec.nearest_neighbors;

                for(PhxIndex k = 0; k < max_neighbors; ++k)
                {
                    PhxIndex neighbor_idx = distances[k].point_index;

                    body->addInternalSpring(phxCreateSpring(surface_idx,
                                                            neighbor_idx,
                                                            sqrtf(distances[k].distance),
                                                            spring_coeffs.ks,
                                                            spring_coeffs.kd));
                }
            }
        }
    }

    // APPLICATION_INFO("Mass Aggregate Body Result: candidate points: {}", candidate_points);
    // APPLICATION_INFO("Mass Aggregate Body Result: mesh points: {}", mesh_points);

    const PhxIndex num_particles  = static_cast<PhxIndex>(body->getParticleCount());
    const PhxReal  particles_mass = spec.mass / num_particles;
    APPLICATION_INFO("Particles: {}, Mass: {}", num_particles, particles_mass);

    for(PhxIndex i = 0; i < num_particles; ++i)
    {
        body->setMass(i, particles_mass);
    }

    return body;
}

struct Point
{
    float x;
    float y;
    float z;
};

std::shared_ptr<mad::MassAggregateVolume>
phxCookMassAggregateVolumeNearestNeighbor(const std::shared_ptr<PhxTriangleMesh>& input_mesh,
                                          const mad::MassAggregateBodySpec&       spec,
                                          const PhxBool&                          randomize_sampling_direction,
                                          const PhxVec3&                          sampling_direction)
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

    PhxIndex num_springs = 0;
    for(PhxIndex current_idx = 0; current_idx < candidate_points; ++current_idx)
    {
        if(body->getIsValid(current_idx))
        {
            PhxVec3                 current_position = body->getPosition(current_idx);
            PhxArray<PointDistance> distances;

            for(PhxIndex neighbor_index = 0; neighbor_index < candidate_points; neighbor_index++)
            {
                if(current_idx == neighbor_index || !body->getIsValid(neighbor_index))
                {
                    // Skip the current point and the points that are not valid
                    continue;
                }

                PhxVec3 neighbor_position = body->getPosition(neighbor_index);

                distances.push_back({neighbor_index, phx_magnitude_sq(current_position - neighbor_position)});
            }

            if(!distances.empty())
            {
                std::sort(distances.begin(),
                          distances.end(),
                          [](const PointDistance& a, const PointDistance& b) { return a.distance < b.distance; });

                const PhxIndex neighbor_count = static_cast<PhxIndex>(distances.size());
                const PhxIndex max_neighbors =
                    neighbor_count < spec.nearest_neighbors ? neighbor_count : spec.nearest_neighbors;

                for(PhxIndex k = 0; k < max_neighbors; ++k)
                {
                    PhxIndex neighbor_idx = distances[k].point_index;

                    phx::PhxRay ray = {};
                    ray.origin      = current_position;
                    ray.direction   = phx_normalize(body->getPosition(neighbor_idx) - current_position);
                    PhxArray<phx::PhxRaycastResult> results;
                    if(phxRaycast(ray, *input_mesh.get(), results, phx::PhxQueryMode::ClosestHit); !results.empty())
                    {
                        if(distances[k].distance <= phx_distance(current_position, results[0].point))
                        {
                            PhxReal rest_length =
                                phx_length(body->getPosition(current_idx) - body->getPosition(neighbor_idx));
                            body->addStructuralSpring(phxCreateSpring(current_idx,
                                                                      neighbor_idx,
                                                                      rest_length,
                                                                      spec.structural_spring_coeffs.ks,
                                                                      spec.structural_spring_coeffs.kd));
                            ++num_springs;
                        }
                    }
                }
            }
        }
    }
    // APPLICATION_INFO("Mass Aggregate Body Result: Num springs: {}", num_springs);
    return body;
}

std::size_t EdgeHash::operator()(const Edge& edge) const
{
    return std::hash<PhxIndex>{}(edge.first) ^ std::hash<PhxIndex>{}(edge.second);
}

std::unordered_set<std::pair<PhxIndex, PhxIndex>, EdgeHash> phxExtractEdges(const PhxIndexArray& indices)
{
    std::unordered_set<std::pair<PhxIndex, PhxIndex>, EdgeHash> edges;

    for(PhxSize i = 0; i < indices.size(); ++i)
    {
        PhxIndex i0 = indices[i];
        PhxIndex i1 = indices[i + 1];
        PhxIndex i2 = indices[i + 2];

        // add edges in a consistent order (smallest vertex first)
        edges.insert({std::min(i0, i1), std::max(i0, i1)});
        edges.insert({std::min(i1, i2), std::max(i1, i2)});
        edges.insert({std::min(i2, i0), std::max(i2, i0)});
    }

    return edges;
}

} // namespace phx
