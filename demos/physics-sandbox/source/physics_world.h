#ifndef PHX_WORLD_H
#define PHX_WORLD_H

#include "phx/rigidbody/phx_rigid_body.hpp"
#include "phx/rigidbody/phx_rb_force_generator.hpp"

namespace sputnik::physics
{

using phx::rb::PhxRigidBody;
using RigidBodies = std::vector<PhxRigidBody*>;
using phx::rb::PhxRbForceGenerator;
using phx::rb::PhxRbForceRegistry;

class PhysicsWorld
{
public:
    PhysicsWorld()          = default;
    virtual ~PhysicsWorld() = default;

    void startFrame();
    void runPhysics(const PhxReal& duration);

    void addRigidBody(PhxRigidBody* body);
    void addForceGenerator(PhxRigidBody* body, PhxRbForceGenerator* fgen);

protected:
    void integrate(const PhxReal& duration);

protected:
    RigidBodies        m_rigid_bodies;
    PhxRbForceRegistry m_force_registry;
};

} // namespace sputnik::physics

#endif // !PHX_WORLD_H
