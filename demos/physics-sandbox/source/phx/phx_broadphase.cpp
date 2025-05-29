
#include "phx_types.hpp"
#include "phx_math_utils.hpp"
#include "phx_broadphase.hpp"
#include "rigidbody/phx_rigid_body.hpp"
#include "rigidbody/phx_rb_geometry.hpp"
#include "phx_bounds.hpp"

namespace phx
{

bool CollisionPair::operator==(const CollisionPair& other) const
{
    return (a == other.a && b == other.b) || (a == other.b && b == other.a);
}

bool CollisionPair::operator!=(const CollisionPair& other) const
{
    return !(*this == other);
}

struct PseudoBody
{
    PhxInt  id;
    PhxReal value;
    bool    is_min;
};

static PhxInt compareSweepAndPrune(const void* a, const void* b)
{
    const PseudoBody* ea = reinterpret_cast<const PseudoBody*>(a);
    const PseudoBody* eb = reinterpret_cast<const PseudoBody*>(b);

    if(ea->value < eb->value)
    {
        return -1;
    }

    return 1;
}

static void sortBodiesByBounds(const std::vector<rb::PhxGeometry*>& geometries,
                               std::vector<PseudoBody>&             sorted_bodies,
                               const PhxReal&                       dt)
{
    sorted_bodies.clear();

    // Todo:: This is not correct for all situations. For example, a rigidbody could have more than one collision
    // geometries.
    sorted_bodies.resize(geometries.size() * 2);

    PhxVec3 axis            = phx_normalize(PhxVec3(1.0f, 1.0f, 1.0f));
    PhxVec3 axis_normalized = phx_normalize(PhxVec3(1.0f, 1.0f, 1.0f));

    for(PhxInt i = 0; i < geometries.size(); ++i)
    {
        const rb::PhxGeometry*  geometry = geometries[i];
        const rb::PhxRigidBody* body     = geometry->m_rigid_body;
        PhxExtent               bounds   = geometry->getBounds(body->getWorldPosition(), body->getGlobalOrientation());

        // Expand the bounds by linear velocity
        bounds.expand(bounds.m_min + geometry->m_rigid_body->getLinerVelocity() * dt);
        bounds.expand(bounds.m_max + geometry->m_rigid_body->getLinerVelocity() * dt);

        const PhxReal epsilon = PhxReal(0.01f);
        bounds.expand(bounds.m_min - (axis * epsilon));
        bounds.expand(bounds.m_max + (axis * epsilon));

        PhxSize idx               = static_cast<PhxSize>(i) * 2 + 0;
        sorted_bodies[idx].id     = i;
        sorted_bodies[idx].value  = phx_dot(axis_normalized, bounds.m_min);
        sorted_bodies[idx].is_min = true;

        idx += 1;
        sorted_bodies[idx].id     = i;
        sorted_bodies[idx].value  = phx_dot(axis_normalized, bounds.m_max);
        sorted_bodies[idx].is_min = false;
    }

    qsort(sorted_bodies.data(), sorted_bodies.size(), sizeof(PseudoBody), compareSweepAndPrune);
}

static void buildPairs(const std::vector<PseudoBody>& sorted_bodies, std::vector<CollisionPair>& pairs)
{
    pairs.clear();
    PhxInt num_bodies = static_cast<PhxInt>(sorted_bodies.size());
    for(PhxInt i = 0; i < num_bodies; ++i)
    {
        if(!sorted_bodies[i].is_min)
        {
            continue;
        }

        for(PhxInt j = i + 1; j < num_bodies; ++j)
        {
            if(sorted_bodies[j].is_min)
            {
                continue;
            }
            if(sorted_bodies[i].id == sorted_bodies[j].id)
            {
                break;
            }
            CollisionPair pair{sorted_bodies[i].id, sorted_bodies[j].id};
            pairs.push_back(pair);
        }
    }
}

static void
sweepAndPrunt1D(const std::vector<rb::PhxGeometry*>& geometries, std::vector<CollisionPair>& pairs, const PhxReal& dt)
{
    std::vector<PseudoBody> sorted_bodies;
    sortBodiesByBounds(geometries, sorted_bodies, dt);
    buildPairs(sorted_bodies, pairs);
}

void phx::broadPhase(const std::vector<rb::PhxGeometry*>& geometries,
                     std::vector<CollisionPair>&          pairs,
                     const PhxReal&                       dt)
{
    pairs.clear();
    sweepAndPrunt1D(geometries, pairs, dt);
}

} // namespace phx
