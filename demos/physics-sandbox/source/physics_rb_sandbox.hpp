#pragma once

#include <core/layers/layer.h>
#include <core/time_step.h>
#include <graphics/glcore/gl_buffer.h>
#include <graphics/glcore/gl_vertex_array.h>
#include <graphics/glcore/gl_shader.h>
#include <graphics/glcore/gl_texture.h>
#include <graphics/api/light.h>
#include <graphics/api/model.h>
#include <core/systems/render_system.h>

#include "phx/phx_geometry.hpp"
#include "phx/rigidbody/phx_rigid_body.hpp"
#include "physics_world.h"

namespace sputnik::demos
{

using namespace sputnik::graphics::api;
using namespace sputnik::graphics::gl;
using namespace sputnik::core::systems;

class PhysicsRigidBodySandboxDemoLayer : public core::Layer
{
public:
    PhysicsRigidBodySandboxDemoLayer(const std::string& name);
    virtual ~PhysicsRigidBodySandboxDemoLayer();

    virtual void OnAttach();
    virtual void OnDetach();
    virtual void OnUpdate(const core::TimeStep& time_step);
    virtual void OnEvent();
    virtual void OnUpdateUI(const core::TimeStep& time_step);

private:
    void drawAABB(const phx::PhxAABB& aabb, const mat4& model);

    void setupRaycastTests();
    void simulatePhysics(const double& total_time, const double& step_size);

    void resetSimulation();

private:
    // Rendering stuff

    std::shared_ptr<OglBuffer>      m_vertex_buffer;
    std::shared_ptr<OglVertexArray> m_vertex_array;

    std::shared_ptr<OglTexture2D> m_texture;
    std::shared_ptr<Model>        m_box;
    RenderSystem*                 m_render_system{nullptr};

    // Physics stuff
    // std::shared_ptr<phx::PhxTriangleMesh> m_triangle_mesh;
    // PhxVec3Array                          m_initial_mesh_vertices;

    // Physics simulation stuff
    const double kStepSize{0.01};
    double       m_accumulated_time{0.0};
    double       m_total_time{0.0};
    bool         m_simulate_physics{false};
    bool         m_reset_simulation{false};

    phx::rb::PhxRigidBody m_box_rb;
    phx::rb::PhxRbGravityForceGen  m_gravity_fgen;
    sputnik::physics::PhysicsWorld m_phx_world;

    // Debug stuff
    bool                   m_draw_bvh{false};
    bool                   m_draw_wireframe{true};
    bool                   m_draw_mesh{false};
    bool                   m_draw_grid_points{false};
    bool                   m_draw_mesh_grid_points{false};
    bool                   m_render_surface_points{false};
    std::vector<glm::vec3> m_grid_points;

    std::vector<vec4> m_mesh_intersection_rays;
};

} // namespace sputnik::demos