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

} // namespace sputnik::core::layer