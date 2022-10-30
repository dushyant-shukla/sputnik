#include "pch.h"
#include "layer.h"

namespace sputnik::core::layer
{

Layer::Layer(const std::string& name) : m_name(name) {}

Layer ::~Layer() {}

const std::string& Layer::GetName() const
{
    return m_name;
}

void Layer::OnAttach() {}

void Layer::OnDetach() {}

void Layer::OnUpdate(const TimeStep& time_step) {}

void Layer::OnEvent() {}

void Layer::OnUpdateUI(const core::TimeStep& time_step) {}

} // namespace sputnik::core::layer