#pragma once

#include <main/entry_point.h>
#include <main/application.h>
#include <core/layers/layer.h>
#include <core/time_step.h>

// #include <graphics/glcore/shader.h>
#include <graphics/api/model.h>
#include <graphics/api/light.h>
#include <graphics/glcore/gl_buffer.h>
// #include <graphics/glcore/texture.h>
// #include <graphics/glcore/debug_draw.h>

#include <physics/particle_force_registry.h>
#include <physics/particle_force_generator.h>
#include <physics/particle.h>
#include <physics/particle_world.h>
#include <physics/particle_constraints.h>

#include <vector.hpp>
#include <matrix.hpp>

#include <core/systems/render_system.h>
#include <physics/mass-aggregate/mass_spring_systems.hpp>

namespace sputnik::demos
{

using namespace ramanujan::experimental;
using namespace sputnik::physics;
using namespace core::systems;
using namespace sputnik::graphics::api;
using namespace ::physics::mad;

struct VertexData
{
    alignas(16) vec3 position;
    alignas(16) vec3 normal;
    alignas(8) vec2 uv;
};

class MassAggregateRopeDemoLayer : public core::Layer
{

public:
    MassAggregateRopeDemoLayer(const std::string& name);
    virtual ~MassAggregateRopeDemoLayer();

    virtual void OnAttach();
    virtual void OnDetach();
    virtual void OnUpdate(const core::TimeStep& time_step);
    virtual void OnEvent();
    virtual void OnUpdateUI(const core::TimeStep& time_step);

private:
    std::shared_ptr<Model>              m_sphere;
    RenderSystem*                       m_render_system;
    vec3                                m_grid_size;
    MassAggregateBodySpecification      m_cube_specification;
    std::shared_ptr<MassAggregateCurve> m_mass_spring_curve;
    std::shared_ptr<OglBuffer>          m_instanced_buffer;

    std::vector<vec3> m_positions;
    std::vector<u32>  m_indices;
    std::vector<vec3> m_normals;
    std::vector<vec3> m_tangents;

    bool                 m_render_particles          = true;
    bool                 m_render_structural_springs = false;
    bool                 m_render_bend_springs       = false;
    bool                 m_render_mesh               = false;
    bool                 m_render_vertices           = false;
    SpringForceGenerator m_structural_spring;
    SpringForceGenerator m_bend_spring;

    std::vector<VertexData>    m_vertices;
    std::shared_ptr<OglBuffer> m_pvp_vertex_buffer;
    std::shared_ptr<OglTexture2D> m_rope_diff_texture;
};

class MassAggregateRopeDemo : public sputnik::main::Application
{

public:
    MassAggregateRopeDemo(const std::string& name) : sputnik::main::Application(name)
    {
        PushLayer(std::make_shared<MassAggregateRopeDemoLayer>(name));
    }

    ~MassAggregateRopeDemo() {}
};

} // namespace sputnik::demos

sputnik::main::Application* sputnik::main::CreateApplication()
{
    return new sputnik::demos::MassAggregateRopeDemo("MAD Rope");
}
