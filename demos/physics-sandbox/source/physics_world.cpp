#include "physics_world.h"

namespace sputnik::physics
{

void PhysicsWorld::runPhysics(const PhxReal& duration)
{
    // update forces
    m_force_registry.updateForces(duration);

    // integrate bodies
    integrate(duration);
}

void PhysicsWorld::addRigidBody(PhxRigidBody* body)
{
    m_rigid_bodies.push_back(body);
}

void PhysicsWorld::addForceGenerator(PhxRigidBody* body, PhxRbForceGenerator* fgen)
{
    m_force_registry.add(body, fgen);
}

void PhysicsWorld::startFrame()
{
    for(auto itr = m_rigid_bodies.begin(); itr != m_rigid_bodies.end(); ++itr)
    {
        (*itr)->clearAccumulators();
        (*itr)->calculateDerivedData();
    }
}

void PhysicsWorld::integrate(const PhxReal& duration)
{
    for(auto itr = m_rigid_bodies.begin(); itr != m_rigid_bodies.end(); ++itr)
    {
        (*itr)->integrate(duration);
    }
}

} // namespace sputnik::physics
