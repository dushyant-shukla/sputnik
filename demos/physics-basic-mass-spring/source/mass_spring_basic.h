#pragma once

#include <main/entry_point.h>
#include <main/application.h>
#include <core/layers/layer.h>
#include <core/time_step.h>

// #include <graphics/glcore/shader.h>
#include <graphics/api/model.h>
// #include <graphics/api/light.h>
// #include <graphics/glcore/texture.h>

#include <physics/particle_force_registry.h>
#include <physics/particle_force_generator.h>
#include <physics/particle.h>
#include <core/systems/physics_system.h>

#include <vector.hpp>
#include <matrix.hpp>

#include <memory>

namespace sputnik::demos
{

using namespace sputnik::physics;
// using namespace sputnik::graphics::glcore;
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
    std::shared_ptr<Model> m_box;
    std::shared_ptr<Model> m_sphere;

    u32                                                   kParticleCount{50};
    std::vector<std::shared_ptr<Particle>>                m_particles;
    core::systems::PhysicsSystem*                         m_physics_system;
    std::shared_ptr<ParticleAnchoredBungeeForceGenerator> m_anchored_bungee_force_generator;
    std::shared_ptr<ParticleSpringForceGenerator>         m_spring_force_generator_a;
    std::shared_ptr<ParticleSpringForceGenerator>         m_spring_force_generator_b;
    std::shared_ptr<ParticleAnchoredSpringForceGenerator> m_anchored_spring_force_generator_c;
    std::shared_ptr<ParticleAnchoredSpringForceGenerator> m_anchored_spring_force_generator;

    size_t                                 TOP_LEFT_IDX{0};
    size_t                                 TOP_RIGHT_IDX{1};
    size_t                                 BOTTOM_LEFT_IDX{2};
    size_t                                 BOTTOM_RIGHT_IDX{3};
    std::shared_ptr<Particle>              m_central_particle;
    std::vector<std::shared_ptr<Particle>> m_front_cube_particles;
    std::vector<std::shared_ptr<Particle>> m_back_cube_particles;
    std::vector<std::shared_ptr<Particle>> m_connection_pairs;
    // std::vector<std::shared_ptr<Particle>> m_cube_particles;

    int         m_particle_idx{0};
    std::string m_particle_str;

    int         m_front_particle_idx{0};
    std::string m_front_particle_str;

    int         m_back_particle_idx{0};
    std::string m_back_particle_str;
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
