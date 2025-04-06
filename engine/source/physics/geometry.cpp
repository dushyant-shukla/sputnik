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

Ray& Ray::operator=(const Ray& other) noexcept
{
    if(this != &other)
    {
        origin    = other.origin;
        direction = other.direction;
    }
    return *this;
}

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

// vec3 unproject(const vec3& screen_coordinates, const mat4& projection, const mat4& view, const vec4& viewport)
// noexcept
//{
//	// https://antongerdelan.net/opengl/raycasting.html
//	vec4 v;
//	v.x = (2.0f * screen_coordinates.x) / viewport.z - 1.0f;
//	v.y = 1.0f - (2.0f * screen_coordinates.y) / viewport.w;
//	v.z = 0.0f;
//	v.w = 1.0f;
//	mat4 inv_projection = projection.inverted();
//	mat4 inv_view       = view.inverted();
//	vec4 ray_eye        = inv_projection.transformPoint(v);
//	ray_eye             = vec4{ray_eye.x, ray_eye.y, -1.0f, 0.0f};
//	vec3 ray_world      = (inv_view.transformPoint(ray_eye.xyz(), 0.0f).xyz()).normalize();
//	return ray_world;
// }

vec3 unproject(const vec4& point, const mat4& projection, const mat4& view) noexcept
{
    // https://antongerdelan.net/opengl/raycasting.html
    mat4 inv_projection = projection.inverted();
    mat4 inv_view       = view.inverted();
    vec4 ray_eye        = inv_projection.transformPoint(point);
    ray_eye             = vec4{ray_eye.x, ray_eye.y, -1.0f, 0.0f};
    vec3 ray_world      = (inv_view.transformPoint(ray_eye.xyz(), 0.0f).xyz()).normalize();
    return ray_world;
}

Ray getPickRay_1(vec2 ndc_coordinates, const mat4& projection, const mat4& view) noexcept
{
    vec3 near_point = unproject(vec4{ndc_coordinates.x, ndc_coordinates.y, -1.0f, 1.0f}, projection, view);
    vec3 far_point  = unproject(vec4{ndc_coordinates.x, ndc_coordinates.y, 1.0f, 1.0f}, projection, view);
    vec3 normal     = (far_point - near_point).normalized();
    return Ray{near_point, normal};
}

Ray getPickRay(vec2 ndc_coordinates, const mat4& projection, const mat4& view) noexcept
{
    // https://antongerdelan.net/opengl/raycasting.html
    vec4 ray_clip{ndc_coordinates.x, ndc_coordinates.y, -1.0f, 1.0f};
    mat4 inv_projection = projection.inverted();
    // vec4 ray_eye        = inv_projection.transformPoint(ray_clip);
    vec4 ray_eye  = inv_projection.transformPoint(ray_clip.xyz(), 1.0);
    ray_eye       = vec4{ray_eye.x, ray_eye.y, -1.0f, 0.0f};
    mat4 inv_view = view.inverted();
    // vec3 ray_world      = (inv_view.transformPoint(ray_eye).xyz()).normalize();
    vec3 ray_world = (inv_view.transformPoint(ray_eye.xyz(), 0.0f).xyz()).normalize();
    return Ray({0.0f}, ray_world);
}

#define CMP(x, y) (fabsf(x - y) <= FLT_EPSILON * fmaxf(1.0f, fmaxf(fabsf(x), fabsf(y))))

vec3 Unproject_2(const vec3& viewportPoint,
                 const vec2& viewportOrigin,
                 const vec2& viewportSize,
                 const mat4& view,
                 const mat4& projection)
{
    // Step 1, Normalize the input vector to the view port
    float normalized[4] = {(viewportPoint.x - viewportOrigin.x) / viewportSize.x,
                           (viewportPoint.y - viewportOrigin.y) / viewportSize.y,
                           viewportPoint.z,
                           1.0f};

    // Step 2, Translate into NDC space
    float ndcSpace[4] = {normalized[0], normalized[1], normalized[2], normalized[3]};
    // X Range: -1 to 1
    ndcSpace[0] = ndcSpace[0] * 2.0f - 1.0f;
    // Y Range: -1 to 1, our Y axis is flipped!
    ndcSpace[1] = 1.0f - ndcSpace[1] * 2.0f;
    // Z Range: 0 to 1
    if(ndcSpace[2] < 0.0f)
    {
        ndcSpace[2] = 0.0f;
    }
    if(ndcSpace[2] > 1.0f)
    {
        ndcSpace[2] = 1.0f;
    }

    // Step 3, NDC to Eye Space
    mat4 invProjection = projection.inverted();
    // float eyeSpace[4]   = {0.0f, 0.0f, 0.0f, 0.0f};
    // Multiply(eyeSpace, ndcSpace, 1, 4, invProjection.asArray, 4, 4);
    //  eyeSpace = MultiplyPoint(ndcSpace, invProjection);
    vec4 eyeSpace = invProjection.transformPoint({ndcSpace[0], ndcSpace[1], ndcSpace[2], ndcSpace[3]});

    // Step 4, Eye Space to World Space
    mat4 invView = view.inverted();
    // float worldSpace[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    // Multiply(worldSpace, eyeSpace, 1, 4, invView.asArray, 4, 4);
    //  worldSpace = MultiplyPoint(eyeSpace, invView);
    vec4 worldSpace = invView.transformPoint(eyeSpace);

    // Step 5, Undo perspective divide!
    if(!CMP(worldSpace[3], 0.0f))
    {
        worldSpace[0] /= worldSpace[3];
        worldSpace[1] /= worldSpace[3];
        worldSpace[2] /= worldSpace[3];
    }

    // Return the resulting world space point
    return vec3(worldSpace[0], worldSpace[1], worldSpace[2]);
}

Ray GetPickRay_2(const vec2& viewportPoint,
                 const vec2& viewportOrigin,
                 const vec2& viewportSize,
                 const mat4& view,
                 const mat4& projection)
{
    vec3 nearPoint(viewportPoint.x, viewportPoint.y, 0.0f);
    vec3 farPoint(viewportPoint.x, viewportPoint.y, 1.0f);

    vec3 pNear = Unproject_2(nearPoint, viewportOrigin, viewportSize, view, projection);
    vec3 pFar  = Unproject_2(farPoint, viewportOrigin, viewportSize, view, projection);

    vec3 normal = (pFar - pNear).normalized();
    vec3 origin = pNear;

    return Ray(origin, normal);
}

std::optional<RaycastResult> raycast(const Ray& ray, const Sphere& sphere) noexcept
{
    // return std::optional<RaycastResult>();
    // return std::nullopt;
    // return {};

    // real radiusSq = sphere.radius * sphere.radius;

    // vec3 e   = sphere.center - ray.origin;
    // real eSq = e.magnitudeSquared();

    // real a = e.dot(ray.direction); // ray direction is normalized (assumed)

    vec3  e   = sphere.center - ray.origin;
    float rSq = sphere.radius * sphere.radius;

    float eSq = e.magnitudeSquared();
    float a   = e.dot(ray.direction); // ray.direction is assumed to be normalized
    float bSq = /*sqrtf(*/ eSq - (a * a) /*)*/;
    float f   = sqrt(fabsf((rSq) - /*(b * b)*/ bSq));

    // Assume normal intersection!
    float t = a - f;

    // No collision has happened
    if(rSq - (eSq - a * a) < 0.0f)
    {
        return std::nullopt;
    }
    // Ray starts inside the sphere
    else if(eSq < rSq)
    {
        // Just reverse direction
        t = a + f;
    }
    RaycastResult result;
    result.t      = t;
    result.hit    = true;
    result.point  = ray.origin + ray.direction * t;
    result.normal = (result.point - sphere.center).normalized();
    return result;
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
    : normal{real(1.0), real(0.0), real(0.0)} // Todo:: Why is x component 1.0? Could just be random
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
