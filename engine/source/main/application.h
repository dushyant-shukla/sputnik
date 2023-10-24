#pragma once

#include "core/layers/layer_stack.h"
#include "core/input/input_manager.h"
#include "editor/editor.h"

#include <memory>

namespace sputnik::main
{
class Application
{

public:
    Application(const std::string& application_name = "Sputnik");
    virtual ~Application();

    void Run();
    void Shutdown();

    core::LayerStack& GetApplicationLayerStack();

    static Application* GetInstance();

public:
    virtual void PushLayer(const std::shared_ptr<core::Layer>& layer);
    virtual void PushOverlay(const std::shared_ptr<core::Layer>& layer);

private:
    std::string                              m_application_name;
    bool                                     m_is_running;
    bool                                     m_is_minimized;
    float                                    m_last_frame_time;
    core::LayerStack                         m_application_layer_stack;
    core::InputManager*                      m_input_manager;
    static Application*                      s_instance;
    std::unique_ptr<sputnik::editor::Editor> m_editor;
};

Application* CreateApplication(); // Must be defined by classes inheriting from this class
} // namespace sputnik::main
