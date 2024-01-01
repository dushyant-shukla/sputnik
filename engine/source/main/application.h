#pragma once

#include "core/layers/layer_stack.h"
#include "api/input_manager.h"
#include "editor/editor.hpp"
#include <imgui.h>
#include "graphics/api/light.h"
#include "graphics/api/model.h"

#include <memory>

namespace sputnik::core::systems
{
class RenderSystem;
class PhysicsSystem;
} // namespace sputnik::core::systems

namespace sputnik::main
{

using namespace sputnik::engine::api;
using namespace sputnik::core::systems;
using namespace sputnik::editor;

class Application
{

public:
    Application(const std::string& application_name = "Sputnik");
    virtual ~Application();

    virtual void Run();
    void         Shutdown();

    core::LayerStack& GetApplicationLayerStack();

    static Application* GetInstance();

    std::string& GetApplicationName() { return m_application_name; }

    virtual void PushLayer(const std::shared_ptr<core::Layer>& layer);
    virtual void PushOverlay(const std::shared_ptr<core::Layer>& layer);

protected:
    std::string         m_application_name;
    bool                m_is_running;
    bool                m_is_minimized;
    float               m_last_frame_time;
    core::LayerStack    m_application_layer_stack;
    InputManager*       m_input_manager;
    Editor*             m_editor;
    RenderSystem*       m_render_system;
    PhysicsSystem*      m_physics_system;
    static Application* s_instance;
};

Application* CreateApplication(); // Must be defined by classes inheriting from this class
} // namespace sputnik::main
