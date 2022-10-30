#include "pch.h"
#include "renderer.h"
#include "render_command.h"

namespace sputnik::api
{

GraphicsSubsystemType Renderer::s_graphics_subsystem;

void Renderer::Init(const GraphicsSubsystemType& subsystem_type)
{
    s_graphics_subsystem = subsystem_type;
    RenderCommand::Init(subsystem_type);
}

void Renderer::Update(const core::TimeStep& time_step)
{
    RenderCommand::Update(time_step);
}

void Renderer::Clear()
{
    RenderCommand::Clear();
}

void Renderer::SetClearColor(float r, float g, float b, float a)
{
    RenderCommand::SetClearColor(r, g, b, a);
}

void Renderer::SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    RenderCommand::SetViewPort(x, y, width, height);
}

void Renderer::Shutdown() {}

void Renderer::OnWindowResize(uint32_t width, uint32_t height)
{
    RenderCommand::SetViewPort(0, 0, width, height);
}

const GraphicsSubsystemType& Renderer::GetCurrentGraphicsSubsystemType()
{
    return s_graphics_subsystem;
}

} // namespace sputnik::api