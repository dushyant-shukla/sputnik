#include "pch.h"
#include "gl_graphics_subsystem.h"
#include "graphics/window/window.h"
#include "graphics/glcore/gl_context.h"
#include "gl_ui_layer.h"
#include "main/application.h"
#include "editor/editor_viewport_canvas.h"

#include <glad/glad.h>
#include <imgui.h>

namespace sputnik::graphics::glcore
{

GlGraphicsSubsystem::GlGraphicsSubsystem()
{
    m_window                = std::make_unique<window::Window>();
    m_context               = std::make_unique<GlContext>(m_window->GetNativeWindow());
    m_ui_layer              = std::make_shared<GlUiLayer>(m_window->GetNativeWindow());

    // TODO:: GLobal VAO - only temporary
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    main::Application::GetInstance()->PushOverlay(m_ui_layer);
    main::Application::GetInstance()->PushOverlay(std::make_shared<sputnik::graphics::EditorViewPortCanvas>(m_window->GetWidth(), m_window->GetHeight()));
}

GlGraphicsSubsystem::~GlGraphicsSubsystem()
{
    // TODO:: GLobal VAO - only temporary
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &m_vao);
}

void GlGraphicsSubsystem::Update(const sputnik::core::TimeStep& time_step)
{
    m_ui_layer->Begin();
    sputnik::core::LayerStack& layer_stack = main::Application::GetInstance()->GetApplicationLayerStack();
    // Todo:: layer stack to have const iterator

    for(const std::shared_ptr<sputnik::core::Layer>& layer : layer_stack)
    {
        layer->OnPreUpdateUI(time_step);
    }

    for(const std::shared_ptr<sputnik::core::Layer>& layer : layer_stack)
    {
        layer->OnUpdateUI(time_step);
    }

    for(const std::shared_ptr<sputnik::core::Layer>& layer : layer_stack)
    {
        layer->OnPostUpdateUI(time_step);
    }
    m_ui_layer->End();

    m_window->OnUpdate(time_step);
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

} // namespace sputnik::graphics::glcore