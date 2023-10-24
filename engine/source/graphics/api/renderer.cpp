#include "pch.h"
#include "renderer.h"
#include "graphics/core/render_command.h"

namespace sputnik::graphics::api
{

graphics::core::GraphicsSubsystemType Renderer::s_graphics_subsystem;

void Renderer::Init(const sputnik::graphics::core::GraphicsSubsystemType& subsystem_type)
{
    s_graphics_subsystem = subsystem_type;
    sputnik::graphics::core::RenderCommand::Init(subsystem_type);
}

void Renderer::Update(const sputnik::core::TimeStep& time_step)
{
    sputnik::graphics::core::RenderCommand::Update(time_step);
}

void Renderer::Clear()
{
    core::RenderCommand::Clear();
}

void Renderer::SetClearColor(float r, float g, float b, float a)
{
    core::RenderCommand::SetClearColor(r, g, b, a);
}

void Renderer::SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    core::RenderCommand::SetViewPort(x, y, width, height);
}

void Renderer::Shutdown() {}

void Renderer::OnWindowResize(uint32_t width, uint32_t height)
{
    core::RenderCommand::SetViewPort(0, 0, width, height);
}

const graphics::core::GraphicsSubsystemType& Renderer::GetCurrentGraphicsSubsystemType()
{
    return s_graphics_subsystem;
}

std::pair<uint32_t, uint32_t> Renderer::GetViewportDimensions()
{
    return core::RenderCommand::GetViewportDimensions();
}

GLFWwindow* Renderer::GetNativeWindow()
{
    return core::RenderCommand::GetNativeWindow();
}

} // namespace sputnik::graphics::api