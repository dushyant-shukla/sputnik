#pragma once

#include "core/core.h"
#include "core/layers/layer_stack.h"
#include "graphics/glcore/gl_ui_layer.h"
#include "core/time_step.h"

#include <stdint.h>
#include <memory>

namespace sputnik::api
{

class GraphicsContext;

class GraphicsSubsystem
{

public:
    virtual void Update(const core::TimeStep& time_step)                              = 0;
    virtual void Clear() const                                                        = 0;
    virtual void SetClearColor(float r, float g, float b, float a)                    = 0;
    virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

protected:
    virtual void PushOverlay(const std::shared_ptr<core::layer::Layer>& layer);

protected:
    std::shared_ptr<GraphicsContext>   m_context;
    std::shared_ptr<glcore::GlUiLayer> m_ui_layer;
    core::layer::LayerStack            m_ui_layer_stack;
};

} // namespace sputnik::api