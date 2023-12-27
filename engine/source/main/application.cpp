#include "pch.h"
#include "application.h"
#include "core/time_step.h"
#include "core/layers/layer.h"
#include "core/systems/render_system.h"

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

    m_input_manager = InputManager::GetInstance();
    s_instance      = this;

    // Initialize windows, and imgui
    m_render_system = RenderSystem::getInstance();
    m_render_system->initialize(RenderSystemType::OPEN_GL);

    m_editor = sputnik::editor::Editor::getInstance();
}

Application::~Application() {}

void Application::Run()
{
    while(m_is_running)
    {
        float          time      = (float)glfwGetTime();
        core::TimeStep time_step = time - m_last_frame_time;
        m_last_frame_time        = time;

        m_render_system->preUpdate(time_step);

        if(!m_is_minimized)
        {
            // graphics::api::Renderer::Update(time_step);

            m_editor->beginFrame();
            m_render_system->update(time_step);
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
            m_editor->update(time_step);
            m_editor->endFrame();
        }

        m_editor->lateUpdate(time_step);
        m_input_manager->LateUpdate(time_step);
        m_render_system->lateUpdate(time_step);
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