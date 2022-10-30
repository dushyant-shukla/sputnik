#include "pch.h"
#include "layer_stack.h"
#include "layer.h"

namespace sputnik::core
{

LayerStack::LayerStack() : m_current_layer_insert_index(0) {}

LayerStack::~LayerStack() {}

void LayerStack::PushLayer(const std::shared_ptr<Layer>& layer)
{
    m_layers.emplace(m_layers.begin() + m_current_layer_insert_index, layer);
    m_layers[m_current_layer_insert_index]->OnAttach();
    ++m_current_layer_insert_index;
}

void LayerStack::PushOverLay(const std::shared_ptr<Layer>& layer)
{
    m_layers.emplace_back(layer);
    m_layers.back()->OnAttach();
}

void LayerStack::PopLayer(const std::shared_ptr<Layer>& layer)
{
    auto it = std::find(m_layers.begin(), m_layers.end(), layer);
    if(it != m_layers.end())
    {
        m_layers.erase(it);
        --m_current_layer_insert_index;
    }
}

void LayerStack::PopOverlay(const std::shared_ptr<Layer>& layer)
{
    auto it = std::find(m_layers.begin(), m_layers.end(), layer);
    if(it != m_layers.end())
    {
        m_layers.erase(it);
    }
}

std::vector<std::shared_ptr<Layer>>::iterator LayerStack::begin()
{
    return m_layers.begin();
}

std::vector<std::shared_ptr<Layer>>::iterator LayerStack::end()
{
    return m_layers.end();
}

} // namespace sputnik::core