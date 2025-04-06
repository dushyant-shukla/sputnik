#pragma once

#include <main/entry_point.h>
#include <main/application.h>
#include <core/layers/layer.h>
#include <core/core.h>

#include <graphics/api/model.h>
#include <graphics/api/light.h>

#include <physics/particle_force_generator.h>
#include <physics/particle.h>
#include <core/systems/physics_system.h>
#include <core/systems/render_system.h>
#include <physics/geometry.h>

#include <vector.hpp>
#include <matrix.hpp>

namespace sputnik::demos
{

using namespace sputnik::physics;
using namespace core::systems;
using namespace sputnik::graphics::api;
using namespace ramanujan::experimental;

class MassSpringCubeDemoLayer : public core::Layer
{

public:
    MassSpringCubeDemoLayer(const std::string& name);
    virtual ~MassSpringCubeDemoLayer();

    virtual void OnAttach();
    virtual void OnDetach();
    virtual void OnUpdate(const core::TimeStep& time_step);
    virtual void OnEvent();
    virtual void OnUpdateUI(const core::TimeStep& time_step);

private:
    std::shared_ptr<Model> m_sphere;

    int         m_particle_idx{0};
    std::string m_particle_str;

    std::shared_ptr<Particle>              m_central_particle;
    std::vector<std::shared_ptr<Particle>> m_particles;
    std::vector<vec4>                      m_particle_positions;
    std::vector<Sphere>                    m_particle_shapes;
    i32 hit_particle_idx{-1};
    i32 gizmo_particle_idx{-1};

    // Constraints (contact generators)
    PhysicsSystem* m_physics_system;
    RenderSystem*  m_render_system;
    // GroundContactGenerator m_ground_contact_generator;

    // Stable configuration
    vec3 const kCubeGridSize{1.0f};                   // 10 units x 10 units x 10 units
    vec3 const kCubeGridResolution{5.0f, 5.0f, 5.0f}; // 20 x 20 x 20 particles
    real const kParticleMass{1.0f};
    real const kParticleDamping{0.05f};
    real const kParticleRadius{0.05f};
    real const kSpringStiffness{2.0f};

    Sphere m_sphere_geometry;
};

unsigned const kParticleCount{12};

class MassSpringCubeDemo : public sputnik::main::Application
{

public:
    MassSpringCubeDemo(const std::string& name) : sputnik::main::Application(name)
    {
        PushLayer(std::make_shared<MassSpringCubeDemoLayer>(name));
    }

    ~MassSpringCubeDemo() {}
};

} // namespace sputnik::demos

sputnik::main::Application* sputnik::main::CreateApplication()
{
    return new sputnik::demos::MassSpringCubeDemo("Mass Spring Cube Demo");
}
