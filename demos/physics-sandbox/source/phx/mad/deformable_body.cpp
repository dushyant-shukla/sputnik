#include "deformable_body.hpp"

namespace phx::mad
{

DeformableBody::DeformableBody(const PhxUint& triangle_count, const MassAggregateBodySpec& spec)
{

    m_mesh = std::make_shared<PhxTriangleMesh>(triangle_count);
    // m_body     = std::make_shared<MassAggregateBody>(triangle_count * 3);
}

DeformableBody::DeformableBody(const std::shared_ptr<PhxTriangleMesh>&   mesh,
                               const std::shared_ptr<MassAggregateBody>& body)
    : m_mesh(mesh)
    , m_body(body)
{
}

std::shared_ptr<PhxTriangleMesh> DeformableBody::getGeometry() const noexcept
{
    return m_mesh;
}

std::shared_ptr<MassAggregateBody> DeformableBody::getBody() const noexcept
{
    return m_body;
}

void DeformableBody::addTriangle(const PhxTriangle& triangle) noexcept
{
    m_mesh->addTriangle(triangle);
}

void DeformableBody::setup() noexcept
{
    m_mesh->buildAccelerationStructure();

    const auto& bvh       = m_mesh->getBvh();
    const auto& root_aabb = bvh->getNodes()[0].aabb;
}

void DeformableBody::update(const double& total_time, const double& step_size) noexcept
{
    updateInternalForces(total_time, step_size);

    // Update the triangle mesh with new vertex positions and refit/reconstruct the BVH.

    // start iterating positions from idx of the surface particles

    PhxIndex     surface_particle_idx = m_body->getSurfacePaticleIndex();
    PhxIndex     count                = static_cast<PhxIndex>(m_body->getParticleCount());
    PhxVec3Array vertices;
    for(PhxIndex i = surface_particle_idx; i < count; ++i)
    {
        vertices.emplace_back(m_body->getPosition(i));
    }

    // update the triangle mesh with new vertex positions
    m_mesh->updateTriangles(vertices);

    PhxArray<PhxTriangle>& triangles    = m_mesh->getTriangles();
    PhxIndex               triangle_idx = 0;
    auto&                  indices      = m_mesh->getIndices();
    for(PhxSize index = 0; index < indices.size(); index += 3)
    {
        PhxTriangle& triangle      = triangles[triangle_idx];
        PhxReal      original_area = triangle.area;
        PhxReal      current_area  = phxCalculateArea(triangle);
        PhxVec3      centroid      = phxCalculateCentroid(triangle);

        PhxReal area_change = current_area - original_area;
        if(area_change > kPhxEpsilon)
        {
            // Apply the area change to the triangle.
            PhxVec3 normal = phxCalculateNormal(triangle);
            PhxVec3 force  = normal * area_change * 0.35f;

            // Apply the force to the triangle.
            PhxIndex i0 = surface_particle_idx + indices[index];
            PhxIndex i1 = surface_particle_idx + indices[index + 1];
            PhxIndex i2 = surface_particle_idx + indices[index + 2];

            m_body->addForce(i0, force);
            m_body->addForce(i1, force);
            m_body->addForce(i2, force);
        }

        ++triangle_idx;
    }
}

void DeformableBody::updateInternalForces(const double& total_time, const double& step_size) noexcept
{
    m_body->updateInternalForces(static_cast<PhxReal>(total_time), static_cast<PhxReal>(step_size));
}

// void DeformableBody::buildAccelerationStructure()
//{
//     m_mesh->buildAccelerationStructure();
// }

} // namespace phx::mad