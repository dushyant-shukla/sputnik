#pragma once

#include <main/entry_point.h>
#include <main/application.h>
#include <core/layers/layer.h>
#include <core/time_step.h>
#include <graphics/glcore/v2/gl_buffer.h>
#include <graphics/glcore/v2/gl_vertex_array.h>
#include <graphics/glcore/v2/gl_shader.h>

namespace sputnik::demos
{

using namespace sputnik::graphics::gl;

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
    std::shared_ptr<OglBuffer>        m_vertex_buffer;
    std::shared_ptr<OglVertexArray>   m_vertex_array;
    std::shared_ptr<OglShaderProgram> m_static_program;
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
