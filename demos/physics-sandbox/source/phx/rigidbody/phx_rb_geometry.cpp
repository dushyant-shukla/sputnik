#include "phx_rb_geometry.hpp"
#include "phx_rigid_body.hpp"

namespace phx::rb
{

//////////////////////////////////// PhxGeometry //////////////////////////////////////

PhxGeometry::PhxGeometry(PhxGeometryType type) : m_type{type}, m_rigid_body{nullptr}, m_offset{1.0f} {}

PhxGeometry::~PhxGeometry() {}

PhxGeometryType PhxGeometry::getType() const
{
    return m_type;
}

const PhxMat4& PhxGeometry::getTransform() const
{
    return m_transform;
}

PhxVec3 PhxGeometry::getPosition() const
{
    return getAxis(3);
}

void PhxGeometry::updateTransform()
{
    m_transform = m_rigid_body->getWorldTransform() * m_offset;
}

PhxVec3 PhxGeometry::getAxis(unsigned int index) const
{
    const PhxVec4& column = m_transform[index];
    return PhxVec3(column[0], column[1], column[2]);
}

PhxReal PhxGeometry::fastestLinearSpeed(const PhxVec3& angular_velocity, const PhxVec3& direction) const
{
    return PhxReal(0.0);
}

////////////////////////////////// PhxSphereGeometry ///////////////////////////////////

PhxSphereGeometry::PhxSphereGeometry() : PhxGeometry(PhxGeometryType::Sphere) {}

const PhxExtent& PhxSphereGeometry::getBounds() const
{
    return m_bounds;
}

PhxExtent PhxSphereGeometry::getBounds(const PhxVec3& position, const PhxQuat& orientation) const
{
    PhxExtent bounds;
    bounds.m_min = PhxVec3(-m_radius, -m_radius, -m_radius) + position;
    bounds.m_max = PhxVec3(m_radius, m_radius, m_radius) + position;
    return bounds;
}

PhxVec3 PhxSphereGeometry::getSupportPoint(const PhxVec3& direction,
                                           const PhxVec3& position,
                                           const PhxQuat& orientation,
                                           const PhxReal& bias) const
{
    PhxVec3 support_point = position + direction * (m_radius + bias);
    return support_point;
}

void PhxSphereGeometry::buildGeometry()
{
    m_bounds.m_min = PhxVec3(-m_radius, -m_radius, -m_radius);
    m_bounds.m_max = PhxVec3(m_radius, m_radius, m_radius);
    updateTransform();
}

////////////////////////////////// PhxHalfSpaceGeometry /////////////////////////////////

PhxHalfSpaceGeometry::PhxHalfSpaceGeometry()
    : PhxGeometry(PhxGeometryType::HalfSpace)
    , m_normal(0.0f, 1.0f, 0.0f)
    , m_distance(0.0f)
{
}

///////////////////////////////////// PhxBoxGeometry ///////////////////////////////////

PhxBoxGeometry::PhxBoxGeometry(const std::vector<PhxVec3>& points) : PhxGeometry(PhxGeometryType::Box)
{
    buildGeometry(points);
}

const PhxExtent& PhxBoxGeometry::getBounds() const
{
    return m_bounds;
}

PhxExtent PhxBoxGeometry::getBounds(const PhxVec3& position, const PhxQuat& orientation) const
{
    PhxExtent    bounds;
    PhxVec3Array points = m_points;
    for(auto& point : points)
    {
        point = phxRotatePoint(orientation, point) + position;
        bounds.expand(point);
    }
    return bounds;
}

PhxVec3 PhxBoxGeometry::getSupportPoint(const PhxVec3& direction,
                                        const PhxVec3& position,
                                        const PhxQuat& orientation,
                                        const PhxReal& bias) const
{
    PhxVec3 max_point    = phxRotatePoint(orientation, m_bounds.m_max) + position;
    PhxReal max_distance = phx_dot(direction, max_point);

    for(PhxSize i = 1; i < m_points.size(); ++i)
    {
        PhxVec3 point    = phxRotatePoint(orientation, m_points[i]) + position;
        PhxReal distance = phx_dot(direction, point);
        if(distance > max_distance)
        {
            max_distance = distance;
            max_point    = point;
        }
    }

    PhxVec3 normalized_direction = phx_normalize(direction);
    PhxVec3 bias_vector          = normalized_direction * bias;
    return max_point + bias_vector;
}

PhxReal PhxBoxGeometry::fastestLinearSpeed(const PhxVec3& angular_velocity, const PhxVec3& direction) const
{
    PhxReal max_speed(0.0);
    for(const auto& point : m_points)
    {
        PhxVec3 r               = point - m_center_of_mass;
        PhxVec3 linear_velocity = phx_cross(angular_velocity, r);
        PhxReal speed           = phx_dot(linear_velocity, direction);
        if(speed > max_speed)
        {
            max_speed = speed;
        }
    }
    return max_speed;
}

void PhxBoxGeometry::buildGeometry(const PhxVec3Array& points)
{
    for(PhxSize i = 0; i < points.size(); ++i)
    {
        m_bounds.expand(points[i]);
    }

    m_points.clear();

    // Eight corner vertices of the box
    // Todo:: This is probably not needed. Just store the input points.
    m_points.emplace_back(PhxVec3(m_bounds.m_min.x, m_bounds.m_min.y, m_bounds.m_min.z));
    m_points.emplace_back(PhxVec3(m_bounds.m_max.x, m_bounds.m_min.y, m_bounds.m_min.z));
    m_points.emplace_back(PhxVec3(m_bounds.m_max.x, m_bounds.m_max.y, m_bounds.m_min.z));
    m_points.emplace_back(PhxVec3(m_bounds.m_min.x, m_bounds.m_max.y, m_bounds.m_min.z));
    m_points.emplace_back(PhxVec3(m_bounds.m_min.x, m_bounds.m_min.y, m_bounds.m_max.z));
    m_points.emplace_back(PhxVec3(m_bounds.m_max.x, m_bounds.m_min.y, m_bounds.m_max.z));
    m_points.emplace_back(PhxVec3(m_bounds.m_max.x, m_bounds.m_max.y, m_bounds.m_max.z));
    m_points.emplace_back(PhxVec3(m_bounds.m_min.x, m_bounds.m_max.y, m_bounds.m_max.z));

    m_center_of_mass = (m_bounds.m_min + m_bounds.m_max) * PhxReal(0.5);
    m_rigid_body->setCenterOfMass(m_center_of_mass);
}

void PhxBoxGeometry::calculateInertiaTensor()
{
    // The inertia tensor for a box is calculated using the formula:
    // I = (1/12) * m * (h^2 + w^2) for each axis, where h and w are the half extents of the box.
    PhxReal mass = m_rigid_body->getMass();
    PhxReal dx   = m_bounds.m_max.x - m_bounds.m_min.x;
    PhxReal dy   = m_bounds.m_max.y - m_bounds.m_min.y;
    PhxReal dz   = m_bounds.m_max.z - m_bounds.m_min.z;
    PhxReal ix   = (1.0f / 12.0f) * mass * (dy * dy + dz * dz);
    PhxReal iy   = (1.0f / 12.0f) * mass * (dx * dx + dz * dz);
    PhxReal iz   = (1.0f / 12.0f) * mass * (dx * dx + dy * dy);

    // We also need to account for situations where the box is not centered at the origin.
    // We will use the parallel axis theorem to adjust the inertia tensor.
    // The parallel axis theorem states that the inertia tensor around an axis of rotation that is parallel to the axis
    // of rotation passing through the center of mass is given by:
    // I = Icm + m * d^2, where,
    // Icm is the inertia tensor around the center of mass,
    // m is the mass of the body, and
    // d is the distance from the center of mass to the axis of rotation.

    const PhxVec3& r        = m_center_of_mass;
    const PhxReal  r_square = phx_magnitude_sq(r);
    ix += mass * (r_square - r.x * r.x);
    iy += mass * (r_square - r.y * r.y);
    iz += mass * (r_square - r.z * r.z);
    PhxReal ixy = r.x * r.y;
    PhxReal ixz = r.x * r.z;
    PhxReal iyz = r.y * r.z;

    m_rigid_body->setInertiaTensorWithCoefficients(ix, iy, iz, ixy, ixz, iyz);
}

/////////////////////////////// Intersection methods ///////////////////////////////////

bool phxIntersect(const PhxBoxGeometry& box1, const PhxBoxGeometry& box2)
{
    return false;
}

bool phxIntersect(const PhxBoxGeometry& box, const PhxHalfSpaceGeometry& half_space)
{
    return false;
}

bool phxIntersect(const PhxSphereGeometry* sphere1, const PhxSphereGeometry* sphere2, PhxContact& contact_out)
{
    PhxVec3 distance_vector    = sphere2->getTransform()[3] - sphere1->getTransform()[3];
    PhxReal distance_vector_sq = phx_magnitude_sq(distance_vector);
    PhxReal sum_radii          = sphere1->m_radius + sphere2->m_radius;
    if(distance_vector_sq < (sum_radii * sum_radii))
    {
        contact_out.body_a       = sphere1->m_rigid_body;
        contact_out.body_b       = sphere2->m_rigid_body;
        contact_out.normal_world = phx_normalize(distance_vector);
        contact_out.point_on_a_world =
            PhxVec3(sphere1->getTransform()[3]) + contact_out.normal_world * sphere1->m_radius;
        contact_out.point_on_b_world =
            PhxVec3(sphere2->getTransform()[3]) - contact_out.normal_world * sphere2->m_radius;
        return true;
    }
    return false;
}

bool phxIntersectDynamic(PhxGeometry* const geometry1,
                         PhxGeometry* const geometry2,
                         const PhxReal&     dt,
                         PhxContact&        contact_out)
{
    contact_out.body_a = geometry1->m_rigid_body;
    contact_out.body_b = geometry2->m_rigid_body;

    if(geometry1->getType() == PhxGeometryType::Sphere && geometry2->getType() == PhxGeometryType::Sphere)
    {
        const PhxSphereGeometry* sphere1 = static_cast<PhxSphereGeometry*>(geometry1);
        const PhxSphereGeometry* sphere2 = static_cast<PhxSphereGeometry*>(geometry2);

        PhxVec3 pos_a = sphere1->m_rigid_body->getWorldPosition();
        PhxVec3 pos_b = sphere2->m_rigid_body->getWorldPosition();

        PhxVec3 velocity_a = sphere1->m_rigid_body->getLinerVelocity();
        PhxVec3 velocity_b = sphere2->m_rigid_body->getLinerVelocity();

        if(phxIntersectSphereSphereDynamic(sphere1, sphere2, dt, contact_out))
        {
            // Step bodies forward to get the local space collision points
            sphere1->m_rigid_body->update(contact_out.time_of_impact);
            sphere2->m_rigid_body->update(contact_out.time_of_impact);

            contact_out.point_on_a_local = sphere1->m_rigid_body->getPointInLocalSpace(contact_out.point_on_a_world);
            contact_out.point_on_b_local = sphere2->m_rigid_body->getPointInLocalSpace(contact_out.point_on_b_world);

            contact_out.normal_world = phx_normalize(pos_a - pos_b);

            // Unwind time step
            sphere1->m_rigid_body->update(-contact_out.time_of_impact);
            sphere2->m_rigid_body->update(-contact_out.time_of_impact);

            // Set the penetration depth
            PhxVec3 ab                      = pos_b - pos_a;
            contact_out.m_penetration_depth = phx_magnitude(ab) - (sphere1->m_radius + sphere2->m_radius);

            return true;
        }
    }

    return false;
}

bool phxIntersectSphereSphereDynamic(const PhxSphereGeometry* sphere1,
                                     const PhxSphereGeometry* sphere2,
                                     const PhxReal&           dt,
                                     PhxContact&              contact_out)
{
    const PhxVec3& vel_a   = sphere1->m_rigid_body->getLinerVelocity();
    const PhxVec3& vel_b   = sphere2->m_rigid_body->getLinerVelocity();
    const PhxVec3  rel_vel = vel_a - vel_b;

    const PhxVec3 start_point_a = sphere1->getPosition();
    const PhxVec3 end_point_a   = start_point_a + rel_vel * dt;
    const PhxVec3 ray_dir       = end_point_a - start_point_a;
    PhxRay        ray;
    ray.origin    = start_point_a;
    ray.direction = ray_dir;

    std::vector<PhxRaycastResult> raycast_results;
    if(phx_magnitude_sq(ray_dir) < kPhxEpsilon)
    {
        // The ray is too short, just check if it's already intersecting
        PhxVec3 distance_vector = sphere2->getPosition() - sphere1->getPosition();
        PhxReal total_radius    = sphere1->m_radius + sphere2->m_radius + 0.001f;
        if(phx_magnitude_sq(distance_vector) > total_radius * total_radius)
        {
            return false;
        }
    }
    else if(!phxRaycastSphere(&ray, sphere2->getPosition(), sphere1->m_radius + sphere2->m_radius, raycast_results))
    {
        return false;
    }

    if(raycast_results.empty())
    {
        // Ideally this shouldn't happen, but it's happening
        // Todo: Check raycast sphere implementation
        return false;
    }

    // Change from [0,1] range to [0,dt] range
    PhxReal t0 = raycast_results[0].t * dt;
    PhxReal t1 = raycast_results[1].t * dt;

    // If the collision is only in the past, then there's no future collision this frame
    if(t1 < 0.0f)
    {
        return false;
    }

    // Get the earliest positive time of impact
    contact_out.time_of_impact = (t0 < 0.0f) ? 0.0f : t0;

    // If the earliest collision is too far in the future, then there is no collision in this frame
    if(contact_out.time_of_impact > dt)
    {
        return false;
    }

    PhxVec3 new_pos_a       = sphere1->getPosition() + vel_a * contact_out.time_of_impact;
    PhxVec3 new_pos_b       = sphere2->getPosition() + vel_b * contact_out.time_of_impact;
    PhxVec3 distance_vector = phx_normalize(new_pos_b - new_pos_a);

    contact_out.point_on_a_world = new_pos_a + distance_vector * sphere1->m_radius;
    contact_out.point_on_b_world = new_pos_b - distance_vector * sphere2->m_radius;

    return true;
}

////////////////////////////// Collision methods //////////////////////////////////////

unsigned int
phxCollide(const PhxBoxGeometry& box, const PhxHalfSpaceGeometry& half_space, PhxCollisionData* collision_data)
{
    return 0;
}

bool phxRaycast(PhxRay* const                  ray,
                PhxSphereGeometry* const       sphere,
                std::vector<PhxRaycastResult>& out_results,
                const PhxQueryMode&            query_mode)
{
    // Equation of sphere: ||P - C|| = r^2
    // Where P is the point on the sphere, C is the center of the sphere, and r is the radius of the sphere.

    // Here, P = ray->origin + ray->direction * t
    // Substituting in the equation of the sphere:
    // ||ray->origin + ray->direction * t - C|| = r^2
    // Expanding this gives us a quadratic equation in t of the form: at^2 + bt + c = 0
    // (ray->dir * ray->dir) * t^2 + 2 * (ray->origin - C) * ray->dir * t + (ray->origin - C) * (ray->origin - C) - r^2
    // = 0

    const PhxVec3 oc           = ray->origin - sphere->getPosition();
    const PhxReal a            = phx_dot(ray->direction, ray->direction);
    const PhxReal b            = 2.0f * phx_dot(oc, ray->direction);
    const PhxReal c            = phx_dot(oc, oc) - (sphere->m_radius * sphere->m_radius);
    const PhxReal discriminant = b * b - 4.0f * a * c;
    if(discriminant < 0.0f)
    {
        return false; // No intersection
    }

    // Calculate the two intersection points
    const PhxReal    t1 = (-b - sqrtf(discriminant)) / (2.0f * a);
    const PhxReal    t2 = (-b + sqrtf(discriminant)) / (2.0f * a);
    PhxRaycastResult result;
    result.hit = true;

    switch(query_mode)
    {
    case PhxQueryMode::ClosestHit:
        if(t1 < 0.0f && t2 < 0.0f)
        {
            return false; // Both intersection points are behind the ray
        }
        else if(t1 < 0.0f)
        {
            result.t = t2;
        }
        else if(t2 < 0.0f)
        {
            result.t = t1;
        }
        else
        {
            result.t = std::min(t1, t2);
        }
        out_results.push_back(result);
        return true;
        break;

    case PhxQueryMode::AnyHit:
        if(t1 < 0.0f && t2 < 0.0f)
        {
            return false; // Both intersection points are behind the ray
        }
        else if(t1 < 0.0f)
        {
            result.t = t2;
            out_results.push_back(result);
            return true;
        }
        else if(t2 < 0.0f)
        {
            result.t = t1;
            out_results.push_back(result);
            return true;
        }
        break;

    case PhxQueryMode::AllHits:
        if(t1 >= 0.0f)
        {
            result.t = t1;
            out_results.push_back(result);
        }
        if(t2 >= 0.0f)
        {
            result.t = t2;
            out_results.push_back(result);
        }
        return true;
    default:
        break;
    }

    return true;
}

bool phxRaycastSphere(PhxRay* const                  ray,
                      const PhxVec3&                 sphere_center,
                      const PhxReal&                 sphere_radius,
                      std::vector<PhxRaycastResult>& out_results)
{
    // Equation of sphere: ||P - C|| = r^2
    // Where P is the point on the sphere, C is the center of the sphere, and r is the radius of the sphere.

    // Here, P = ray->origin + ray->direction * t
    // Substituting in the equation of the sphere:
    // ||ray->origin + ray->direction * t - C|| = r^2
    // Expanding this gives us a quadratic equation in t of the form: at^2 + bt + c = 0
    // (ray->dir * ray->dir) * t^2 + 2 * (ray->origin - C) * ray->dir * t + (ray->origin - C) * (ray->origin - C) - r^2
    // = 0

    const PhxVec3 oc = ray->origin - sphere_center;
    const PhxReal a  = phx_dot(ray->direction, ray->direction);
    const PhxReal b  = 2.0f * phx_dot(oc, ray->direction);
    const PhxReal c  = phx_dot(oc, oc) - (sphere_radius * sphere_radius);

    // If the discriminant is negative, there is no intersection
    // If it is zero, there is one intersection point (the ray is tangent to the sphere)
    // If it is positive, there are two intersection points
    const PhxReal discriminant = b * b - 4.0f * a * c;
    if(discriminant < 0.0f)
    {
        return false; // No intersection
    }

    // Calculate the two intersection points
    const PhxReal t1 = (-b - sqrtf(discriminant)) / (2.0f * a);
    const PhxReal t2 = (-b + sqrtf(discriminant)) / (2.0f * a);

    PhxRaycastResult result;
    result.t = t1;
    if(t1 >= kPhxEpsilon)
    {
        // Todo:: calculate normal at intersection
        result.hit   = true;
        result.point = ray->origin + ray->direction * t1;
    }
    out_results.push_back(result);

    result.reset();
    result.t = t2;
    if(t2 >= kPhxEpsilon)
    {
        // Todo:: calculate normal at intersection
        result.hit   = true;
        result.point = ray->origin + ray->direction * t2;
    }
    out_results.push_back(result);

    return true;
}

} // namespace phx::rb