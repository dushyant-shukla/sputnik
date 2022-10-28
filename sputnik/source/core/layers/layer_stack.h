#pragma once

#include "pch.h"

namespace sputnik::core::layer
{

class Layer;

class LayerStack
{

public:
    LayerStack();
    ~LayerStack();

    void PushLayer(const std::shared_ptr<Layer>& layer);
    void PushOverLay(const std::shared_ptr<Layer>& layer);
    void PopLayer(const std::shared_ptr<Layer>& layer);
    void PopOverlay(const std::shared_ptr<Layer>& layer);

    std::vector<std::shared_ptr<Layer>>::iterator begin();
    std::vector<std::shared_ptr<Layer>>::iterator end();

private:
    std::vector<std::shared_ptr<Layer>> m_layers;
    size_t                              m_current_layer_insert_index;
};

} // namespace sputnik::core::layer