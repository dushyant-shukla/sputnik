#include "pch.h"
#include "atmospheric_scattering.h"
#include "graphics/api/renderer.h"

namespace sputnik::graphics::api
{

AtmosphericScatteringLayer::AtmosphericScatteringLayer() : Layer("Atmospheric Scattering") {}

AtmosphericScatteringLayer::~AtmosphericScatteringLayer() {}

void AtmosphericScatteringLayer::OnAttach() {}

void AtmosphericScatteringLayer::OnDetach() {}

void AtmosphericScatteringLayer::OnUpdate(const sputnik::core::TimeStep& time_step)
{
     graphics::api::Renderer::RenderAtmoshericScattering();
}

void AtmosphericScatteringLayer::OnUpdateUI(const sputnik::core::TimeStep& time_step) {}

} // namespace sputnik::graphics::api
