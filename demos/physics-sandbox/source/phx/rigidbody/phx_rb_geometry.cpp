#include "phx_rb_geometry.hpp"
#include "phx_rigid_body.hpp"
#include "../phx_math_utils.hpp"

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

void PhxGeometry::updateGeometry()
{
    m_transform = m_rigid_body->getWorldTransform() * m_offset;
}

PhxVec3 PhxGeometry::getAxis(unsigned int index) const
{
    const PhxVec4& column = m_transform[index];
    return PhxVec3(column[0], column[1], column[2]);
}

PhxSphereGeometry::PhxSphereGeometry() : PhxGeometry(PhxGeometryType::Sphere) {}

PhxHalfSpaceGeometry::PhxHalfSpaceGeometry()
    : PhxGeometry(PhxGeometryType::HalfSpace)
    , m_normal(0.0f, 1.0f, 0.0f)
    , m_distance(0.0f)
{
}

PhxBoxGeometry::PhxBoxGeometry() : PhxGeometry(PhxGeometryType::Box) {}

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

////////////////////////////// Collision methods //////////////////////////////////////

unsigned int
phxCollide(const PhxBoxGeometry& box, const PhxHalfSpaceGeometry& half_space, PhxCollisionData* collision_data)
{
    return 0;
}

} // namespace phx::rb