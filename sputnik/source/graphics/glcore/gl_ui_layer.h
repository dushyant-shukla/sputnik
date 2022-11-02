#pragma once

#include "core/layers/layer.h"

#include <string>

struct GLFWwindow;

namespace sputnik::graphics::glcore
{

struct SystemInformation
{
    std::string vendor;
    std::string renderer;
    std::string gl_version;
    std::string shading_language_version;
};

class GlUiLayer : public sputnik::core::Layer
{

public:
    GlUiLayer(GLFWwindow* window);
    ~GlUiLayer();

    virtual void OnAttach();
    virtual void OnDetach();
    virtual void OnUpdate(const sputnik::core::TimeStep& time_step);
    virtual void OnEvent();
    virtual void OnUpdateUI(const sputnik::core::TimeStep& time_step);

    void Begin();
    void End();

private:
    GLFWwindow*       m_window;
    SystemInformation m_system_information;
};

} // namespace sputnik::graphics::glcore