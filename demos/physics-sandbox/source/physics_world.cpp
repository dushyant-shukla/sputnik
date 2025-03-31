#include "physics_world.h"

#include "phx/phx_math_utils.hpp"

#include "core/core.h"

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

void PhysicsWorld::addGeometry(PhxGeometry* geometry)
{
    m_geometries.push_back(geometry);
}

void PhysicsWorld::addForceGenerator(PhxRigidBody* body, PhxRbForceGenerator* fgen)
{
    m_force_registry.add(body, fgen);
}

RigidBodies::const_iterator PhysicsWorld::rigidBodiesBegin() const
{
    return m_rigid_bodies.begin();
}

RigidBodies::const_iterator PhysicsWorld::rigidBodiesEnd() const
{
    return m_rigid_bodies.end();
}

Geometries::const_iterator PhysicsWorld::geometriesBegin() const
{
    return m_geometries.begin();
}
Geometries::const_iterator PhysicsWorld::geometriesEnd() const
{
    return m_geometries.end();
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
        phx::rb::PhxRigidBody* rb            = *itr;
        PhxVec3                acceleration  = rb->getAcceleration();
        PhxVec3                total_impluse = rb->getMass() * acceleration * duration;
        rb->applyLinearImpluse(total_impluse);
    }

    for(auto itr1 = m_geometries.begin(); itr1 != m_geometries.end(); ++itr1)
    {
        for(auto itr2 = itr1 + 1; itr2 != m_geometries.end(); ++itr2)
        {
            PhxGeometry* geometry1 = *itr1;
            PhxGeometry* geometry2 = *itr2;

            // Skip intersections between two static bodies (bodies with infinite mass)
            if(CMP_FLOAT_EQ(geometry1->m_rigid_body->getMass(), 0.0f) &&
               CMP_FLOAT_EQ(geometry2->m_rigid_body->getMass(), 0.0f))
            {
                continue;
            }

            if(geometry1->getType() == phx::rb::PhxGeometryType::Sphere &&
               geometry2->getType() == phx::rb::PhxGeometryType::Sphere)
            {
                auto sphere_geom1 = dynamic_cast<phx::rb::PhxSphereGeometry*>(geometry1);
                auto sphere_geom2 = dynamic_cast<phx::rb::PhxSphereGeometry*>(geometry2);
                if(sphere_geom1 && sphere_geom2)
                {
                    PhxContact contact;
                    if(phx::rb::phxIntersect(sphere_geom1, sphere_geom2, contact))
                    {
                        // APPLICATION_INFO("Sphere-Sphere collision detected");
                        // sphere_geom1->m_rigid_body->setVelocity({0.0f, 0.0f, 0.0f});
                        // sphere_geom2->m_rigid_body->setVelocity({0.0f, 0.0f, 0.0f});

                        resolveContact(contact);
                    }
                }
            }
        }
    }

    for(auto itr = m_rigid_bodies.begin(); itr != m_rigid_bodies.end(); ++itr)
    {
        phx::rb::PhxRigidBody* rb       = *itr;
        PhxVec3                position = rb->getWorldPosition();
        position += rb->getLinerVelocity() * duration;
        rb->setPosition(position);

        rb->calculateDerivedData();
    }

    for(auto itr = m_geometries.begin(); itr != m_geometries.end(); ++itr)
    {
        (*itr)->updateGeometry();
    }
}

void PhysicsWorld::resolveContact(const PhxContact& contact)
{
    PhxRigidBody* body_a = contact.body_a;
    PhxRigidBody* body_b = contact.body_b;

    // body_a->setVelocity({0.0f, 0.0f, 0.0f});
    // body_b->setVelocity({0.0f, 0.0f, 0.0f});

    const PhxReal inv_mass_a = body_a->getInverseMass();
    const PhxReal inv_mass_b = body_b->getInverseMass();
    const PhxReal elasticity = body_a->getElasticity() * body_b->getElasticity();

    // Apply collision impulse
    const PhxVec3& contact_normal = contact.normal_world;
    const PhxVec3  velocity_ab    = body_b->getLinerVelocity() - body_a->getLinerVelocity();
    const PhxReal  impluse = (1.0f + elasticity) * phx_dot(velocity_ab, contact_normal) / (inv_mass_a + inv_mass_b);
    const PhxVec3  impluse_vector = contact_normal * impluse;
    body_a->applyLinearImpluse(impluse_vector);
    body_b->applyLinearImpluse(-impluse_vector);

    // Adjust bodies so that they are no longer penetrating using projection
    const PhxReal ta = body_a->getInverseMass() / (body_a->getInverseMass() + body_b->getInverseMass());
    const PhxReal tb = body_b->getInverseMass() / (body_a->getInverseMass() + body_b->getInverseMass());
    const PhxVec3 initial_sep_distance       = contact.point_on_b_world - contact.point_on_a_world;
    PhxVec3       position_with_correction_a = body_a->getWorldPosition() + ta * initial_sep_distance;
    PhxVec3       position_with_correction_b = body_b->getWorldPosition() - tb * initial_sep_distance;
    body_a->setPosition(position_with_correction_a);
    body_a->calculateDerivedData();
    body_b->setPosition(position_with_correction_b);
    body_b->calculateDerivedData();
}

} // namespace sputnik::physics
