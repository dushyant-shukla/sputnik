#pragma once

#include <main/application.h>
#include <graphics/glcore/shader.h>
#include <graphics/glcore/debug_draw.h>
#include <graphics/glcore/texture.h>
#include <main/entry_point.h>
#include <core/layers/layer.h>
#include <core/time_step.h>
#include <graphics/api/PreethamSkyModel.h>
#include <graphics/api/model.h>

#include <physics/particle.h>

#include <vector.hpp>

#include <memory>
#include <vector>

namespace sputnik::demos
{

using namespace sputnik::graphics::api;

class BasicParticlesDemoLayer : public core::Layer
{
public:
    BasicParticlesDemoLayer(const std::string& name);
    virtual ~BasicParticlesDemoLayer();

    virtual void OnAttach();
    virtual void OnDetach();
    virtual void OnUpdate(const core::TimeStep& time_step);
    virtual void OnEvent();
    virtual void OnUpdateUI(const core::TimeStep& time_step);

private:
    struct Material
    {
        vec3  ambient{1.0f, 0.5f, 0.31f};
        vec3  diffuse{1.0f, 0.5f, 0.31f};
        vec3  specular{0.5f, 0.5f, 0.5f};
        float shininess{32.0f};
    };

    struct Light
    {
        vec3  position{0.0f, 20.0f, 0.0f};
        vec3  ambient{0.2f, 0.2f, 0.2f};
        vec3  diffuse{0.5f, 0.5f, 0.5f};
        vec3  specular{1.0f, 1.0f, 1.0f};
        float constant{1.0f};
        float linear{0.09f};
        float quadratic{0.032f};
    };

    std::shared_ptr<sputnik::graphics::glcore::Texture> m_static_mesh_texture;

    std::shared_ptr<sputnik::graphics::glcore::Shader> m_static_shader;
    std::shared_ptr<sputnik::graphics::glcore::Shader> m_sky_shader;
    std::shared_ptr<sputnik::graphics::glcore::Shader> m_simple_lighting_shader;

    std::shared_ptr<Model> m_box;
    std::shared_ptr<Model> m_sphere;

    Material m_platform_material;
    Material m_particle_material;

    std::vector<sputnik::physics::Particle> m_particles;

    Light m_light;

    float            m_exposure  = 1.0f;
    float            m_sun_angle = -1.45f; // radians (~ (-83) degress)
    vec3             mDirection  = vec3(0.0f, 0.0f, 1.0f);
    PreethamSkyModel mPreethamSkyModel;
};

class BasicParticlesDemo : public sputnik::main::Application
{

public:
    BasicParticlesDemo(const std::string& name) : sputnik::main::Application::Application(name)
    {
        PushLayer(std::make_shared<BasicParticlesDemoLayer>(name));
    }

    ~BasicParticlesDemo() {}
};

} // namespace sputnik::demos

sputnik::main::Application* sputnik::main::CreateApplication()
{
    return new sputnik::demos::BasicParticlesDemo("Basic Particles");
}

// m_platform_material.ambient    = {0.2f, 0.2f, 0.2f};
// m_platform_material.diffuse    = {0.8f, 0.8f, 0.8f};
// m_platform_material.specular   = {1.0f, 1.0f, 1.0f};
// m_platform_material.shininess  = 0.25f;
