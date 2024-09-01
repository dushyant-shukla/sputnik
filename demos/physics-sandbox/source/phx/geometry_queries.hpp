#pragma once

#include "geometry.hpp"

namespace phx
{

/*!
 * @brief Scene query mode
 * // Todo:: AnyHit is not supported yet.
 */
enum class QueryMode
{
    /*
     * ClosestHit: The closest intersection along the ray is returned.
     */
    ClosestHit,

    /*
     * AnyHit: Any intersection along the ray is returned.
     */
    AnyHit,

    /*
     * AllHits: All intersections along the ray are returned.
     */
    AllHits
};

bool Raycast(const Plane& plane, const Ray& ray, RayCastResult outResult);

Plane fromTriangle(const Triangle& t);

bool raycastTriangle(const Ray& ray, const Triangle& triangle, RayCastResult& out_results);

glm::vec3 barycentric(const Point& point, const Triangle& triangle);

glm::vec3 project(const glm::vec3& length, const glm::vec3& direction);

bool raycastTriangleBarycentric(const Ray& ray, const Triangle& triangle, RayCastResult& out_results);

bool raycastAABB(const Ray& ray, const AABB& aabb, std::vector<RayCastResult>& out_results);

bool raycastTriangleMesh(const Ray&                  ray,
                         const TriangleMesh&         triangle_mesh,
                         std::vector<RayCastResult>& out_results,
                         const QueryMode&            query_mode = QueryMode::ClosestHit);

} // namespace phx