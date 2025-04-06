#pragma once

#include "../phx_types.hpp"
#include "phx_rb_contact.hpp"

namespace phx::rb
{

class PhxRigidBody;

enum class PhxGeometryType
{
    Plane,
    HalfSpace,
    Sphere,
    Box,
    Capsule,
    Cone,
    Hull,
    TriangleMesh
};

class PhxGeometry
{
public:
    PhxGeometry(PhxGeometryType type);

    virtual ~PhxGeometry();

    virtual PhxGeometryType getType() const;

    virtual const PhxMat4& getTransform() const;

    virtual void updateGeometry();

    virtual PhxVec3 getAxis(unsigned int index) const;

public:
    PhxRigidBody* m_rigid_body{nullptr}; // The rigid body to which this primitive is attached.
    PhxMat4       m_offset{1.0f};        // Offset of the primitive from the body's center of mass.

protected:
    PhxMat4         m_transform{1.0f}; // Transformation matrix of the primitive.
    PhxGeometryType m_type;            // Type of the geometry.
};

struct PhxPlaneGeometry : public PhxGeometry
{
    PhxVec3 m_normal;   // Normal of the plane.
    PhxReal m_distance; // Distance from the origin.
};

/*!
 * @brief A half space represents a plane geometry that's infinite in extent in one direction (usually the back side of
 * the plane).
 */
class PhxHalfSpaceGeometry : public PhxGeometry
{
public:
    PhxHalfSpaceGeometry();

public:
    PhxVec3 m_normal{0.0f, 1.0f, 0.0f}; // Normal of the plane.
    PhxReal m_distance{0.0f};           // Distance from the origin.
};

class PhxSphereGeometry : public PhxGeometry
{
public:
    PhxSphereGeometry();

public:
    PhxReal m_radius{0.0f}; // Radius of the sphere.
};

class PhxBoxGeometry : public PhxGeometry
{
public:
    PhxBoxGeometry();

public:
    PhxVec3 m_half_extents{0.0f}; // Half extents of the box.
};

/////////////////////////////// Intersection Tests /////////////////////////////////

/*!
 * @brief A fast intersection test on two arbitrarily aligned box collision shapes. The intersection test can be used in
 * broad phase coliision detection or as a early-out test during narrow phase collision.
 *
 * @param box1 The first box shape
 * @param box2 The second box shape
 * @return True, if the boxes are in collision, false otherwise.
 */
bool phxIntersect(const PhxBoxGeometry& box1, const PhxBoxGeometry& box2);

/*!
 * @brief A fast intersection test between an arbitrarily aligned box and a half-space. The intersection test can be
 * used in broad phase coliision detection or as a early-out test during narrow phase collision.
 *
 * @param box The box shape
 * @param half_space The half space
 * @return True, if the box and the plane are in collision, false otherwise.
 */
bool phxIntersect(const PhxBoxGeometry& box, const PhxHalfSpaceGeometry& half_space);

bool phxIntersect(const PhxSphereGeometry* sphere1, const PhxSphereGeometry* sphere2, PhxContact& contact_out);

////////////////////////////// Collision Tests ///////////////////////////////////

struct PhxCollisionData
{
};

unsigned int
phxCollide(const PhxBoxGeometry& box, const PhxHalfSpaceGeometry& half_space, PhxCollisionData* collision_data);

} // namespace phx::rb