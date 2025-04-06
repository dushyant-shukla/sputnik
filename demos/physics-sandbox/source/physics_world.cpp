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
    // Integrate rigid bodies
    for(auto itr = m_rigid_bodies.begin(); itr != m_rigid_bodies.end(); ++itr)
    {
        phx::rb::PhxRigidBody* rb            = *itr;
        PhxVec3                acceleration  = rb->getAcceleration();
        PhxVec3                total_impluse = rb->getMass() * acceleration * duration;
        rb->applyLinearImpulse(total_impluse, duration);
    }

    // Detect collisions and resolve contacts
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
                        APPLICATION_INFO("Sphere-Sphere collision detected");
                        // sphere_geom1->m_rigid_body->setVelocity({0.0f, 0.0f, 0.0f});
                        // sphere_geom2->m_rigid_body->setVelocity({0.0f, 0.0f, 0.0f});

                        resolveContact(contact, duration);
                    }
                }
            }
        }
    }

    // Update positions
    for(auto itr = m_rigid_bodies.begin(); itr != m_rigid_bodies.end(); ++itr)
    {
        phx::rb::PhxRigidBody* rb = *itr;
        // PhxVec3                position = rb->getWorldPosition();
        // position += rb->getLinerVelocity() * duration;
        // rb->setPosition(position);
        // rb->calculateDerivedData();
        rb->update(duration);
    }

    // Update geometries
    for(auto itr = m_geometries.begin(); itr != m_geometries.end(); ++itr)
    {
        (*itr)->updateGeometry();
    }
}

// void PhysicsWorld::resolveContact(const PhxContact& contact, const PhxReal& dt)
//{
//     PhxRigidBody* body_a = contact.body_a;
//     PhxRigidBody* body_b = contact.body_b;
//
//     // body_a->setVelocity({0.0f, 0.0f, 0.0f});
//     // body_b->setVelocity({0.0f, 0.0f, 0.0f});
//
//     const PhxVec3 point_on_a = contact.point_on_a_world;
//     const PhxVec3 point_on_b = contact.point_on_b_world;
//
//     const PhxReal inv_mass_a = body_a->getInverseMass();
//     const PhxReal inv_mass_b = body_b->getInverseMass();
//     const PhxReal elasticity = body_a->getElasticity() * body_b->getElasticity();
//
//     const PhxMat3 inv_inertia_tensor_a = body_a->getInverseInertiaTensorInWorldSpace();
//     const PhxMat3 inv_inertia_tensor_b = body_b->getInverseInertiaTensorInWorldSpace();
//
//     // Apply collision impulse
//     const PhxVec3& contact_normal = contact.normal_world;
//     const PhxVec3  velocity_ab    = body_b->getLinerVelocity() - body_a->getLinerVelocity();
//     const PhxReal  impluse = (1.0f + elasticity) * phx_dot(velocity_ab, contact_normal) / (inv_mass_a + inv_mass_b);
//     const PhxVec3  impluse_vector = contact_normal * impluse;
//     body_a->applyLinearImpulse(impluse_vector, dt);
//     body_b->applyLinearImpulse(-impluse_vector, dt);
//
//     // Adjust bodies so that they are no longer penetrating using projection
//     const PhxReal ta = body_a->getInverseMass() / (body_a->getInverseMass() + body_b->getInverseMass());
//     const PhxReal tb = body_b->getInverseMass() / (body_a->getInverseMass() + body_b->getInverseMass());
//     const PhxVec3 initial_sep_distance       = contact.point_on_b_world - contact.point_on_a_world;
//     PhxVec3       position_with_correction_a = body_a->getWorldPosition() + ta * initial_sep_distance;
//     PhxVec3       position_with_correction_b = body_b->getWorldPosition() - tb * initial_sep_distance;
//     body_a->setPosition(position_with_correction_a);
//     body_b->setPosition(position_with_correction_b);
//
//     body_a->calculateDerivedData();
//     body_b->calculateDerivedData();
// }

// void PhysicsWorld::resolveContact(const PhxContact& contact, const PhxReal& dt)
//{
//     PhxRigidBody* body_a = contact.body_a;
//     PhxRigidBody* body_b = contact.body_b;
//
//     // body_a->setVelocity({0.0f, 0.0f, 0.0f});
//     // body_b->setVelocity({0.0f, 0.0f, 0.0f});
//
//     const PhxVec3 point_on_a = contact.point_on_a_world;
//     const PhxVec3 point_on_b = contact.point_on_b_world;
//
//     const PhxReal inv_mass_a = body_a->getInverseMass();
//     const PhxReal inv_mass_b = body_b->getInverseMass();
//     const PhxReal elasticity = body_a->getElasticity() * body_b->getElasticity();
//
//     const PhxMat3 inv_inertia_tensor_a = body_a->getInverseInertiaTensorInWorldSpace();
//     const PhxMat3 inv_inertia_tensor_b = body_b->getInverseInertiaTensorInWorldSpace();
//
//     const PhxVec3 ra = point_on_a - body_a->getCenterOfMassInWorldSpace();
//     const PhxVec3 rb = point_on_b - body_b->getCenterOfMassInWorldSpace();
//
//     const PhxVec3 angular_ja     = phx_cross(inv_inertia_tensor_a * phx_cross(ra, contact.normal_world), ra);
//     const PhxVec3 angular_jb     = phx_cross(inv_inertia_tensor_b * phx_cross(rb, contact.normal_world), rb);
//     const PhxReal angular_factor = phx_dot(contact.normal_world, angular_ja + angular_jb);
//
//     const PhxVec3 velocity_a = body_a->getLinerVelocity() + phx_cross(body_a->getAngularVelocity(), ra);
//     const PhxVec3 velocity_b = body_b->getLinerVelocity() + phx_cross(body_b->getAngularVelocity(), rb);
//
//     // Apply collision impulse
//     const PhxVec3& contact_normal = contact.normal_world;
//     const PhxVec3  velocity_ab    = velocity_b - velocity_a;
//
//     PhxReal contact_vel = phx_dot(velocity_ab, contact.normal_world);
//     if(contact_vel > 0.0f)
//     {
//         return;
//     }
//
//     const PhxReal impluse =
//         (1.0f + elasticity) * phx_dot(velocity_ab, contact_normal) / (inv_mass_a + inv_mass_b + angular_factor);
//     const PhxVec3 impluse_vector = contact_normal * impluse;
//     body_a->applyImpulse(impluse_vector, point_on_a, dt);
//     body_b->applyImpulse(-impluse_vector, point_on_b, dt);
//
//     // Adjust bodies so that they are no longer penetrating using projection
//     const PhxReal ta = body_a->getInverseMass() / (body_a->getInverseMass() + body_b->getInverseMass());
//     const PhxReal tb = body_b->getInverseMass() / (body_a->getInverseMass() + body_b->getInverseMass());
//     const PhxVec3 initial_sep_distance       = contact.point_on_b_world - contact.point_on_a_world;
//     PhxVec3       position_with_correction_a = body_a->getWorldPosition() + ta * initial_sep_distance;
//     PhxVec3       position_with_correction_b = body_b->getWorldPosition() - tb * initial_sep_distance;
//     body_a->setPosition(position_with_correction_a);
//     body_b->setPosition(position_with_correction_b);
//
//     body_a->calculateDerivedData();
//     body_b->calculateDerivedData();
// }

void PhysicsWorld::resolveContact(const PhxContact& contact, const PhxReal& dt)
{
    PhxRigidBody* body_a = contact.body_a;
    PhxRigidBody* body_b = contact.body_b;

    // body_a->setVelocity({0.0f, 0.0f, 0.0f});
    // body_b->setVelocity({0.0f, 0.0f, 0.0f});

    const PhxVec3 point_on_a = contact.point_on_a_world;
    const PhxVec3 point_on_b = contact.point_on_b_world;

    const PhxReal inv_mass_a = body_a->getInverseMass();
    const PhxReal inv_mass_b = body_b->getInverseMass();
    const PhxReal elasticity = body_a->getElasticity() * body_b->getElasticity();

    const PhxMat3 inv_inertia_tensor_a = body_a->getInverseInertiaTensorInWorldSpace();
    const PhxMat3 inv_inertia_tensor_b = body_b->getInverseInertiaTensorInWorldSpace();

    const PhxVec3 ra = point_on_a - body_a->getCenterOfMassInWorldSpace();
    const PhxVec3 rb = point_on_b - body_b->getCenterOfMassInWorldSpace();

    const PhxVec3 angular_ja     = phx_cross(inv_inertia_tensor_a * phx_cross(ra, contact.normal_world), ra);
    const PhxVec3 angular_jb     = phx_cross(inv_inertia_tensor_b * phx_cross(rb, contact.normal_world), rb);
    const PhxReal angular_factor = phx_dot(contact.normal_world, angular_ja + angular_jb);

    const PhxVec3 velocity_a = body_a->getLinerVelocity() + phx_cross(body_a->getAngularVelocity(), ra);
    const PhxVec3 velocity_b = body_b->getLinerVelocity() + phx_cross(body_b->getAngularVelocity(), rb);

    // Apply collision impulse
    const PhxVec3& contact_normal = contact.normal_world;
    const PhxVec3  velocity_ab    = velocity_a - velocity_b;

    const PhxReal impluse =
        (1.0f + elasticity) * phx_dot(velocity_ab, contact_normal) / (inv_mass_a + inv_mass_b + angular_factor);
    const PhxVec3 impluse_vector = contact_normal * impluse;
    body_a->applyImpulse(-impluse_vector, point_on_a, dt);
    body_b->applyImpulse(impluse_vector, point_on_b, dt);

    // Apply impulse caused by friction (kinetic friction)
    const PhxReal friction_a = body_a->getFriction();
    const PhxReal friction_b = body_b->getFriction();
    const PhxReal friction   = friction_a * friction_b;

    // Find the normal direction of velocity with respect to the normal of the collision
    const PhxVec3 vel_normal = phx_dot(velocity_ab, contact_normal) * contact_normal;

    // Find the tangential direction of velocity with respect to the normal of the collision
    const PhxVec3 vel_tangent = velocity_ab - vel_normal;
    if(!CMP_FLOAT_EQ(phx_magnitude(vel_tangent), 0.0f))
    {
        PhxVec3 rel_vel_tangent = phx_normalize(vel_tangent);

        const PhxVec3 inertia_a   = phx_cross(inv_inertia_tensor_a * phx_cross(ra, rel_vel_tangent), ra);
        const PhxVec3 inertia_b   = phx_cross(inv_inertia_tensor_b * phx_cross(rb, rel_vel_tangent), rb);
        const PhxReal inv_inertia = phx_dot(rel_vel_tangent, inertia_a + inertia_b);

        // Calculate the tangential impluse for friction
        const PhxReal reduced_mass     = 1.0f / (inv_mass_a + inv_mass_b + inv_inertia);
        const PhxVec3 impulse_friction = vel_tangent * reduced_mass * friction;

        body_a->applyImpulse(-impulse_friction, point_on_a, dt);
        body_b->applyImpulse(impulse_friction, point_on_b, dt);
    }

    // Adjust bodies so that they are no longer penetrating using projection
    const PhxReal ta = body_a->getInverseMass() / (body_a->getInverseMass() + body_b->getInverseMass());
    const PhxReal tb = body_b->getInverseMass() / (body_a->getInverseMass() + body_b->getInverseMass());
    const PhxVec3 initial_sep_distance       = contact.point_on_b_world - contact.point_on_a_world;
    PhxVec3       position_with_correction_a = body_a->getWorldPosition() + ta * initial_sep_distance;
    PhxVec3       position_with_correction_b = body_b->getWorldPosition() - tb * initial_sep_distance;
    body_a->setPosition(position_with_correction_a);
    body_b->setPosition(position_with_correction_b);

    body_a->calculateDerivedData();
    body_b->calculateDerivedData();
}

} // namespace sputnik::physics
