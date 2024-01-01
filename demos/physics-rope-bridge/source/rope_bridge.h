#pragma once

#include <main/entry_point.h>
#include <main/application.h>
#include <core/layers/layer.h>
#include <core/time_step.h>

// #include <graphics/glcore/shader.h>
#include <graphics/api/model.h>
#include <graphics/api/light.h>
// #include <graphics/glcore/texture.h>
// #include <graphics/glcore/debug_draw.h>

#include <physics/particle_force_registry.h>
#include <physics/particle_force_generator.h>
#include <physics/particle.h>
#include <physics/particle_world.h>
#include <physics/particle_constraints.h>
#include <core/systems/physics_system.h>

#include <vector.hpp>
#include <matrix.hpp>

#include <memory>

namespace sputnik::demos
{

using namespace sputnik::physics;
// using namespace sputnik::graphics::glcore;
using namespace sputnik::graphics::api;

class RopeBridgeDemoLayer : public core::Layer
{

public:
    RopeBridgeDemoLayer(const std::string& name);
    virtual ~RopeBridgeDemoLayer();

    virtual void OnAttach();
    virtual void OnDetach();
    virtual void OnUpdate(const core::TimeStep& time_step);
    virtual void OnEvent();
    virtual void OnUpdateUI(const core::TimeStep& time_step);

    void UpdateAdditionalMass();

private:
    std::shared_ptr<Model> m_sphere;

    core::systems::PhysicsSystem*          m_physics_system;
    std::vector<std::shared_ptr<Particle>> m_particles;
    std::shared_ptr<Particle>              m_falling_particle;

    // Constraints (contact generators)
    std::vector<std::shared_ptr<AnchoredParticleCable>> m_anchored_cables;
    std::vector<std::shared_ptr<ParticleCable>>         m_cables;
    std::vector<std::shared_ptr<ParticleRod>>           m_rods;
    std::shared_ptr<GroundContactGenerator>             m_ground_contact_generator;

    vec3 m_mass_position;
    vec3 m_mass_display_position;

    real const     kBaseMass{1};
    real const     kExtraMass{10};
    unsigned const kRodCount{6};
    unsigned const kCableCount{10};
    unsigned const kAnchoredCableCount{12};
};

unsigned const kParticleCount{12};

class RopeBridgeDemo : public sputnik::main::Application
{

public:
    RopeBridgeDemo(const std::string& name) : sputnik::main::Application(name)
    {
        PushLayer(std::make_shared<RopeBridgeDemoLayer>(name));
    }

    ~RopeBridgeDemo() {}
};

} // namespace sputnik::demos

sputnik::main::Application* sputnik::main::CreateApplication()
{
    return new sputnik::demos::RopeBridgeDemo("Sandbox Demo");
}
