#pragma once

#include <main/application.h>
#include <graphics/glcore/vertex_attribute.h>
#include <graphics/glcore/shader.h>
#include <graphics/core/animation/animation_clip.h>
#include <graphics/core/animation/pose.h>
#include <graphics/glcore/debug_draw.h>
#include <graphics/core/animation/skeleton.h>
#include <graphics/glcore/texture.h>
#include <graphics/core/animation/skinning_type.h>
#include <graphics/core/geometry/mesh.h>
#include <main/entry_point.h>
#include <core/layers/layer.h>
#include <core/time_step.h>

#include <vector.hpp>

#include <memory>
#include <vector>

namespace sputnik::demos
{

using namespace ramanujan;
using namespace ramanujan::experimental;
using namespace sputnik::graphics::glcore;

class ComputeShaderParticlesDemo : public core::Layer
{
public:
    ComputeShaderParticlesDemo(const std::string& name);
    virtual ~ComputeShaderParticlesDemo();

    virtual void OnAttach();
    virtual void OnDetach();
    virtual void OnUpdate(const core::TimeStep& time_step);
    virtual void OnEvent();
    virtual void OnUpdateUI(const core::TimeStep& time_step);

private:
    std::shared_ptr<sputnik::graphics::glcore::Shader>   m_particle_compute_shader;
    std::shared_ptr<sputnik::graphics::glcore::Shader>   m_particle_draw_shader;
    std::shared_ptr<VertexAttribute<ramanujan::Vector4>> m_particle_positions;

    unsigned int m_texture_id;

    ramanujan::Vector4 m_particle_color{0.0f, 1.0f, 1.0f, 1.0f};
    ramanujan::Vector4 m_attractor_color{1.0f, 0.0f, 0.0f, 1.0f};
    vec3               m_attactor1_position{5.0f, 0.0f, 0.0f};
    vec3               m_attactor2_position{-5.0f, 0.0f, 0.0f};

    unsigned int ssbos[2];
    unsigned int attractor_vbo;
    float        speed = 35.0f;
    float        angle = 0.0f;

    const unsigned int PARTICLE_COUNT_X     = 100;
    const unsigned int PARTICLE_COUNT_Y     = 100;
    const unsigned int PARTICLE_COUNT_Z     = 100;
    const unsigned int TOTAL_PARTICLE_COUNT = PARTICLE_COUNT_X * PARTICLE_COUNT_Y * PARTICLE_COUNT_Z;

    const unsigned int TEXTURE_WIDTH = 512, TEXTURE_HEIGHT = 512;
};

class ComputeShaderParticlesDemoApplication : public sputnik::main::Application
{

public:
    ComputeShaderParticlesDemoApplication(const std::string& name) : sputnik::main::Application::Application(name)
    {
        PushLayer(std::make_shared<ComputeShaderParticlesDemo>(name));
    }

    ~ComputeShaderParticlesDemoApplication() {}
};

} // namespace sputnik::demos

sputnik::main::Application* sputnik::main::CreateApplication()
{
    return new sputnik::demos::ComputeShaderParticlesDemoApplication("Compute Shader Demo");
}
