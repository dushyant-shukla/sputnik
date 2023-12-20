#include "pch.h"
#include "render_command.h"
#include "graphics_subsystem.h"
#include "graphics_subsystem_type.h"
#include "graphics/glcore/gl_graphics_subsystem.h"

namespace sputnik::graphics::core
{

std::shared_ptr<sputnik::graphics::core::GraphicsSubsystem> RenderCommand::s_graphics_subsystem;

void RenderCommand::Init(const sputnik::graphics::core::GraphicsSubsystemType& subsystem_type)
{
    SPUTNIK_ASSERT(subsystem_type != GraphicsSubsystemType::NONE && subsystem_type != GraphicsSubsystemType::VULKAN &&
                       subsystem_type != GraphicsSubsystemType::DIRECTX,
                   "Invalid graphics subsystem type.");
    s_graphics_subsystem = std::make_shared<glcore::GlGraphicsSubsystem>();
}

void RenderCommand::Update(const sputnik::core::TimeStep& time_step)
{
    s_graphics_subsystem->Update(time_step);
}

void RenderCommand::Clear()
{
    s_graphics_subsystem->Clear();
}

void RenderCommand::SetClearColor(float r, float g, float b, float a)
{
    s_graphics_subsystem->SetClearColor(r, g, b, a);
}

void RenderCommand::SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    s_graphics_subsystem->SetViewPort(x, y, width, height);
}

std::pair<uint32_t, uint32_t> RenderCommand::GetViewportDimensions()
{
    return s_graphics_subsystem->GetViewPortDimensions();
}

GLFWwindow* RenderCommand::GetNativeWindow()
{
    return s_graphics_subsystem->GetNativeWindow();
}

} // namespace sputnik::graphics::core