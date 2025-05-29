#ifndef PHX_BROADPHASE_HPP
#define PHX_BROADPHASE_HPP

#include "phx_types.hpp"
#include "phx_math_utils.hpp"

namespace phx::rb
{
class PhxRigidBody;
class PhxGeometry;
} // namespace phx::rb

namespace phx
{

struct CollisionPair
{
    PhxInt a;
    PhxInt b;

    bool operator==(const CollisionPair& other) const;
    bool operator!=(const CollisionPair& other) const;
};

void broadPhase(const std::vector<rb::PhxGeometry*>& rigid_bodies,
                std::vector<CollisionPair>&          pairs,
                const PhxReal&                       dt);

} // namespace phx

#endif // !PHX_BROADPHASE_HPP