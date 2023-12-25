#pragma once

#include "graphics/api/model.h"
#include "core/layers/layer.h"
#include "graphics/glcore/gl_shader.h"
#include "graphics/glcore/gl_texture.h"

#include <string>

namespace sputnik::graphics::api
{

using namespace sputnik::core;

class AtmosphericScatteringLayer : public Layer
{

public:
    AtmosphericScatteringLayer();
    virtual ~AtmosphericScatteringLayer();

    virtual void OnAttach();
    virtual void OnDetach();
    virtual void OnUpdate(const sputnik::core::TimeStep& time_step);
    virtual void OnUpdateUI(const sputnik::core::TimeStep& time_step);

private:
    std::shared_ptr<Model>            m_animated_model;
    std::shared_ptr<OglTexture2D>     m_diff_texture;
    std::shared_ptr<OglShaderProgram> m_shader_program;
};

} // namespace sputnik::graphics::api
