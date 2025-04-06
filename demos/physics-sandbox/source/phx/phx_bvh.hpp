#pragma once

//#include "geometry.hpp"

namespace phx
{

//struct BvhNode
//{
//    AABB     aabb;
//    uint32_t num_primitives; // number of primitives in this node. Zero if it is not a leaf node.
//
//    // For a leaf node, idx is the index to the fist primitive in the primitive array
//    // For a non-leaf node, idx represents the index of the left child BVH node in the BVH hierarchy (array).
//    uint32_t idx;
//
//    inline bool isLeaf() const { return num_primitives > 0; }
//};
//
//class Bvh
//{
//public:
//    Bvh(const uint32_t& num_primitives) noexcept;
//
//    void build(const std::vector<Triangle>& triangles) noexcept;
//
//private:
//    void updateBounds(const uint32_t& node_idx, const std::vector<Triangle>& triangles);
//    void subdivide(const uint32_t& node_idx, const std::vector<Triangle>& triangles) noexcept;
//
//private:
//    std::vector<BvhNode>  m_nodes;
//    std::vector<uint32_t> m_primitive_indices; // Indices to the primitives in the triangle mesh.
//    uint32_t              m_nodes_used = 1;
//};
//
//class BvhInstance
//{
//};

} // namespace phx