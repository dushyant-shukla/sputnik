#include "pch.h"
#include "application.h"
#include "core/time_step.h"
#include "core/layers/layer.h"
#include "graphics/api/renderer.h"
#include "graphics/core/graphics_subsystem_type.h"

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
    sputnik::core::Logger::Init();

    m_input_manager = core::InputManager::GetInstance();
    s_instance      = this;
    graphics::api::Renderer::Init(graphics::core::GraphicsSubsystemType::OPENGL);

    m_editor = std::make_unique<sputnik::editor::Editor>();
}

Application ::~Application() {}

void Application::Run()
{
    while(m_is_running)
    {
        float          time      = (float)glfwGetTime();
        core::TimeStep time_step = time - m_last_frame_time;
        m_last_frame_time        = time;

        graphics::api::Renderer::SetClearColor(0.16f, 0.16f, 0.16f, 1.00f);
        graphics::api::Renderer::Clear();

        if(!m_is_minimized)
        {
            graphics::api::Renderer::Update(time_step);

            m_editor->BeginFrame();
            for(const std::shared_ptr<core::Layer>& layer : m_application_layer_stack)
            {
                layer->OnPreUpdate(time_step);
            }

            for(const std::shared_ptr<core::Layer>& layer : m_application_layer_stack)
            {
                layer->OnUpdate(time_step);
            }

            for(const std::shared_ptr<core::Layer>& layer : m_application_layer_stack)
            {
                layer->OnPostUpdate(time_step);
            }
            m_editor->EndFrame();

            m_editor->Update(time_step);
        }

        m_input_manager->LateUpdate(time_step);
        graphics::api::Renderer::LateUpdate(time_step);
    }
}

// It is being called from Window.cpp as a glfwSetWindowCloseCallback
void Application::Shutdown()
{
    m_is_running = false;
}

core::LayerStack& Application::GetApplicationLayerStack()
{
    return m_application_layer_stack;
}

Application* Application::GetInstance()
{
    return s_instance;
}

void Application::PushLayer(const std::shared_ptr<core::Layer>& layer)
{
    m_application_layer_stack.PushLayer(layer);
}

void Application::PushOverlay(const std::shared_ptr<core::Layer>& layer)
{
    m_application_layer_stack.PushOverLay(layer);
}

} // namespace sputnik::main