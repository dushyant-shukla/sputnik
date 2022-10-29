#include "pch.h"
#include "application.h"
#include "core/time_step.h"
#include "core/layers/layer.h"
#include "graphics/api/renderer.h"
#include "graphics/api/graphics_subsystem_type.h"

#include <GLFW/glfw3.h>

namespace sputnik::main
{

Application* Application::s_instance = nullptr;

Application::Application(const std::string& application_name)
    : m_application_name(application_name)
    , m_is_running(true)
    , m_is_minimized(false)
    , m_last_frame_time(0.0f)
{
    s_instance = this;

    api::Renderer::Init(api::GraphicsSubsystemType::OPENGL);
}

Application ::~Application() {}

void Application::Run()
{
    while(m_is_running)
    {
        float          time      = (float)glfwGetTime();
        core::TimeStep time_step = time - m_last_frame_time;
        m_last_frame_time        = time;

        api::Renderer::SetClearColor(0.16f, 0.16f, 0.16f, 1.00f);
        api::Renderer::Clear();

        if(!m_is_minimized)
        {
            for(std::shared_ptr<core::layer::Layer> layer : m_application_layer_stack)
            {
                layer->OnUpdate(time_step);
            }
        }

        api::Renderer::Update();
    }
}

// It is being called from Window.cpp as a glfwSetWindowCloseCallback
void Application::Shutdown()
{
    m_is_running = false;
}

Application* Application::GetInstance()
{
    return s_instance;
}

void Application::PushLayer(const std::shared_ptr<core::layer::Layer>& layer)
{
    m_application_layer_stack.PushLayer(layer);
}

void Application::PushOverlay(const std::shared_ptr<core::layer::Layer>& layer)
{
    m_application_layer_stack.PushOverLay(layer);

    // Renderer push overlay
}

} // namespace sputnik::main