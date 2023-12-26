#include "pch.h"

#include "editor.h"
#include "main/application.h"
#include "graphics/api/renderer.h"
#include "core/layers/layer_stack.h"

#include <memory>

namespace sputnik::editor
{

Editor::Editor()
{
    const auto& [width, height] = sputnik::graphics::api::Renderer::GetViewportDimensions();

    m_ui_layer               = std::make_shared<GlUiLayer>();
    m_editor_viewport_canvas = std::make_shared<sputnik::graphics::EditorViewPortCanvas>(width, height);

    main::Application::GetInstance()->PushOverlay(m_ui_layer);
    main::Application::GetInstance()->PushOverlay(m_editor_viewport_canvas);
}

Editor::~Editor() {}

void Editor::BeginFrame()
{
    // call imgui begin frame
    //m_ui_layer->Begin();
    m_editor_viewport_canvas->BeginFrame();
}

void Editor::EndFrame()
{
    // call imgui end frame
    m_editor_viewport_canvas->EndFrame();
    //m_ui_layer->End();
}

void Editor::Update(sputnik::core::TimeStep& time_step)
{
    m_ui_layer->Begin();

    sputnik::core::LayerStack& layer_stack = sputnik::main::Application::GetInstance()->GetApplicationLayerStack();

    for(const std::shared_ptr<sputnik::core::Layer>& layer : layer_stack)
    {
        layer->OnPreUpdateUI(time_step);
    }

    for(const std::shared_ptr<sputnik::core::Layer>& layer : layer_stack)
    {
        layer->OnUpdateUI(time_step);
    }

    for(const std::shared_ptr<sputnik::core::Layer>& layer : layer_stack)
    {
        layer->OnPostUpdateUI(time_step);
    }

    m_ui_layer->End();
}

} // namespace sputnik::editor
