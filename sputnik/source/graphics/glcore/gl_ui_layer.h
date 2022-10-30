#pragma once

#include "core/layers/layer.h"

#include <string>

struct GLFWwindow;

namespace sputnik::glcore
{

struct SystemInformation
{
    std::string vendor;
    std::string renderer;
    std::string gl_version;
};

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
    GLFWwindow*       m_window;
    SystemInformation m_system_information;
};

} // namespace sputnik::glcore