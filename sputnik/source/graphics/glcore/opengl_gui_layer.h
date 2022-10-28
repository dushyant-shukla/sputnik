#pragma once

#include "core/layers/layer.h"

#include <string>

namespace sputnik::opengl
{

class OpenGlGuiLayer: public core::layer::Layer
{

public:
    OpenGlGuiLayer(const std::string& name);
    virtual ~OpenGlGuiLayer();

    virtual void Begin();
    virtual void End();
};

} // namespace sputnik::opengl