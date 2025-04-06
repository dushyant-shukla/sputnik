#ifndef PHX_DEFORMABLE_BODY_HPP
#define PHX_DEFORMABLE_BODY_HPP

#include "../phx_types.hpp"
#include "../phx_geometry.hpp"
#include "../phx_geometry_queries.hpp"
#include "mass_aggregate_body.hpp"

#include <memory>

namespace phx::mad
{

class DeformableBody
{
public:
    DeformableBody() = default;
    DeformableBody(const PhxUint& triangle_count, const MassAggregateBodySpec& spec);
    DeformableBody(const std::shared_ptr<PhxTriangleMesh>& mesh, const std::shared_ptr<MassAggregateBody>& body);
    virtual ~DeformableBody() = default;

    std::shared_ptr<PhxTriangleMesh>   getGeometry() const noexcept;
    std::shared_ptr<MassAggregateBody> getBody() const noexcept;

    void addTriangle(const PhxTriangle& triangle) noexcept;

    void setup() noexcept;

    // void buildAccelerationStructure();

    void update(const double& total_time, const double& step_size) noexcept;
    void updateInternalForces(const double& total_time, const double& step_size) noexcept;

protected:
    void satisfyConstraints(const PhxUint& iteration_count) noexcept;

private:
    std::shared_ptr<PhxTriangleMesh>   m_mesh;
    std::shared_ptr<MassAggregateBody> m_body;
};

} // namespace phx::mad

#endif // !PHX_DEFORMABLE_BODY_HPP
