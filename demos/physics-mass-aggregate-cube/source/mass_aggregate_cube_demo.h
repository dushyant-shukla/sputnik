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

using namespace sputnik::physics;
using namespace core::systems;
using namespace sputnik::graphics::api;
using namespace ::physics::mad;

class MassAggregateCubeDemoLayer : public core::Layer
{

public:
    MassAggregateCubeDemoLayer(const std::string& name);
    virtual ~MassAggregateCubeDemoLayer();

    virtual void OnAttach();
    virtual void OnDetach();
    virtual void OnUpdate(const core::TimeStep& time_step);
    virtual void OnEvent();
    virtual void OnUpdateUI(const core::TimeStep& time_step);

private:
    std::shared_ptr<Model>               m_sphere;
    RenderSystem*                        m_render_system;
    vec3                                 m_grid_size;
    MassAggregateBodySpecification       m_cube_specification;
    std::shared_ptr<MassAggregateVolume> m_mass_spring_volume;
    std::shared_ptr<OglBuffer>           m_instanced_buffer;

    bool                 m_render_particles          = true;
    bool                 m_render_structural_springs = false;
    bool                 m_render_shear_springs      = false;
    bool                 m_render_bend_springs       = false;
    SpringForceGenerator m_structural_spring;
    SpringForceGenerator m_shear_spring;
    SpringForceGenerator m_bend_spring;
};

class MassAggregateCubeDemo : public sputnik::main::Application
{

public:
    MassAggregateCubeDemo(const std::string& name) : sputnik::main::Application(name)
    {
        PushLayer(std::make_shared<MassAggregateCubeDemoLayer>(name));
    }

    ~MassAggregateCubeDemo() {}
};

} // namespace sputnik::demos

sputnik::main::Application* sputnik::main::CreateApplication()
{
    return new sputnik::demos::MassAggregateCubeDemo("MAD Cube Demo");
}
