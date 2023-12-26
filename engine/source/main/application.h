#pragma once

#include "core/layers/layer_stack.h"
#include "api/input_manager.h"
#include "editor/editor.hpp"
#include <imgui.h>
#include "graphics/api/light.h"
#include "graphics/api/model.h"

#include <memory>

namespace sputnik::main
{

using namespace sputnik::engine::api;

class Application
{

public:
    Application(const std::string& application_name = "Sputnik");
    virtual ~Application();

    virtual void Run();
    void         Shutdown();

    double GetTime() const;

    core::LayerStack& GetApplicationLayerStack();

    static Application* GetInstance();

    GLFWwindow* GetWindow() const;

    std::string& GetApplicationName() { return m_application_name; }

    virtual void PushLayer(const std::shared_ptr<core::Layer>& layer);
    virtual void PushOverlay(const std::shared_ptr<core::Layer>& layer);

    void LoadModel(sputnik::graphics::api::Model* model, const std::string& path);

    virtual void LoadModel(const std::string& path){};

    virtual void DrawModel(sputnik::graphics::api::Model* model){};
    virtual void DrawModel(){};

    virtual void createShaderProgram(){};

    // template <typename ImGuiCallback>
    // void createImGuiWidgetWithCallback(ImGuiCallback func)
    //{
    //     func();
    //     ENGINE_INFO("ImGui widget created.");
    // }

    virtual void beginImguiWindow(const std::string& name) { ImGui::Begin(name.c_str()); }
    virtual void endImguiWindow() { ImGui::End(); }

    virtual void createImguiWidget(const std::string& name, sputnik::graphics::api::Light& m_light)
    {
        if(ImGui::Begin(name.c_str()))
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
            if(ImGui::Button("Light X"))
            {
            }
            ImGui::SameLine();
            ImGui::SliderFloat("##Light X", &m_light.position.x, -50.0f, 50.0f);
            ImGui::PopStyleColor(3);

            ImGui::SliderFloat("Light Y", &m_light.position.y, -50.0f, 50.0f);
            ImGui::SliderFloat("Light Z", &m_light.position.z, -50.0f, 50.0f);

            ImGui::ColorEdit3("Light ambient", &m_light.ambient.x, ImGuiColorEditFlags_Float);
            ImGui::ColorEdit3("Light diffuse", &m_light.diffuse.x, ImGuiColorEditFlags_Float);
            ImGui::ColorEdit3("Light specular", &m_light.specular.x, ImGuiColorEditFlags_Float);

            ImGui::SliderFloat("Light constant", &m_light.constant, 0.0f, 1.0f);
            ImGui::SliderFloat("Light linear", &m_light.linear, 0.0f, 1.0f);
            ImGui::SliderFloat("Light quadratic", &m_light.quadratic, 0.0f, 1.0f);
        }
        ImGui::End();
        ENGINE_INFO("ImGui widget created: {}", name);
    }

protected:
    std::string                              m_application_name;
    bool                                     m_is_running;
    bool                                     m_is_minimized;
    float                                    m_last_frame_time;
    core::LayerStack                         m_application_layer_stack;
    InputManager*                            m_input_manager;
    static Application*                      s_instance;
    sputnik::editor::EditorNew*              m_editor_new;
    GLFWwindow*                              m_window;
};

Application* CreateApplication(); // Must be defined by classes inheriting from this class
} // namespace sputnik::main
