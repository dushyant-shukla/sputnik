#ifndef PHX_WORLD_H
#define PHX_WORLD_H

#include "phx/rigidbody/phx_rigid_body.hpp"
#include "phx/rigidbody/phx_rb_force_generator.hpp"
#include "phx/rigidbody/phx_rb_geometry.hpp"
#include "phx/rigidbody/phx_rb_contact.hpp"

namespace sputnik::physics
{

using phx::rb::PhxGeometry;
using phx::rb::PhxRbForceGenerator;
using phx::rb::PhxRbForceRegistry;
using phx::rb::PhxRigidBody;
using phx::rb::PhxContact;

using RigidBodies = std::vector<PhxRigidBody*>;
using Geometries  = std::vector<PhxGeometry*>;

class PhysicsWorld
{
public:
    PhysicsWorld()          = default;
    virtual ~PhysicsWorld() = default;

    void startFrame();
    void runPhysics(const PhxReal& duration);

    void addRigidBody(PhxRigidBody* body);
    void addGeometry(PhxGeometry* geometry);
    void addForceGenerator(PhxRigidBody* body, PhxRbForceGenerator* fgen);

    RigidBodies::const_iterator rigidBodiesBegin() const;
    RigidBodies::const_iterator rigidBodiesEnd() const;
    Geometries::const_iterator  geometriesBegin() const;
    Geometries::const_iterator  geometriesEnd() const;

protected:
    void integrate(const PhxReal& duration);

    void resolveContact(const PhxContact& contact);

protected:
    RigidBodies        m_rigid_bodies;
    Geometries         m_geometries;
    PhxRbForceRegistry m_force_registry;
};

} // namespace sputnik::physics

#endif // !PHX_WORLD_H
