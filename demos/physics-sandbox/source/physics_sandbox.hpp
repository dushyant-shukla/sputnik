#pragma once

#include <main/entry_point.h>
#include <main/application.h>
#include <core/layers/layer.h>
#include <core/time_step.h>
#include <graphics/glcore/gl_buffer.h>
#include <graphics/glcore/gl_vertex_array.h>
#include <graphics/glcore/gl_shader.h>
#include <graphics/glcore/gl_texture.h>
#include <graphics/api/light.h>
#include <core/systems/render_system.h>

#include "phx/geometry.hpp"
#include "phx/mad/deformable_body.hpp"

namespace sputnik::demos
{

using namespace sputnik::graphics::api;
using namespace sputnik::graphics::gl;
using namespace sputnik::core::systems;

class PhysicsSandboxDemoLayer : public core::Layer
{
public:
    PhysicsSandboxDemoLayer(const std::string& name);
    virtual ~PhysicsSandboxDemoLayer();

    virtual void OnAttach();
    virtual void OnDetach();
    virtual void OnUpdate(const core::TimeStep& time_step);
    virtual void OnEvent();
    virtual void OnUpdateUI(const core::TimeStep& time_step);

private:
    void drawAABB(const phx::PhxAABB& aabb, const mat4& model);

    void setupRaycastTests();
    void simulatePhysics(const double& total_time, const double& step_size);
    void integrateExplicitEuler(const double& total_time, const double& step_size);
    void integrateSemiImplicitEuler(const double& total_time, const double& step_size);

    void resetSimulation();

    void setupTorus();
    void setupSuzanne();

private:
    // Rendering stuff
    std::shared_ptr<Model>          m_suzanne;
    std::shared_ptr<OglTexture2D>   m_cloth_diff_texture;
    std::shared_ptr<Model>          m_box;
    RenderSystem*                   m_render_system{nullptr};
    ramanujan::experimental::mat4   m_suzanne_model_mat{};
    glm::mat4                       m_suzanne_model_mat_glm{1.0f};
    std::shared_ptr<OglBuffer>      m_vertex_buffer;
    std::shared_ptr<OglBuffer>      m_index_buffer;
    std::shared_ptr<OglVertexArray> m_vertex_array;

    // Physics stuff
    std::shared_ptr<phx::mad::DeformableBody>      m_deformable_body;
    std::shared_ptr<phx::PhxTriangleMesh>          m_triangle_mesh;
    std::shared_ptr<phx::PhxTriangleMesh>          m_triangle_mesh_copy; // Temp copy for resetting
    PhxVec3Array                                   m_initial_mesh_vertices;
    std::shared_ptr<phx::mad::MassAggregateVolume> m_aggregate_mass_volume;
    std::shared_ptr<phx::mad::MassAggregateVolume> m_aggregate_mass_volume_copy; // Temp copy for resetting
    std::vector<float>                             m_point_masses;
    std::vector<glm::vec3>                         m_point_positions;
    std::vector<glm::vec3>                         m_point_velocities;
    std::vector<glm::vec3>                         m_point_accelerations;
    std::vector<float>                             m_point_damping_values;
    std::vector<float>                             m_point_inverse_masses;
    std::vector<glm::vec3>                         m_point_accumulated_forces;
    const double                                   kStepSize{0.01};
    double                                         m_accumulated_time{0.0};
    double                                         m_total_time{0.0};
    bool                                           m_simulate_physics{false};
    bool                                           m_reset_simulation{false};

    // Debug stuff
    bool                   m_draw_bvh{false};
    bool                   m_draw_wireframe{true};
    bool                   m_draw_mesh{false};
    bool                   m_draw_grid_points{false};
    bool                   m_draw_mesh_grid_points{false};
    bool                   m_render_surface_points{false};
    bool                   m_render_structural_springs{false};
    bool                   m_render_shear_springs{false};
    bool                   m_render_bend_springs{false};
    bool                   m_render_surface_springs{false};
    bool                   m_render_internal_springs{false};
    std::vector<glm::vec3> m_grid_points;

    std::vector<vec4> m_mesh_intersection_rays;
};

class PhysicsSandboxDemo : public sputnik::main::Application
{
public:
    PhysicsSandboxDemo(const std::string& name) : sputnik::main::Application(name)
    {
        PushLayer(std::make_shared<PhysicsSandboxDemoLayer>(name));
    }

    ~PhysicsSandboxDemo() {}
};

} // namespace sputnik::demos

sputnik::main::Application* sputnik::main::CreateApplication()
{
    return new sputnik::demos::PhysicsSandboxDemo("Physics Sandbox");
}