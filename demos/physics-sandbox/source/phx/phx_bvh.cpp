//#include "bvh.hpp"
//
//#include <ranges>
//#include <execution>
//#include <algorithm>

namespace phx
{

//Bvh::Bvh(const uint32_t& num_primitives) noexcept
//{
//    // A binary tree has 2n - 1 nodes where n is the number of leaf nodes.
//    m_nodes.resize(static_cast<size_t>(num_primitives) * 2 - 1);
//    m_primitive_indices.resize(num_primitives);
//
//    // Initialize the primitive indices.
//    for(uint32_t i = 0; i < num_primitives; ++i)
//    {
//        m_primitive_indices[i] = i;
//    }
//}
//
//void Bvh::build(const std::vector<Triangle>& triangles) noexcept
//{
//    auto& root_node          = m_nodes[m_nodes_used];
//    root_node.idx            = 0;
//    root_node.num_primitives = static_cast<uint32_t>(m_primitive_indices.size());
//
//    // Recursively build the BVH.
//    updateBounds(m_nodes_used, triangles);
//    subdivide(m_nodes_used, triangles);
//}
//
//void Bvh::updateBounds(const uint32_t& node_idx, const std::vector<Triangle>& triangles)
//{
//    auto& node    = m_nodes[node_idx];
//    node.aabb.min = glm::vec3(kFloatMax);
//    node.aabb.max = glm::vec3(-kFloatMax);
//
//    for(uint32_t first = node.idx, i = 0; i < node.num_primitives; ++i)
//    {
//        const auto& triangle = triangles[m_primitive_indices[first + i]];
//        node.aabb.min        = min(node.aabb.min, min(triangle.v0, min(triangle.v1, triangle.v2)));
//        node.aabb.max        = max(node.aabb.max, max(triangle.v0, max(triangle.v1, triangle.v2)));
//    }
//}
//
//void Bvh::subdivide(const uint32_t& node_idx, const std::vector<Triangle>& triangles) noexcept
//{
//    auto& node = m_nodes[node_idx];
//    if(node.num_primitives <= 2)
//    {
//        return;
//    }
//
//    // Find the longest axis of the AABB.
//    glm::vec3 extent = node.aabb.max - node.aabb.min;
//    uint32_t  axis   = 0;
//    if(extent.y > extent.x)
//    {
//        axis = 1;
//    }
//    if(extent.z > extent.y && extent.z > extent.x)
//    {
//        axis = 2;
//    }
//
//    // Find the midpoint of the longest axis.
//    float split_point = 0.5f * (node.aabb.min[axis] + node.aabb.max[axis]);
//
//    // partition the primitives based on the longest axis.
//    uint32_t i = node.idx;
//    uint32_t j = i + node.num_primitives - 1;
//    while(i <= j)
//    {
//        const auto& triangle = triangles[m_primitive_indices[i]];
//        if(triangle.centroid[axis] < split_point)
//        {
//            i++;
//        }
//        else
//        {
//            std::swap(m_primitive_indices[i], m_primitive_indices[j]);
//            --j;
//        }
//    }
//
//    uint32_t remaining_primitives_count = i - node.idx;
//
//    // Abort the subdivision if the partitioning did not split the primitives (nothing left).
//    if(remaining_primitives_count == 0 || remaining_primitives_count == node.num_primitives)
//    {
//        return;
//    }
//
//    // Create the left and right child nodes.
//    uint32_t left_child_idx  = m_nodes_used;
//    uint32_t right_child_idx = m_nodes_used + 1;
//    m_nodes_used += 2;
//
//    auto& left_node  = m_nodes[left_child_idx];
//    auto& right_node = m_nodes[right_child_idx];
//
//    left_node.idx             = node.idx;
//    left_node.num_primitives  = remaining_primitives_count;
//    right_node.idx            = i;
//    right_node.num_primitives = node.num_primitives - left_node.num_primitives;
//
//    // Update the parent node.
//    node.idx            = left_child_idx;
//    node.num_primitives = 0; // Not a leaf node.
//
//    // Update the bounds of the left and right child nodes.
//    updateBounds(left_child_idx, triangles);
//    updateBounds(right_child_idx, triangles);
//
//    // Recursively subdivide the left and right child nodes.
//    subdivide(left_child_idx, triangles);
//    subdivide(right_child_idx, triangles);
//}

}; // namespace phx