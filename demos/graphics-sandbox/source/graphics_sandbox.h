#pragma once

#include <main/entry_point.h>
#include <main/application.h>
#include <core/layers/layer.h>
#include <core/time_step.h>
#include <graphics/glcore/gl_buffer.h>
#include <graphics/glcore/gl_vertex_array.h>
#include <graphics/glcore/gl_shader.h>
#include <graphics/glcore/gl_texture.h>
#include <graphics/api/light.h>

namespace sputnik::demos
{

using namespace sputnik::graphics::api;
using namespace sputnik::graphics::gl;
using namespace ramanujan::experimental;

class GraphicsSandboxDemoLayer : public core::Layer
{

public:
    GraphicsSandboxDemoLayer(const std::string& name);
    virtual ~GraphicsSandboxDemoLayer();

    virtual void OnAttach();
    virtual void OnDetach();
    virtual void OnUpdate(const core::TimeStep& time_step);
    virtual void OnEvent();
    virtual void OnUpdateUI(const core::TimeStep& time_step);

private:
    struct PerFrameData
    {
        alignas(16) mat4 projection;
        alignas(16) mat4 view;
        alignas(16) vec3 camera_position;
    };
    PerFrameData m_per_frame_data;
    Light        m_light;

    std::shared_ptr<OglBuffer>        m_per_frame_data_buffer;
    std::shared_ptr<OglBuffer>        m_light_data_buffer;
    std::shared_ptr<OglBuffer>        m_vertex_buffer;
    std::shared_ptr<OglVertexArray>   m_vertex_array;
    std::shared_ptr<OglShaderProgram> m_static_program;
    std::shared_ptr<OglTexture2D>     m_diff_texture;
    std::shared_ptr<OglTexture2D>     m_spec_texture;

    std::shared_ptr<Model>            m_animated_model;
    std::shared_ptr<OglTexture2D>     m_diff_texture_woman;
    std::shared_ptr<OglShaderProgram> m_shader_program;
};

class GraphicsSandboxDemo : public sputnik::main::Application
{

public:
    GraphicsSandboxDemo(const std::string& name) : sputnik::main::Application(name)
    {
        PushLayer(std::make_shared<GraphicsSandboxDemoLayer>(name));
    }

    ~GraphicsSandboxDemo() {}
};

} // namespace sputnik::demos

sputnik::main::Application* sputnik::main::CreateApplication()
{
    return new sputnik::demos::GraphicsSandboxDemo("Graphics Sandbox Demo");
}
