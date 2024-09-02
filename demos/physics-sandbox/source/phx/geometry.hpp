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

using PhxPoint = PhxVec3;

struct PhxAABB
{
    PhxVec3 min;
    PhxVec3 max;
};

struct PhxTriangle
{
    PhxPoint a;
    PhxPoint b;
    PhxPoint c;
    PhxPoint centroid;

    inline void calculateCentroid() { centroid = (a + b + c) * 0.333333f; }
};

struct PhxPlane
{
    PhxVec3 normal;
    PhxReal distance;

    inline PhxPlane() : normal(1.0f, 0.0f, 0.0f), distance(0.0f) {}
    inline PhxPlane(const PhxVec3& n, const PhxReal& d) : normal(n), distance(d) {}
};

struct PhxRay
{
    PhxPoint origin;
    PhxVec3  direction;
    PhxReal  t = kPhxFloatMax; // Max because we want to find the closest intersection.
};

struct PhxRaycastResult
{
    PhxBool   hit = false;
    PhxReal   t   = kPhxFloatMax;
    PhxPoint  point{0.0f};
    glm::vec3 normal{0.0f, 0.0f, 1.0f};

    inline void reset()
    {
        hit    = false;
        t      = kPhxFloatMax;
        point  = glm::vec3(0.0f);
        normal = glm::vec3(0.0f, 0.0f, 1.0f);
    }
};

struct PhxBvhNode
{
    PhxAABB  aabb;
    uint32_t num_primitives; // number of primitives in this node. Zero if it is not a leaf node.

    // For a leaf node, idx is the index to the fist primitive in the primitive array
    // For a non-leaf node, idx represents the index of the left child BVH node in the BVH hierarchy (array).
    uint32_t idx;

    inline bool isLeaf() const { return num_primitives > 0; }
};

class PhxBvh
{
public:
    PhxBvh(const uint32_t& num_primitives) noexcept;

    void build(const PhxArray<PhxTriangle>& triangles) noexcept;

    const PhxArray<PhxBvhNode>& getNodes() const noexcept;

    const PhxIndexArray& getPrimitiveIndices() const noexcept;

private:
    void updateBounds(const PhxIndex& node_idx, const PhxArray<PhxTriangle>& triangles);
    void subdivide(const PhxIndex& node_idx, const PhxArray<PhxTriangle>& triangles) noexcept;

private:
    PhxArray<PhxBvhNode> m_nodes;
    PhxArray<uint32_t>   m_primitive_indices; // Indices to the primitives in the triangle mesh.
    uint32_t             m_nodes_used    = 1;
    uint32_t             m_root_node_idx = 0;
};

class BvhInstance
{
};

class PhxTriangleMesh
{
public:
    PhxTriangleMesh(const uint32_t& num_primitives);

    void addTriangle(const PhxTriangle& triangle);

    void buildAccelerationStructure();

    const std::shared_ptr<PhxBvh>& getBvh() const noexcept;

    const PhxArray<PhxTriangle>& getTriangles() const noexcept;

private:
    PhxArray<PhxTriangle> m_triangles;
    // PhxArray<uint32_t> m_indices; // Indices to the triangles in the triangle mesh.
    std::shared_ptr<PhxBvh> m_bvh;
};

} // namespace phx

// Physics world
//      Dynamic Body (static / kinematic)
//  	    Rigid Body
//          Deformable Body