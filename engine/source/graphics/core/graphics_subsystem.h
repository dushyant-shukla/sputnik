#pragma once

#include "core/core.h"
#include "core/layers/layer_stack.h"
#include "graphics/glcore/gl_ui_layer.h"
#include "core/time_step.h"

#include <stdint.h>
#include <memory>

namespace sputnik::graphics::core
{

class GraphicsContext;

class GraphicsSubsystem
{

public:
    virtual void Update(const sputnik::core::TimeStep& time_step)                     = 0;
    virtual void Clear() const                                                        = 0;
    virtual void SetClearColor(float r, float g, float b, float a)                    = 0;
    virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

protected:
    virtual void PushOverlay(const std::shared_ptr<sputnik::core::Layer>& layer);

protected:
    std::shared_ptr<GraphicsContext>   m_context;
    std::shared_ptr<glcore::GlUiLayer> m_ui_layer;
    sputnik::core::LayerStack   m_ui_layer_stack;
};

} // namespace sputnik::graphics::core