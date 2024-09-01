#pragma once

#include "math_utils.hpp"

#include <vector>
#include <memory>

// namespace phx
//{
// class Bvh;
// }

namespace phx
{

using Point = glm::vec3;

struct AABB
{
    glm::vec3 min;
    glm::vec3 max;
};

struct Triangle
{
    glm::vec3 v0;
    glm::vec3 v1;
    glm::vec3 v2;
    glm::vec3 centroid;

    inline void calculateCentroid() { centroid = (v0 + v1 + v2) * 0.333333f; }
};

struct Plane
{
    glm::vec3 normal;
    float     distance;

    inline Plane() : normal(1.0f, 0.0f, 0.0f), distance(0.0f) {}
    inline Plane(const glm::vec3& n, float d) : normal(n), distance(d) {}
};

struct Ray
{
    glm::vec3 origin;
    glm::vec3 direction;
    float     t = kFloatMax; // Max because we want to find the closest intersection.
};

struct RayCastResult
{
    bool      hit = false;
    float     t   = kFloatMax;
    glm::vec3 point{0.0f};
    glm::vec3 normal{0.0f, 0.0f, 1.0f};

    inline void reset()
    {
        hit    = false;
        t      = kFloatMax;
        point  = glm::vec3(0.0f);
        normal = glm::vec3(0.0f, 0.0f, 1.0f);
    }
};

struct BvhNode
{
    AABB     aabb;
    uint32_t num_primitives; // number of primitives in this node. Zero if it is not a leaf node.

    // For a leaf node, idx is the index to the fist primitive in the primitive array
    // For a non-leaf node, idx represents the index of the left child BVH node in the BVH hierarchy (array).
    uint32_t idx;

    inline bool isLeaf() const { return num_primitives > 0; }
};

class Bvh
{
public:
    Bvh(const uint32_t& num_primitives) noexcept;

    void build(const std::vector<Triangle>& triangles) noexcept;

    const std::vector<BvhNode>& getNodes() const noexcept;

    const std::vector<uint32_t>& getPrimitiveIndices() const noexcept;

private:
    void updateBounds(const uint32_t& node_idx, const std::vector<Triangle>& triangles);
    void subdivide(const uint32_t& node_idx, const std::vector<Triangle>& triangles) noexcept;

private:
    std::vector<BvhNode>  m_nodes;
    std::vector<uint32_t> m_primitive_indices; // Indices to the primitives in the triangle mesh.
    uint32_t              m_nodes_used    = 1;
    uint32_t              m_root_node_idx = 0;
};

class BvhInstance
{
};

class TriangleMesh
{
public:
    TriangleMesh(const uint32_t& num_primitives);

    void addTriangle(const Triangle& triangle);

    void buildAccelerationStructure();

    const std::shared_ptr<Bvh>& getBvh() const noexcept;

    const std::vector<Triangle>& getTriangles() const noexcept;

private:
    std::vector<Triangle> m_triangles;
    // std::vector<uint32_t> m_indices; // Indices to the triangles in the triangle mesh.
    std::shared_ptr<Bvh> m_bvh;
};

} // namespace phx

// Physics world
//      Dynamic Body (static / kinematic)
//  	    Rigid Body
//          Deformable Body