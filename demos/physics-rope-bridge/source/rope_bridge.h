#pragma once

#include <main/entry_point.h>
#include <main/application.h>
#include <core/layers/layer.h>
#include <core/time_step.h>

#include <graphics/glcore/shader.h>
#include <graphics/api/model.h>
#include <graphics/api/light.h>
#include <graphics/glcore/texture.h>
#include <graphics/glcore/debug_draw.h>

#include <physics/particle_force_registry.h>
#include <physics/particle_force_generator.h>
#include <physics/particle.h>
#include <physics/particle_world.h>
#include <physics/particle_constraints.h>

#include <vector.hpp>
#include <matrix.hpp>

#include <memory>

namespace sputnik::demos
{

using namespace sputnik::physics;
using namespace sputnik::graphics::glcore;
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
    std::shared_ptr<Shader>  m_simple_lighting_shader;
    std::shared_ptr<Shader>  m_static_shader;
    std::shared_ptr<Texture> m_static_mesh_texture;

    std::shared_ptr<sputnik::graphics::glcore::DebugDraw> m_debug_lines;

    std::shared_ptr<Model> m_box;
    std::shared_ptr<Model> m_sphere;

    Light m_light;

    int         m_particle_idx{0};
    std::string m_particle_str;

    ParticleWorld          m_particle_world;
    std::vector<Particle*> m_particles;
    GroundContactGenerator m_ground_contact_generator;

    AnchoredParticleCable* m_anchored_cables;
    ParticleCable*         m_cables;
    ParticleRod*           m_rods;

    vec3 m_mass_position;
    vec3 m_mass_display_position;

    real const             kBaseMass{1};
    real const             kExtraMass{10};
    unsigned const         kRodCount{6};
    unsigned const         kCableCount{10};
    unsigned const         kAnchoredCableCount{12};
    inline static unsigned debug_count{0};
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
