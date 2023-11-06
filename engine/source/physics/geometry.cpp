#include "pch.h"

#include "geometry.h"

namespace sputnik::physics
{
//////////////////////////////// Line //////////////////////////////////////

Line::Line(const Line& other) noexcept : start(other.start), end(other.end) {}
Line::Line(Line&& other) noexcept : start(std::move(other.start)), end(std::move(other.end)) {}
Line::Line(const Point& _start, const Point& _end) noexcept : start(_start), end(_end) {}
Line::Line(const Point& _end) noexcept : start(real(0.0)), end(_end) {}

real Line::length() const noexcept
{
    return (start - end).length();
}

real Line::lengthSquared() const noexcept
{
    return (start - end).lengthSquared();
}
//////////////////////////////// Ray //////////////////////////////////////

Ray::Ray() noexcept : origin{real(0.0)}, direction{real(0.0), real(0.0), real(1.0)} {}

Ray::Ray(const Ray& other) noexcept : origin{other.origin}, direction{other.direction} {}

Ray::Ray(Ray&& other) noexcept : origin{std::move(other.origin)}, direction{std::move(other.direction)} {}

Ray::Ray(const Point& _origin, const vec3& _direction) : origin{_origin}, direction{_direction}
{
    normalizeDirection();
}

void Ray::normalizeDirection() noexcept
{
    direction.normalize();
}

Ray fromPoints(const Point& from, const Point& to) noexcept
{
    return Ray{from, to - from};
}

//////////////////////////////// Triangle //////////////////////////////////////

Triangle::Triangle(const Triangle& other) noexcept : a(other.a), b(other.b), c(other.c) {}
Triangle::Triangle(Triangle&& other) noexcept : a(std::move(other.a)), b(std::move(other.b)), c(std::move(other.c)) {}
Triangle::Triangle(const Point& _a, const Point& _b, const Point& _c) noexcept : a(_a), b(_b), c(_c) {}

//////////////////////////////// AABB //////////////////////////////////////

AABB::AABB() noexcept : center{real(0.0)}, half_extents{real(1.0)} {}

AABB::AABB(const AABB& other) noexcept : center{other.center}, half_extents{other.half_extents} {}

AABB::AABB(AABB&& other) noexcept : center{std::move(other.center)}, half_extents{std::move(other.half_extents)} {}

AABB::AABB(const Point& _center, const vec3& _half_extents) noexcept : center(_center), half_extents(_half_extents) {}

vec3 AABB::min() const noexcept
{
    vec3 p1 = center + half_extents;
    vec3 p2 = center - half_extents;
    return vec3{real_min(p1.x, p2.x), real_min(p1.y, p2.y), real_min(p1.z, p2.z)};
}

vec3 AABB::max() const noexcept
{
    vec3 p1 = center + half_extents;
    vec3 p2 = center - half_extents;
    return vec3{real_max(p1.x, p2.x), real_max(p1.y, p2.y), real_max(p1.z, p2.z)};
}

AABB fromMinMax(const vec3& min, const vec3& max) noexcept
{
    return AABB{real(0.5) * (min + max), real(0.5) * (max - min)};
}

//////////////////////////////// OBB //////////////////////////////////////

OBB::OBB() noexcept : center{real(0.0)}, half_extents{real(1.0)}, orientation{mat3::identity()} {}

OBB::OBB(const OBB& other) noexcept
    : center{other.center}
    , half_extents{other.half_extents}
    , orientation{other.orientation}
{
}

OBB::OBB(OBB&& other) noexcept
    : center{std::move(other.center)}
    , half_extents{std::move(other.half_extents)}
    , orientation{std::move(other.orientation)}
{
}

OBB::OBB(const Point& _center, const vec3& _half_extents, const mat3& _orientation) noexcept
    : center{_center}
    , half_extents{_half_extents}
    , orientation{_orientation}
{
}

//////////////////////////////// Sphere //////////////////////////////////////

Sphere::Sphere() noexcept : center{real(0.0)}, radius{real(1.0f)} {}

Sphere::Sphere(const Sphere& other) noexcept : center{other.center}, radius{other.radius} {}

Sphere::Sphere(Sphere&& other) noexcept : center{std::move(other.center)}, radius{std::move(other.radius)} {}

Sphere::Sphere(const Point& _center, real _radius) noexcept : center(_center), radius(_radius) {}

//////////////////////////////// Plane //////////////////////////////////////

Plane::Plane() noexcept
    : normal{real(1.0), real(0.0), real(0.0)} // Todo:: Why is x component 1.0?
    , distance{real(0.0)}
{
}

Plane::Plane(const Plane& other) noexcept : normal{other.normal}, distance{other.distance} {}

Plane::Plane(Plane&& other) noexcept : normal{std::move(other.normal)}, distance{std::move(other.distance)} {}

Plane::Plane(const vec3& _normal, const real& _distance) noexcept : normal{_normal}, distance{_distance} {}

real Plane::distanceToPoint(const Point& point) const noexcept
{
    return normal.dot(point);
}

real Plane::planeEquation(const Point& point) const noexcept
{
    return normal.dot(point) - distance;
}

} // namespace sputnik::physics
