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

    m_input_manager = InputManager::GetInstance();
    s_instance      = this;
    graphics::api::Renderer::Init(graphics::core::GraphicsSubsystemType::OPENGL);
    m_window     = graphics::api::Renderer::GetNativeWindow();
    m_editor_new = sputnik::editor::Editor::getInstance();
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

            m_editor_new->beginFrame();
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
            m_editor_new->update(time_step);
            m_editor_new->endFrame();
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

double Application::GetTime() const
{
    return glfwGetTime();
}

core::LayerStack& Application::GetApplicationLayerStack()
{
    return m_application_layer_stack;
}

Application* Application::GetInstance()
{
    return s_instance;
}

GLFWwindow* Application::GetWindow() const
{
    // return graphics::api::Renderer::GetNativeWindow();
    return m_window;
}

void Application::PushLayer(const std::shared_ptr<core::Layer>& layer)
{
    m_application_layer_stack.PushLayer(layer);
}

void Application::PushOverlay(const std::shared_ptr<core::Layer>& layer)
{
    m_application_layer_stack.PushOverLay(layer);
}

void Application::LoadModel(sputnik::graphics::api::Model* model, const std::string& path)
{
    auto m = sputnik::graphics::api::Model::LoadModel("../data/assets/Woman.gltf");
    model  = m.get();
}

} // namespace sputnik::main