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

void DeformableBody::updateInternalForces(const double& total_time, const double& step_size) noexcept
{
    m_body->updateInternalForces(static_cast<PhxReal>(total_time), static_cast<PhxReal>(step_size));
}

// void DeformableBody::buildAccelerationStructure()
//{
//     m_mesh->buildAccelerationStructure();
// }

} // namespace phx::mad