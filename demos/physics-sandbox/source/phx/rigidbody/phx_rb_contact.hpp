#ifndef PHX_RB_CONTACT_HPP
#define PHX_RB_CONTACT_HPP

#include "../phx_types.hpp"
#include "phx_rigid_body.hpp"

namespace phx::rb
{

struct PhxContact
{
    PhxVec3 point_on_a_world;
    PhxVec3 point_on_b_world;
    PhxVec3 point_on_a_local;
    PhxVec3 point_on_b_local;

    /*!
     * @brief Contact normal in world space. It points from body A to body B.
     */
    PhxVec3 normal_world;

    /*!
     * @brief Penetration depth of the contact. It is positive when non-penetrating, negative when penetrating.
     */
    PhxReal m_penetration_depth;

    PhxRigidBody* body_a{nullptr};
    PhxRigidBody* body_b{nullptr};
};

} // namespace phx::rb

#endif // PHX_RB_CONTACT_HPP
