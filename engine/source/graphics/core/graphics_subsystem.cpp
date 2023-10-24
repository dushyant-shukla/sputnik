#include "pch.h"
#include "graphics_subsystem.h"

namespace sputnik::graphics::core
{

GraphicsSubsystem::GraphicsSubsystem() {}

GraphicsSubsystem::~GraphicsSubsystem() {}

void GraphicsSubsystem::PushOverlay(const std::shared_ptr<sputnik::core::Layer>& layer)
{
    m_ui_layer_stack.PushOverLay(layer);
}
} // namespace sputnik::graphics::core