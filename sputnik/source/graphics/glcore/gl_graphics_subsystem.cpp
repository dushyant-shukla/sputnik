#include "pch.h"
#include "gl_graphics_subsystem.h"
#include "graphics/window/window.h"
#include "graphics/glcore/gl_context.h"
#include "gl_ui_layer.h"
#include "main/application.h"

#include <glad/glad.h>
#include <imgui.h>

namespace sputnik::glcore
{

GlGraphicsSubsystem::GlGraphicsSubsystem()
{
    m_window   = std::make_unique<Window>();
    m_context  = std::make_unique<GlContext>(m_window->GetNativeWindow());
    m_ui_layer = std::make_shared<GlUiLayer>(m_window->GetNativeWindow());
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    main::Application::GetInstance()->PushOverlay(m_ui_layer);
}

GlGraphicsSubsystem::~GlGraphicsSubsystem()
{
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &m_vao);
}

void GlGraphicsSubsystem::Update(const core::TimeStep& time_step)
{
    m_ui_layer->Begin();
    core::layer::LayerStack& layer_stack = main::Application::GetInstance()->GetApplicationLayerStack();
    for(const std::shared_ptr<core::layer::Layer>& layer : layer_stack) // Todo:: layer stack to have const iterator
    {
        layer->OnUpdateUI(time_step);
    }
    m_ui_layer->End();

    m_window->OnUpdate();
    m_context->SwapBuffers();
}

void GlGraphicsSubsystem::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void GlGraphicsSubsystem::SetClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void GlGraphicsSubsystem::SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    glViewport(x, y, width, height);
}

} // namespace sputnik::glcore