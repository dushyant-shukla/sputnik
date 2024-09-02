#pragma once

#include "geometry.hpp"

namespace phx
{

/*!
 * @brief Scene query mode
 * // Todo:: AnyHit is not supported yet.
 */
enum class PhxQueryMode
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

bool Raycast(const PhxPlane& plane, const PhxRay& ray, PhxRaycastResult outResult);

PhxPlane fromTriangle(const PhxTriangle& t);

bool raycastTriangle(const PhxRay& ray, const PhxTriangle& triangle, PhxRaycastResult& out_results);

glm::vec3 barycentric(const PhxPoint& point, const PhxTriangle& triangle);

glm::vec3 project(const glm::vec3& length, const glm::vec3& direction);

bool raycastTriangleBarycentric(const PhxRay& ray, const PhxTriangle& triangle, PhxRaycastResult& out_results);

bool raycastAABB(const PhxRay& ray, const PhxAABB& aabb, std::vector<PhxRaycastResult>& out_results);

bool phxRaycast(const PhxRay&                  ray,
                         const PhxTriangleMesh&         triangle_mesh,
                         std::vector<PhxRaycastResult>& out_results,
                         const PhxQueryMode&            query_mode = PhxQueryMode::ClosestHit);

} // namespace phx