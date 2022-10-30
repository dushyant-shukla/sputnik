#include "pch.h"
#include "graphics_subsystem.h"

namespace sputnik::api
{
void GraphicsSubsystem::PushOverlay(const std::shared_ptr<core::layer::Layer>& layer)
{
    m_ui_layer_stack.PushOverLay(layer);
}
}