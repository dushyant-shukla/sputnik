#pragma once

#include <main/entry_point.h>
#include <main/application.h>
#include <core/layers/layer.h>
#include <core/time_step.h>

#include <graphics/api/model.h>
#include <graphics/api/light.h>

#include <core/systems/physics_system.h>
#include <core/systems/render_system.h>
#include <physics/physics_export.h>

#include <vector.hpp>
#include <matrix.hpp>

namespace sputnik::demos
{

using namespace sputnik::physics;
using namespace core::systems;
using namespace sputnik::graphics::api;

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
    std::shared_ptr<Model> m_sphere;

    // Constraints (contact generators)

    std::vector<std::shared_ptr<Particle>>              m_particles;
    std::vector<vec4>                                   m_particle_positions;
    std::vector<Sphere>                                 m_particle_shapes;
    std::vector<std::shared_ptr<ParticleCable>>         m_cables;
    std::vector<std::shared_ptr<AnchoredParticleCable>> m_anchored_cables;
    std::shared_ptr<AnchoredParticleCable>              m_cable_start;
    std::shared_ptr<AnchoredParticleCable>              m_cable_end;

    PhysicsSystem* m_physics_system;
    RenderSystem*  m_render_system;
    // GroundContactGenerator m_ground_contact_generator;

    // Stable configuration
    vec3 const kRopeGridSize{10.0f, 0.0f, 0.0f}; // 10 units x 10 units
    // vec3       kRopeGridResolution{51.0f, 1.0f, 1.0f}; // 20 x 20 particles
    vec3       kRopeGridResolution{25.0f, 1.0f, 1.0f}; // 20 x 20 particles
    real const kParticleMass{10.0f};
    // real const kParticleDamping{0.05f};
    real const kParticleDamping{0.00001f};
    real const kParticleRadius{0.05f};
    // real const kSpringStiffness{2.0f};
    real const kSpringStiffness{2500.0f};
    vec3 const kGravity{0.0f, -9.8f, 0.0f};

    real const kRestitution{0.00025f};
    real const kCableLength{0.10f};

    bool m_render_particles{true};
};

unsigned const kParticleCount{12};

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
    return new sputnik::demos::MassAggregateRopeDemo("Mass Aggregate Rope Demo");
}
