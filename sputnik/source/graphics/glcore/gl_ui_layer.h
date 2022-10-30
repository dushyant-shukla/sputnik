#pragma once

#include "core/layers/layer.h"

struct GLFWwindow;

namespace sputnik::glcore
{

class GlUiLayer : public core::layer::Layer
{

public:
    GlUiLayer(GLFWwindow* window);
    ~GlUiLayer();

    virtual void OnAttach();
    virtual void OnDetach();
    virtual void OnUpdate(const core::TimeStep& time_step);
    virtual void OnEvent();
    virtual void OnUpdateUI(const core::TimeStep& time_step);

    void Begin();
    void End();

private:
    GLFWwindow* m_window;
};

} // namespace sputnik::glcore