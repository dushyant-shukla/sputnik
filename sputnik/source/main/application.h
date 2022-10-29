#pragma once

#include "core/layers/layer_stack.h"

namespace sputnik::main
{
class Application
{

public:
    Application(const std::string& application_name = "Sputnik");
    virtual ~Application();

    void Run();
    void Shutdown();

    static Application* GetInstance();

public:
    virtual void PushLayer(const std::shared_ptr<core::layer::Layer>& layer);
    virtual void PushOverlay(const std::shared_ptr<core::layer::Layer>& layer);

    // inline Application(){};
    // inline virtual ~Application() {}
    // inline virtual void Initialize() {}
    // inline virtual void Update(float delta_time) {}
    // inline virtual void Render(float aspect_ratio) {}
    // inline virtual void RenderUI(float aspect_ratio) {}
    // inline virtual void Shutdown() {}

private:
    std::string             m_application_name;
    bool                    m_is_running;
    bool                    m_is_minimized;
    float                   m_last_frame_time;
    core::layer::LayerStack m_application_layer_stack;
    static Application*     s_instance;
};

Application* CreateApplication(); // Must be defined by classes inheriting from this class
} // namespace sputnik::main
