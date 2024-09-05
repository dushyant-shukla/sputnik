#include "geometry.hpp"
#include "bvh.hpp"

#include <core/logging/logging_core.h>

namespace phx
{

PhxTriangleMesh::PhxTriangleMesh(const uint32_t& num_primitives)
{
    // m_triangles.resize(num_primitives);
    //  m_indices.resize(num_primitives);
    m_bvh = std::make_shared<PhxBvh>(num_primitives);
}

PhxTriangleMesh::PhxTriangleMesh(const PhxVec3Array& input_vertices, const PhxIndexArray& input_indices)
    : m_indices(input_indices)
    , m_num_vertices(input_vertices.size())
{
    // m_triangles.resize(input_indices.size() / 3);
    // m_indices.resize(input_indices.size() / 3);

    for(size_t i = 0; i < input_indices.size(); i += 3)
    {
        PhxTriangle triangle;
        triangle.a = input_vertices[input_indices[i]];
        triangle.b = input_vertices[input_indices[i + 1]];
        triangle.c = input_vertices[input_indices[i + 2]];
        triangle.calculateCentroid();
        m_triangles.emplace_back(triangle);
    }

    // for(PhxSize i = 0; i < input_indices.size(); i += 3)
    //{
    //  PhxIndex         index = input_indices[i];
    //  phx::PhxTriangle triangle{
    //      .a = glm::vec3(input_vertices[index].x, input_vertices[index].y, input_vertices[index].z),
    //      .b = glm::vec3(input_vertices[index + 1].x, input_vertices[index + 1].y, input_vertices[index + 1].z),
    //      .c = glm::vec3(input_vertices[index + 2].x, input_vertices[index + 2].y, input_vertices[index + 2].z)};

    // PhxIndex         i0 = input_indices[i];
    // PhxIndex         i1 = input_indices[i + 1];
    // PhxIndex         i2 = input_indices[i + 2];
    // phx::PhxTriangle triangle{.a = glm::vec3(input_vertices[i0].x, input_vertices[i0].y, input_vertices[i0].z),
    //                           .b = glm::vec3(input_vertices[i1].x, input_vertices[i1].y, input_vertices[i1].z),
    //                           .c = glm::vec3(input_vertices[i2].x, input_vertices[i2].y, input_vertices[i2].z)};
    // triangle.calculateCentroid();
    // mesh->addTriangle(triangle);
    //}

    m_bvh = std::make_shared<PhxBvh>(static_cast<uint32_t>(input_indices.size() / 3));
    buildAccelerationStructure();
}

void PhxTriangleMesh::addTriangle(const PhxTriangle& triangle)
{
    m_triangles.emplace_back(triangle);
}

void PhxTriangleMesh::buildAccelerationStructure()
{
    m_bvh->build(m_triangles);
}

const std::shared_ptr<PhxBvh>& PhxTriangleMesh::getBvh() const noexcept
{
    return m_bvh;
}

const std::vector<PhxTriangle>& PhxTriangleMesh::getTriangles() const noexcept
{
    return m_triangles;
}

std::vector<PhxTriangle>& PhxTriangleMesh::getTriangles() noexcept
{
    return m_triangles;
}

void PhxTriangleMesh::fetchVertices(PhxVec3Array& vertices) const noexcept
{
    std::list<PhxIndex> indices_processed;

    auto is_index_not_processed = [&indices_processed](const PhxIndex& index) -> bool
    { return std::find(indices_processed.begin(), indices_processed.end(), index) == indices_processed.end(); };

    vertices.clear();
    // vertices.resize(m_indices.size() * 3);

    // for(const auto& triangle : m_triangles)
    //{
    //     vertices.emplace_back(triangle.a);
    //     vertices.emplace_back(triangle.b);
    //     vertices.emplace_back(triangle.c);
    // }

    // PhxSize triangle_idx = 0;
    // for(PhxSize i = 0; i < m_indices.size(); i += 3)
    //{
    //     PhxSize     index    = m_indices[i];
    //     const auto& triangle = m_triangles[triangle_idx];
    //     PhxPoint    p0       = triangle.a;
    //     PhxPoint    p1       = triangle.b;
    //     PhxPoint    p2       = triangle.c;

    //    if(is_index_not_processed(index))
    //    {
    //        vertices.emplace_back(p0);
    //        indices_processed.emplace_back(index);
    //    }

    //    if(is_index_not_processed(index + 1))
    //    {
    //        vertices.emplace_back(p1);
    //        indices_processed.emplace_back(index + 1);
    //    }

    //    if(is_index_not_processed(index + 2))
    //    {
    //        vertices.emplace_back(p2);
    //        indices_processed.emplace_back(index + 2);
    //    }

    //    // vertices.emplace_back(p0);
    //    // vertices.emplace_back(p1);
    //    // vertices.emplace_back(p2);
    //    //  vertices[index]     = p0;
    //    //  vertices[index + 1] = p1;
    //    //  vertices[index + 2] = p2;
    //    ++triangle_idx;
    //}

    vertices.resize(m_num_vertices);

    for(PhxSize i = 0; i < m_triangles.size(); ++i)
    {
        const auto& triangle           = m_triangles[i];
        vertices[m_indices[i * 3]]     = triangle.a;
        vertices[m_indices[i * 3 + 1]] = triangle.b;
        vertices[m_indices[i * 3 + 2]] = triangle.c;
    }
}

const PhxIndexArray& PhxTriangleMesh::getIndices() const noexcept
{
    return m_indices;
}

void PhxTriangleMesh::updateTriangles(const PhxVec3Array& vertices)
{
    PhxIndex triangle_index = 0;
    for(size_t i = 0; i < m_indices.size(); i += 3)
    {
        // PhxTriangle triangle;
        // triangle.a = input_vertices[input_indices[i]];
        // triangle.b = input_vertices[input_indices[i + 1]];
        // triangle.c = input_vertices[input_indices[i + 2]];
        // triangle.calculateCentroid();
        // m_triangles.emplace_back(triangle);

        m_triangles[triangle_index].a = vertices[m_indices[i]];
        m_triangles[triangle_index].b = vertices[m_indices[i + 1]];
        m_triangles[triangle_index].c = vertices[m_indices[i + 2]];
        m_triangles[triangle_index].calculateCentroid();

        ++triangle_index;
    }

    // Todo:: Update the BVH.
}

PhxBvh::PhxBvh(const uint32_t& num_primitives) noexcept
{
    // A binary tree has 2n - 1 nodes where n is the number of leaf nodes.
    m_nodes.resize(static_cast<size_t>(num_primitives) * 2 - 1);
    m_primitive_indices.resize(num_primitives);

    // Initialize the primitive indices.
    for(uint32_t i = 0; i < num_primitives; ++i)
    {
        m_primitive_indices[i] = i;
    }
}

void PhxBvh::build(const PhxArray<PhxTriangle>& triangles) noexcept
{
    auto& root_node          = m_nodes[m_root_node_idx];
    root_node.idx            = 0;
    root_node.num_primitives = static_cast<uint32_t>(m_primitive_indices.size());

    // Recursively build the BVH.
    updateBounds(m_root_node_idx, triangles);
    subdivide(m_root_node_idx, triangles);
}

const PhxArray<PhxBvhNode>& PhxBvh::getNodes() const noexcept
{
    return m_nodes;
}

const PhxIndexArray& PhxBvh::getPrimitiveIndices() const noexcept
{
    return m_primitive_indices;
}

void PhxBvh::updateBounds(const PhxIndex& node_idx, const PhxArray<PhxTriangle>& triangles)
{
    auto& node    = m_nodes[node_idx];
    node.aabb.min = glm::vec3(kPhxFloatMax);
    node.aabb.max = glm::vec3(-kPhxFloatMax);

    for(uint32_t first = node.idx, i = 0; i < node.num_primitives; ++i)
    {
        const auto& triangle = triangles[m_primitive_indices[first + i]];
        node.aabb.min        = phxMin(node.aabb.min, phxMin(triangle.a, phxMin(triangle.b, triangle.c)));
        node.aabb.max        = phxMax(node.aabb.max, phxMax(triangle.a, phxMax(triangle.b, triangle.c)));
    }
}

void PhxBvh::subdivide(const PhxIndex& node_idx, const PhxArray<PhxTriangle>& triangles) noexcept
{
    auto& node = m_nodes[node_idx];
    if(node.num_primitives <= 2)
    {
        // APPLICATION_INFO("Leaf node: Primitive count: {}", node.num_primitives);
        return;
    }

    // Find the longest axis of the AABB.
    glm::vec3 extent = node.aabb.max - node.aabb.min;
    uint32_t  axis   = 0;
    if(extent.y > extent.x)
    {
        axis = 1;
    }
    if(extent.z > extent.y && extent.z > extent.x)
    {
        axis = 2;
    }

    // Find the midpoint of the longest axis.
    float split_point = 0.5f * (node.aabb.min[axis] + node.aabb.max[axis]);

    // partition the primitives based on the longest axis.
    uint32_t i = node.idx;
    uint32_t j = i + node.num_primitives - 1;
    while(i <= j)
    {
        const auto& triangle = triangles[m_primitive_indices[i]];
        if(triangle.centroid[axis] < split_point)
        {
            i++;
        }
        else
        {
            std::swap(m_primitive_indices[i], m_primitive_indices[j]);
            --j;
        }
    }

    uint32_t remaining_primitives_count = i - node.idx;

    // Abort the subdivision if the partitioning did not split the primitives (nothing left).
    if(remaining_primitives_count == 0 || remaining_primitives_count == node.num_primitives)
    {
        return;
    }

    // Create the left and right child nodes.
    // uint32_t left_child_idx  = m_nodes_used;
    // uint32_t right_child_idx = m_nodes_used + 1;
    // m_nodes_used += 2;

    uint32_t left_child_idx  = m_nodes_used++;
    uint32_t right_child_idx = m_nodes_used++;

    auto& left_node  = m_nodes[left_child_idx];
    auto& right_node = m_nodes[right_child_idx];

    left_node.idx             = node.idx;
    left_node.num_primitives  = remaining_primitives_count;
    right_node.idx            = i;
    right_node.num_primitives = node.num_primitives - left_node.num_primitives;

    // Update the parent node.
    node.idx            = left_child_idx;
    node.num_primitives = 0; // Not a leaf node.

    // Update the bounds of the left and right child nodes.
    updateBounds(left_child_idx, triangles);
    updateBounds(right_child_idx, triangles);

    // Recursively subdivide the left and right child nodes.
    subdivide(left_child_idx, triangles);
    subdivide(right_child_idx, triangles);
}

PhxReal phxCalculateArea(const PhxTriangle& triangle)
{
    PhxVec3 u    = triangle.b - triangle.a;
    PhxVec3 v    = triangle.c - triangle.a;
    PhxVec3 w    = glm::cross(u, v);
    PhxReal area = 0.5f * phx_magnitude(w);
    return area;
}

PhxVec3 phxCalculateCentroid(const PhxTriangle& triangle)
{
    PhxVec3 centroid = (triangle.a + triangle.b + triangle.c) * 0.333333f;
    return centroid;
}

PhxVec3 phxCalculateNormal(const PhxTriangle& triangle)
{
    PhxVec3 u = triangle.b - triangle.a;
    PhxVec3 v = triangle.c - triangle.a;
    PhxVec3 n = glm::cross(u, v);
    return phx_normalize(n);
}

} // namespace phx