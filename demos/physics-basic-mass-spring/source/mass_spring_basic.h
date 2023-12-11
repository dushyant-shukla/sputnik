#pragma once

#include <main/entry_point.h>
#include <main/application.h>
#include <core/layers/layer.h>
#include <core/time_step.h>

#include <graphics/glcore/shader.h>
#include <graphics/api/model.h>
#include <graphics/api/light.h>
#include <graphics/glcore/texture.h>

#include <physics/particle_force_registry.h>
#include <physics/particle_force_generator.h>
#include <physics/particle.h>

#include <vector.hpp>
#include <matrix.hpp>

#include <memory>

namespace sputnik::demos
{

using namespace sputnik::physics;
using namespace sputnik::graphics::glcore;
using namespace sputnik::graphics::api;

class MassSpringBasicDemoLayer : public core::Layer
{

public:
    MassSpringBasicDemoLayer(const std::string& name);
    virtual ~MassSpringBasicDemoLayer();

    virtual void OnAttach();
    virtual void OnDetach();
    virtual void OnUpdate(const core::TimeStep& time_step);
    virtual void OnEvent();
    virtual void OnUpdateUI(const core::TimeStep& time_step);

private:
    ParticleForceRegistry m_particle_force_registry;

    std::shared_ptr<Shader>  m_simple_lighting_shader;
    std::shared_ptr<Shader>  m_static_shader;
    std::shared_ptr<Texture> m_static_mesh_texture;

    std::shared_ptr<Model> m_box;
    std::shared_ptr<Model> m_sphere;

    std::vector<Particle*> m_particles;

    ParticleAnchoredBungeeForceGenerator* m_anchored_bungee_force_generator;
    ParticleSpringForceGenerator*         m_spring_force_generator_a;
    ParticleSpringForceGenerator*         m_spring_force_generator_b;

    Light m_light;

    int         m_particle_idx{0};
    std::string m_particle_str;
};

class MassSpringBasicDemo : public sputnik::main::Application
{

public:
    MassSpringBasicDemo(const std::string& name) : sputnik::main::Application(name)
    {
        PushLayer(std::make_shared<MassSpringBasicDemoLayer>(name));
    }

    ~MassSpringBasicDemo() {}
};

} // namespace sputnik::demos

sputnik::main::Application* sputnik::main::CreateApplication()
{
    return new sputnik::demos::MassSpringBasicDemo("Sandbox Demo");
}
