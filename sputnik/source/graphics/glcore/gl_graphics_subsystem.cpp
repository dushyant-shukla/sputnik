#include "pch.h"
#include "gl_graphics_subsystem.h"
#include "graphics/window/window.h"
#include "graphics/glcore/gl_context.h"

#include <glad/glad.h>

namespace sputnik::glcore
{

GlGraphicsSubsystem::GlGraphicsSubsystem()
{
    m_window  = std::make_unique<Window>();
    m_context = std::make_unique<GlContext>(m_window->GetNativeWindow());
}

GlGraphicsSubsystem::~GlGraphicsSubsystem() {}

void GlGraphicsSubsystem::Update()
{
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