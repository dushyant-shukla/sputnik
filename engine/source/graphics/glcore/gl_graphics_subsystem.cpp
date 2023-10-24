#include "pch.h"

#include "gl_graphics_subsystem.h"
#include "graphics/window/window.h"
#include "graphics/glcore/gl_context.h"
#include "editor/editor_viewport_canvas.h"

#include <glad/glad.h>

namespace sputnik::graphics::glcore
{

GlGraphicsSubsystem::GlGraphicsSubsystem()
{
    m_window  = std::make_unique<window::Window>();
    m_context = std::make_unique<GlContext>(m_window->GetNativeWindow());

    // TODO:: GLobal VAO - only temporary
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
}

GlGraphicsSubsystem::~GlGraphicsSubsystem()
{
    // TODO:: GLobal VAO - only temporary
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &m_vao);
}

void GlGraphicsSubsystem::Update(const sputnik::core::TimeStep& time_step)
{
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

std::pair<uint32_t, uint32_t> GlGraphicsSubsystem::GetViewPortDimensions() const
{
    SPUTNIK_ASSERT(m_window != nullptr, "Window is not available!");
    return {m_window->GetWidth(), m_window->GetHeight()};
}

GLFWwindow* GlGraphicsSubsystem::GetNativeWindow()
{
    SPUTNIK_ASSERT(m_window != nullptr, "Window is not available!");
    return m_window->GetNativeWindow();
}

} // namespace sputnik::graphics::glcore