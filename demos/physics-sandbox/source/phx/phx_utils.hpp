#ifndef PHX_UTILS_HPP
#define PHX_UTILS_HPP

#include "phx_geometry.hpp"
#include "phx_core.hpp"
#include "mad/deformable_body.hpp"

#include <unordered_set>

namespace phx
{

struct PointDistance
{
    PhxIndex point_index;
    PhxReal  distance;
};

using Edge = std::pair<PhxIndex, PhxIndex>;

struct EdgeHash
{
    std::size_t operator()(const Edge& edge) const;
};

std::unordered_set<std::pair<PhxIndex, PhxIndex>, EdgeHash> phxExtractEdges(const PhxIndexArray& indices);

std::shared_ptr<PhxTriangleMesh> phxCookTriangleMesh(const PhxVec3Array&  input_vertices,
                                                     const PhxIndexArray& input_indices);

std::shared_ptr<mad::MassAggregateVolume>
phxCookMassAggregateVolume(const std::shared_ptr<PhxTriangleMesh>& input_mesh,
                           const mad::MassAggregateBodySpec&       spec,
                           const PhxBool&                          randomize_sampling_direction = false,
                           const PhxVec3&                          sampling_direction           = {1.0f, 0.0f, 0.0f});

std::shared_ptr<mad::MassAggregateVolume>
phxCookMassAggregateVolumeNearestNeighbor(const std::shared_ptr<PhxTriangleMesh>& input_mesh,
                                          const mad::MassAggregateBodySpec&       spec,
                                          const PhxBool&                          randomize_sampling_direction = false,
                                          const PhxVec3& sampling_direction = {1.0f, 0.0f, 0.0f});

} // namespace phx

#endif // !PHX_UTILS_HPP
