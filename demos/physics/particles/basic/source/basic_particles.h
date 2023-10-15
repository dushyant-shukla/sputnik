#pragma once

#include <main/application.h>
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

#include <memory>
#include <vector>

namespace sputnik::demos
{

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
    std::shared_ptr<sputnik::graphics::glcore::Texture> m_static_mesh_texture;

    std::shared_ptr<sputnik::graphics::glcore::Shader> m_static_shader;

    std::vector<sputnik::graphics::core::Mesh> m_static_meshes;
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
