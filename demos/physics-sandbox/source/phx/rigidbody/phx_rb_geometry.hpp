#pragma once

#include "../phx_types.hpp"
#include "../phx_math_utils.hpp"
#include "phx_rb_contact.hpp"
#include "../phx_bounds.hpp"

namespace phx::rb
{

class PhxRigidBody;

using PhxPoint = PhxVec3;

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

    virtual PhxVec3 getPosition() const;

    virtual void updateTransform();

    virtual PhxVec3 getAxis(unsigned int index) const;

    virtual const PhxExtent& getBounds() const                                                    = 0;
    virtual PhxExtent        getBounds(const PhxVec3& position, const PhxQuat& orientation) const = 0;

    /*!
     * @brief The method returns the support point of the geometry in the given direction. For a convex set of points
     * (vertices), a support point (or extreme point) is defined as the point in the set that is furthest in the given
     * direction.
     *
     * @param direction Direction in which to find the support point. This is a unit vector.
     * @param position Position of the geometry in world space.
     * @param orientation Orientation of the geometry in world space (unused for some geometries like sphere).
     * @param bias Bias to add to the support point in the given direction. Useful for shrinking and expanding the size
     * of the shape.
     * @return The support point in the given direction. This is a point in world space.
     */
    virtual PhxVec3 getSupportPoint(const PhxVec3& direction,
                                    const PhxVec3& position,
                                    const PhxQuat& orientation,
                                    const PhxReal& bias) const = 0;

    /*!
     * @brief The method calculates the fastest linear speed of the vertex on the geometry in the given direction.
     * @param angular_velocity Angular velocity of the box in world space.
     * @param direction Direction in which to calculate the fastest linear speed. This is a unit vector.
     * @return The fastest linear speed of the vertex on the box in the given direction.
     *         Zero for some shapes such as spheres.
     */
    virtual PhxReal fastestLinearSpeed(const PhxVec3& angular_velocity, const PhxVec3& direction) const;

public:
    PhxRigidBody* m_rigid_body{nullptr};  // The rigid body to which this primitive is attached.
    PhxMat4       m_offset{PhxReal(0.0)}; // Offset of the primitive from the body's center of mass.

protected:
    PhxMat4         m_transform{PhxReal(0.0)}; // Transformation matrix of the primitive.
    PhxGeometryType m_type;                    // Type of the geometry.
    PhxVec3         m_center_of_mass{PhxReal(0.0)};
    PhxMat3         m_inertia_tensor;
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

    virtual const PhxExtent& getBounds() const;
    virtual PhxExtent        getBounds(const PhxVec3& position, const PhxQuat& orientation) const;

    /*!
     * @brief The method calculate the support point of the sphere in the given direction.
     * @param direction Direction in which to find the support point.
     * @param position Position of the sphere in world space.
     * @param orientation Orientation of the sphere in world space (unused for sphere).
     * @param bias Bias to add to the support point in the given direction. Useful for shriking and expanding the size
     * of the shape.
     * @return The support point in the given direction.
     */
    virtual PhxVec3 getSupportPoint(const PhxVec3& direction,
                                    const PhxVec3& position,
                                    const PhxQuat& orientation,
                                    const PhxReal& bias) const;

    void buildGeometry();

public:
    PhxReal   m_radius{0.0f}; // Radius of the sphere.
    PhxExtent m_bounds;
};

class PhxBoxGeometry : public PhxGeometry
{
public:
    explicit PhxBoxGeometry(const std::vector<PhxVec3>& points);

    virtual const PhxExtent& getBounds() const;
    virtual PhxExtent        getBounds(const PhxVec3& position, const PhxQuat& orientation) const;

    /*!
     * @brief The method calculate the support point of the box in the given direction.
     * @param direction Direction in which to find the support point.
     * @param position Position of the box in world space.
     * @param orientation Orientation of the box in world space.
     * @param bias Bias to add to the support point in the given direction. Useful for shriking and expanding the size
     * of the shape.
     * @return The support point in the given direction.
     */
    virtual PhxVec3 getSupportPoint(const PhxVec3& direction,
                                    const PhxVec3& position,
                                    const PhxQuat& orientation,
                                    const PhxReal& bias) const;

    /*!
     * @brief The method calculates the fastest linear speed of the vertex on the box in the given direction.
     * @param angular_velocity Angular velocity of the box in world space.
     * @param direction Direction in which to calculate the fastest linear speed.
     * @return The fastest linear speed of the vertex on the box in the given direction.
     */
    virtual PhxReal fastestLinearSpeed(const PhxVec3& angular_velocity, const PhxVec3& direction) const;

private:
    void buildGeometry(const PhxVec3Array& points);

    void calculateInertiaTensor();

public:
    // PhxVec3 m_half_extents{0.0f}; // Half extents of the box.

    PhxExtent    m_bounds; // Bounds of the box in local space.
    PhxVec3Array m_points; // Points of the box (corner vertices) in local space.
};

struct PhxRay
{
    PhxPoint origin;
    PhxVec3  direction;
    PhxReal  t = kPhxFloatMax; // Max because we want to find the closest intersection.
};

struct PhxRaycastResult
{
    PhxBool   hit{false};
    PhxReal   t{kPhxFloatMax};
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

bool phxIntersectDynamic(PhxGeometry* const geometry1,
                         PhxGeometry* const geometry2,
                         const PhxReal&     dt,
                         PhxContact&        contact_out);

bool phxIntersectSphereSphereDynamic(const PhxSphereGeometry* sphere1,
                                     const PhxSphereGeometry* sphere2,
                                     const PhxReal&           dt,
                                     PhxContact&              contact_out);

////////////////////////////// Collision Tests ///////////////////////////////////

struct PhxCollisionData
{
};

unsigned int
phxCollide(const PhxBoxGeometry& box, const PhxHalfSpaceGeometry& half_space, PhxCollisionData* collision_data);

///////////////////////////// Raycasting Tests //////////////////////////////

bool phxRaycast(PhxRay* const                  ray,
                PhxSphereGeometry* const       sphere,
                std::vector<PhxRaycastResult>& out_results,
                const PhxQueryMode&            query_mode = PhxQueryMode::AllHits);

bool phxRaycastSphere(PhxRay* const                  ray,
                      const PhxVec3&                 sphere_center,
                      const PhxReal&                 sphere_radius,
                      std::vector<PhxRaycastResult>& out_results);

} // namespace phx::rb