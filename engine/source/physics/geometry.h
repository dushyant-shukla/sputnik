#pragma once

#include <vector.hpp>
#include <matrix.hpp>
#include <optional>

namespace sputnik::physics
{

using namespace ramanujan;
using namespace ramanujan::experimental;

/*!
 * @brief A point is the simplest entity that can be simulated in a physics engine.
 */
using Point = vec3;

struct Line
{
    Point start;
    Point end;

    Line() noexcept = default;
    Line(const Line& other) noexcept;
    Line(Line&& other) noexcept;
    Line(const Point& _start, const Point& _end) noexcept;
    Line(const Point& _end) noexcept;

    real length() const noexcept;
    real lengthSquared() const noexcept;
};

struct Triangle
{
    union
    {
        struct
        {
            Point a;
            Point b;
            Point c;
        };
        Point vertices[3];
        real  values[9] = {real(0.0)};
    };

    Triangle() noexcept = default;
    Triangle(const Triangle& other) noexcept;
    Triangle(Triangle&& other) noexcept;
    Triangle(const Point& _a, const Point& _b, const Point& _c) noexcept;
};

/**
 * @brief A plane is a flat surface that extents infinitely is all directions.
 */
struct Plane
{
    vec3 normal;
    real distance;

    Plane() noexcept;
    Plane(const Plane& other) noexcept;
    Plane(Plane&& other) noexcept;
    Plane(const vec3& _normal, const real& _distance) noexcept;

    /*!
     * @brief Evaluate the distance of the given point from origin along the normal of this plane. The method assumes
     * the normal is of unit length.
     *
     * @param point
     * @return The distance of the point from the origin along the normal of this plane.
     */
    real distanceToPoint(const Point& point) const noexcept;

    /*!
     * @brief Evaluate the distance of the point from the plane along the normal of the plane. Use this method to check
     * if the given point lies on this plane.
     *
     * @return distance of the point from the plane along the normal of the plane.
     */
    real planeEquation(const Point& point) const noexcept;
};

struct AABB
{
    Point center;
    vec3  half_extents;

    AABB() noexcept;
    AABB(const AABB& other) noexcept;
    AABB(AABB&& other) noexcept;
    AABB(const Point& _center, const vec3& _half_extents) noexcept;

    vec3        min() const noexcept;
    vec3        max() const noexcept;
    friend AABB fromMinMax(const vec3& min, const vec3& max) noexcept;
};

struct OBB
{
    Point center;
    vec3  half_extents;
    mat3  orientation;

    OBB() noexcept;
    OBB(const OBB& other) noexcept;
    OBB(OBB&& other) noexcept;
    OBB(const Point& _center, const vec3& _half_extents, const mat3& _orientation) noexcept;
};

struct Sphere
{
    Point center;
    real  radius;

    Sphere() noexcept;
    Sphere(const Sphere& other) noexcept;
    Sphere(Sphere&& other) noexcept;
    Sphere(const Point& _center, real _radius) noexcept;
};

struct Ray
{
    Point origin;
    vec3  direction;

    Ray() noexcept;
    Ray(const Ray& other) noexcept;
    Ray& operator=(const Ray& other) noexcept;
    Ray(Ray&& other) noexcept;
    Ray(const Point& _origin, const vec3& _direction);

    void       normalizeDirection() noexcept;
    friend Ray fromPoints(const Point& from, const Point& to) noexcept;
};

struct RaycastResult
{
    bool hit{false};
    real t{-1.0f};
    vec3 normal{0.0f, 0.0f, 1.0f};
    vec3 point{0.0f};
};

// Ray GetPickRay(const vec2& viewportPoint,
//                const vec2& viewportOrigin,
//                const vec2& viewportSize,
//                const mat4& view,
//                const mat4& projection) noexcept
//{
//	vec2 ndc = (viewportPoint - viewportOrigin) / viewportSize;
//	ndc.y = 1.0f - ndc.y;
//	vec4 clip = {ndc.x * 2.0f - 1.0f, ndc.y * 2.0f - 1.0f, -1.0f, 1.0f};
//	//vec4 eye = inverse(projection) * clip;
//     vec4 eye   = projection.inverted() * clip;
//	eye.z = -1.0f;
//	eye.w = 0.0f;
//	vec3 world = view.inverted() * eye;
//     return {vec3{0.0f}, world.normalized()};
// }

Ray GetPickRay_2(const vec2& viewportPoint,
                 const vec2& viewportOrigin,
                 const vec2& viewportSize,
                 const mat4& view,
                 const mat4& projection);
Ray getPickRay_1(vec2 ndc_coordinates, const mat4& projection, const mat4& view) noexcept;
Ray getPickRay(vec2 ndc_coordinates, const mat4& projection, const mat4& view) noexcept;

std::optional<RaycastResult> raycast(const Ray& ray, const Sphere& sphere) noexcept;

} // namespace sputnik::physics
