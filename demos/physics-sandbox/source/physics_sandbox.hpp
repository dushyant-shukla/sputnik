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
    void drawAABB(const phx::AABB& aabb, const mat4& model);

    void generate3DGrid(std::vector<glm::vec3>& out_grid_points,
                        const glm::vec3&        min,
                        const glm::vec3&        max,
                        const glm::vec3&        step);

private:
    // Rendering stuff
    std::shared_ptr<Model> m_suzanne;
    RenderSystem*          m_render_system{nullptr};

    // Physics stuff
    std::shared_ptr<phx::TriangleMesh> m_triangle_mesh;
    std::vector<float>                 m_point_masses;
    std::vector<glm::vec3>             m_point_positions;
    std::vector<glm::vec3>             m_point_velocities;
    std::vector<glm::vec3>             m_point_accelerations;
    std::vector<float>                 m_point_damping_values;
    std::vector<float>                 m_point_inverse_masses;
    std::vector<glm::vec3>             m_point_accumulated_forces;

    // Debug stuff
    bool                   m_draw_bvh{false};
    bool                   m_draw_wireframe{false};
    bool                   m_disable_mesh_rendering{false};
    bool                   m_draw_grid_points{false};
    bool                   m_draw_mesh_grid_points{false};
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