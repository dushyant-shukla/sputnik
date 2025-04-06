#pragma once

#include <main/application.h>
#include <graphics/glcore/gl_shader.h>
#include <graphics/core/animation/animation_clip.h>
#include <graphics/core/animation/pose.h>
// #include <graphics/glcore/debug_draw.h>
#include <graphics/core/animation/skeleton.h>
#include <graphics/glcore/gl_texture.h>
#include <graphics/core/animation/skinning_type.h>
#include <graphics/core/geometry/mesh.h>
#include <main/entry_point.h>
#include <core/layers/layer.h>
#include <core/time_step.h>

#include <memory>
#include <vector>

namespace sputnik::demos
{

using namespace sputnik::graphics::gl;

class ComputeShaderDemo : public core::Layer
{
public:
    ComputeShaderDemo(const std::string& name);
    virtual ~ComputeShaderDemo();

    virtual void OnAttach();
    virtual void OnDetach();
    virtual void OnUpdate(const core::TimeStep& time_step);
    virtual void OnEvent();
    virtual void OnUpdateUI(const core::TimeStep& time_step);

private:
    // std::shared_ptr<sputnik::graphics::glcore::Shader> m_compute_shader;
    // std::shared_ptr<sputnik::graphics::glcore::Shader> m_fs_quad_shader;

    std::unique_ptr<OglVertexArray>   m_vao;
    std::shared_ptr<OglShaderProgram> m_compute_shader;
    std::shared_ptr<OglShaderProgram> m_fs_quad_shader;
    std::shared_ptr<OglTexture2D>     m_image_texture;

    unsigned int m_texture_id;

    const unsigned int TEXTURE_WIDTH = 512, TEXTURE_HEIGHT = 512;
};

class ComputeShaderDemoApplication : public sputnik::main::Application
{

public:
    ComputeShaderDemoApplication(const std::string& name) : sputnik::main::Application::Application(name)
    {
        PushLayer(std::make_shared<ComputeShaderDemo>(name));
    }

    ~ComputeShaderDemoApplication() {}
};

} // namespace sputnik::demos

sputnik::main::Application* sputnik::main::CreateApplication()
{
    return new sputnik::demos::ComputeShaderDemoApplication("Compute Shader Demo");
}
