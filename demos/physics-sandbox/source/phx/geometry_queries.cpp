#include "geometry_queries.hpp"

#include <stack>
#include <core/logging/logging_core.h>

namespace phx
{

PhxPlane fromTriangle(const PhxTriangle& t)
{
    PhxPlane result;
    result.normal   = glm::normalize(glm::cross(t.b - t.a, t.c - t.a));
    result.distance = glm::dot(result.normal, t.a);
    return result;
}

bool raycastTriangle(const PhxRay& ray, const PhxTriangle& triangle, PhxRaycastResult& out_result)
{
    glm::vec3 edge1 = triangle.b - triangle.a;
    glm::vec3 edge2 = triangle.c - triangle.a;

    glm::vec3 ray_cross_e2 = glm::cross(ray.direction, edge2);
    float     det          = glm::dot(edge1, ray_cross_e2);

    if(det > -kPhxEpsilon && det < kPhxEpsilon)
    {
        return false; // This ray is parallel to this triangle.
    }

    float     inv_det = 1.0f / det;
    glm::vec3 s       = ray.origin - triangle.a;
    float     u       = inv_det * glm::dot(s, ray_cross_e2);
    if(u < 0.0f || u > 1.0f)
    {
        return false;
    }

    glm::vec3 s_cross_e1 = glm::cross(s, edge1);
    float     v          = inv_det * glm::dot(ray.direction, s_cross_e1);
    if(v < 0.0f || u + v > 1.0f)
    {
        return false;
    }

    // Determine where the intersection point is on the line.
    float t = inv_det * glm::dot(edge2, s_cross_e1);
    if(t < kPhxEpsilon)
    {
        return false;
    }

    out_result.hit    = true;
    out_result.t      = std::min(t, out_result.t);
    out_result.point  = ray.origin + ray.direction * t;
    out_result.normal = glm::normalize(glm::cross(edge1, edge2));

    return true;
}

glm::vec3 project(const glm::vec3& length, const glm::vec3& direction)
{
    float dot    = glm::dot(length, direction);
    float mag_sq = glm::dot(direction, direction);
    return direction * (dot / mag_sq);
}

glm::vec3 barycentric(const PhxPoint& point, const PhxTriangle& triangle)
{
    // return glm::vec3();

    glm::vec3 ap = point - triangle.a;
    glm::vec3 bp = point - triangle.b;
    glm::vec3 cp = point - triangle.c;

    glm::vec3 ab = triangle.b - triangle.a;
    glm::vec3 ac = triangle.c - triangle.a;
    glm::vec3 bc = triangle.c - triangle.b;
    glm::vec3 cb = triangle.b - triangle.c;
    glm::vec3 ca = triangle.a - triangle.c;

    glm::vec3 v = ab - project(ab, cb);
    float     a = 1.0f - (glm::dot(v, ap) / glm::dot(v, ab));

    v       = bc - project(bc, ac);
    float b = 1.0f - (glm::dot(v, bp) / glm::dot(v, bc));

    v       = ca - project(ca, ab);
    float c = 1.0f - (glm::dot(v, cp) / glm::dot(v, ca));

#ifdef DO_SANITY_TESTS
#ifndef NO_EXTRAS
    vec3 barycentric = BarycentricOptimized(p, t);
    if(fabsf(a - barycentric.x) > 0.1f)
    {
        std::cout << "Expected a: " << a << " to be: " << barycentric.x << "\n";
    }
    if(fabsf(b - barycentric.y) > 0.1f)
    {
        std::cout << "Expected b: " << b << " to be: " << barycentric.y << "\n";
    }
    if(fabsf(c - barycentric.z) > 0.1f)
    {
        std::cout << "Expected c: " << c << " to be: " << barycentric.z << "\n";
    }
#endif
#endif

    return glm::vec3(a, b, c);
}

bool Raycast(const PhxPlane& plane, const PhxRay& ray, PhxRaycastResult outResult)
{
    // ResetRaycastResult(outResult);

    float nd = glm::dot(ray.direction, plane.normal);
    float pn = glm::dot(ray.origin, plane.normal);

    // nd must be negative, and not 0
    // if nd is positive, the ray and plane normals
    // point in the same direction. No intersection.
    if(nd >= 0.0f)
    {
        return false;
    }

    float t = (plane.distance - pn) / nd;

    // t must be positive
    if(t >= 0.0f)
    {
        // if(outResult != 0)
        {
            outResult.t      = t;
            outResult.hit    = true;
            outResult.point  = ray.origin + ray.direction * t;
            outResult.normal = glm::normalize(plane.normal);
        }
        return true;
    }

    return false;
}

bool raycastTriangleBarycentric(const PhxRay& ray, const PhxTriangle& triangle, PhxRaycastResult& out_result)
{
    PhxPlane plane = fromTriangle(triangle);

    PhxRaycastResult planeResult;
    if(!Raycast(plane, ray, planeResult))
    {
        return false;
    }
    float t = planeResult.t;

    PhxPoint result = ray.origin + ray.direction * t;

    glm::vec3 bc = barycentric(result, triangle);
    if(bc.x >= 0.0f && bc.x <= 1.0f && bc.y >= 0.0f && bc.y <= 1.0f && bc.z >= 0.0f && bc.z <= 1.0f)
    {

#ifdef DO_SANITY_TESTS
        if(!PointInTriangle(result, triangle))
        {
            std::cout << "Point in triangle and barycentric coordinates don't match!\n";
        }
#endif
        // if(outResult != 0)
        {
            out_result.t      = t;
            out_result.hit    = true;
            out_result.point  = ray.origin + ray.direction * t;
            out_result.normal = plane.normal;
        }

        return true;
    }
#ifdef DO_SANITY_TESTS
    else if(PointInTriangle(result, triangle))
    {
        std::cout << "Point in triangle and barycentric coordinates don't match!\n";
    }
#endif

    return false;
}

bool raycastAABB(const PhxRay& ray, const PhxAABB& aabb, std::vector<PhxRaycastResult>& out_results)
{
    glm::vec3 min = aabb.min;
    glm::vec3 max = aabb.max;

    float tx_min = (min.x - ray.origin.x) / (CMP_FLOAT_EQ(ray.direction.x, 0.0f) ? kPhxEpsilon : ray.direction.x);
    float tx_max = (max.x - ray.origin.x) / (CMP_FLOAT_EQ(ray.direction.x, 0.0f) ? kPhxEpsilon : ray.direction.x);

    float ty_min = (min.y - ray.origin.y) / (CMP_FLOAT_EQ(ray.direction.y, 0.0f) ? kPhxEpsilon : ray.direction.y);
    float ty_max = (max.y - ray.origin.y) / (CMP_FLOAT_EQ(ray.direction.y, 0.0f) ? kPhxEpsilon : ray.direction.y);

    float tz_min = (min.z - ray.origin.z) / (CMP_FLOAT_EQ(ray.direction.z, 0.0f) ? kPhxEpsilon : ray.direction.z);
    float tz_max = (max.z - ray.origin.z) / (CMP_FLOAT_EQ(ray.direction.z, 0.0f) ? kPhxEpsilon : ray.direction.z);

    float t_min = std::max(std::max(std::min(tx_min, tx_max), std::min(ty_min, ty_max)), std::min(tz_min, tz_max));
    float t_max = std::min(std::min(std::max(tx_min, tx_max), std::max(ty_min, ty_max)), std::max(tz_min, tz_max));

    // If t_max < 0, the ray is intersecting the AABB behind the origin.
    // If t_min > t_max, the ray does not intersect the AABB.
    if(t_max < 0.0f || t_min > t_max)
    {
        return false;
    }

    PhxRaycastResult result;
    result.hit = true;
    result.t   = t_min;

    // If t_min < 0, the ray starts inside the AABB. Therefore, t_max is the closest intersection point.
    if(t_min < 0.0f)
    {
        result.t = t_max;
    }

    result.point = ray.origin + ray.direction * result.t;

    // Determine the normal of the intersection point.
    glm::vec3 normals[] = {{-1.0f, 0.0f, 0.0f},
                           {1.0f, 0.0f, 0.0f},
                           {0.0f, -1.0f, 0.0f},
                           {0.0f, 1.0f, 0.0f},
                           {0.0f, 0.0f, -1.0f},
                           {0.0f, 0.0f, 1.0f}};
    float     t[]       = {tx_min, tx_max, ty_min, ty_max, tz_min, tz_max};
    for(short i = 0; i < 6; ++i)
    {
        if(CMP_FLOAT_EQ(result.t, t[i]))
        {
            result.normal = normals[i];
            break; // Todo:: Is breaking correct?
        }
    }

    out_results.push_back(result);

    return true;
}

static bool isUnique(const std::vector<PhxRaycastResult>& results, const PhxRaycastResult& result)
{
    for(const auto& r : results)
    {
        bool is_t_equal     = CMP_FLOAT_EQ(r.t, result.t);
        bool is_point_equal = CMP_FLOAT_EQ(r.point.x, result.point.x) && CMP_FLOAT_EQ(r.point.y, result.point.y) &&
                              CMP_FLOAT_EQ(r.point.z, result.point.z);
        bool is_normal_equal = CMP_FLOAT_EQ(r.normal.x, result.normal.x) && CMP_FLOAT_EQ(r.normal.y, result.normal.y) &&
                               CMP_FLOAT_EQ(r.normal.z, result.normal.z);
        if(is_t_equal && is_point_equal && is_normal_equal)
        {
            return false;
        }
    }
    return true;
}

bool phxRaycast(const PhxRay&                  ray,
                const PhxTriangleMesh&         triangle_mesh,
                std::vector<PhxRaycastResult>& out_results,
                const PhxQueryMode&               query_mode)
{
    const auto& bvh               = triangle_mesh.getBvh();
    const auto& bvh_nodes         = bvh->getNodes();
    const auto& primitive_indices = bvh->getPrimitiveIndices();
    const auto& triangles         = triangle_mesh.getTriangles();
    bool        hit_found         = false;

    std::stack<PhxBvhNode> nodes_to_visit;
    nodes_to_visit.push(bvh_nodes[0]);

    while(!nodes_to_visit.empty())
    {
        PhxBvhNode node = nodes_to_visit.top();
        nodes_to_visit.pop();

        if(node.isLeaf())
        {
            for(uint32_t i = node.idx; i < node.idx + node.num_primitives; ++i)
            {
                const auto&      triangle       = triangles[primitive_indices[i]];
                PhxRaycastResult raycast_result = {};
                if(raycastTriangle(ray, triangle, raycast_result))
                // if(raycastTriangleBarycentric(ray, triangle, raycast_result))
                {
                    //APPLICATION_INFO("Ray intersection detected: Triangle idx: {}", primitive_indices[i]);
                    //APPLICATION_INFO("Ray intersection detected: Triangle: [({}, {}, {}), ({}, {}, {}), ({}, {}, {})]",
                    //                 triangle.a.x,
                    //                 triangle.a.y,
                    //                 triangle.a.z,
                    //                 triangle.b.x,
                    //                 triangle.b.y,
                    //                 triangle.b.z,
                    //                 triangle.c.x,
                    //                 triangle.c.y,
                    //                 triangle.c.z);
                    //APPLICATION_INFO("Ray intersection detected: Intersection Point: {}, {}, {}",
                    //                 raycast_result.point.x,
                    //                 raycast_result.point.y,
                    //                 raycast_result.point.z);
                    //APPLICATION_INFO("Ray intersection detected: Intersection t: {}", raycast_result.t);
                    hit_found = true;
                    if(query_mode == PhxQueryMode::AnyHit)
                    {
                        // If we are looking for the any hit, we can return early.
                        out_results.push_back(raycast_result);
                        // return hit_found;
                        break;
                    }
                    else if(query_mode == PhxQueryMode::ClosestHit)
                    {
                        if(out_results.empty())
                        {
                            // Add the result if there are no hits yet.
                            out_results.emplace_back(raycast_result);
                        }
                        else
                        {
                            // Replace the result if the new hit is closer.
                            if(raycast_result.t < out_results[0].t)
                            {
                                out_results[0] = raycast_result;
                            }
                        }
                    }
                    else
                    {
                        // Record all hits.
                        if(isUnique(out_results, raycast_result))
                        {
                            out_results.push_back(raycast_result);
                        }
                    }
                }
            }
        }
        else
        {
            // Push the child nodes to the stack, only if the ray intersects the AABB of the child nodes.
            std::vector<phx::PhxRaycastResult> results_1;
            if(raycastAABB(ray, bvh_nodes[static_cast<size_t>(node.idx)].aabb, results_1))
            {
                nodes_to_visit.push(bvh_nodes[static_cast<size_t>(node.idx)]); // Left child
            }
            std::vector<phx::PhxRaycastResult> results_2;
            if(raycastAABB(ray, bvh_nodes[static_cast<size_t>(node.idx) + 1].aabb, results_2))
            {
                nodes_to_visit.push(bvh_nodes[static_cast<size_t>(node.idx) + 1]); // Right child
            }
        }
    }

    return hit_found;
}

} // namespace phx