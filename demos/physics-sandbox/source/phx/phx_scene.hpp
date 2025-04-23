#ifndef PHX_SCENE_H
#define PHX_SCENE_H

#include "rigidbody/phx_rigid_body.hpp"
#include "rigidbody/phx_rb_force_generator.hpp"
#include "rigidbody/phx_rb_geometry.hpp"
#include "rigidbody/phx_rb_contact.hpp"

namespace phx
{

using phx::rb::PhxContact;
using phx::rb::PhxGeometry;
using phx::rb::PhxRbForceGenerator;
using phx::rb::PhxRbForceRegistry;
using phx::rb::PhxRigidBody;

using RigidBodies = std::vector<PhxRigidBody*>;
using Geometries  = std::vector<PhxGeometry*>;

class PhxScene
{
public:
    PhxScene()          = default;
    virtual ~PhxScene() = default;

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
    void integrateDynamic(const PhxReal& duration);
    void integrateDynamicWithBroadPhase(const PhxReal& duration);

    void resolveContact(const PhxContact& contact, const PhxReal& dt);

protected:
    RigidBodies        m_rigid_bodies;
    Geometries         m_geometries;
    PhxRbForceRegistry m_force_registry;
};

} // namespace phx

#endif // !PHX_WORLD_H
