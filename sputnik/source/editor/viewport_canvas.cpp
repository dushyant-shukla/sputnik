#include "pch.h"
#include "viewport_canvas.h"
#include "graphics/api/renderer.h"
#include "graphics/api/renderer.h"

#include <vector2.h>
#include <imgui.h>

namespace sputnik::graphics
{

EditorViewPortCanvas::EditorViewPortCanvas(unsigned int width, unsigned int height)
    : Layer("EditorViewportCanvas")
    , m_window_width(width)
    , m_window_height(height)
    , m_viewport_size(width, height)
{
}

EditorViewPortCanvas::~EditorViewPortCanvas() {}

void EditorViewPortCanvas::OnAttach()
{
    m_framebuffer_spec.width         = m_window_width;
    m_framebuffer_spec.height        = m_window_height;
    m_framebuffer_spec.m_attachments = {glcore::FrameBufferTextureFormat::RGBA8,
                                        glcore::FrameBufferTextureFormat::DEPTH};
    m_framebuffer                    = std::make_shared<glcore::FrameBuffer>(m_framebuffer_spec);
}

void EditorViewPortCanvas::OnDetach() {}

void EditorViewPortCanvas::OnPreUpdate(const TimeStep& time_step)
{
    m_framebuffer->Bind();
    api::Renderer::Clear();
}

void EditorViewPortCanvas::OnUpdate(const TimeStep& time_step) {}

void EditorViewPortCanvas::OnPostUpdate(const TimeStep& time_step)
{
    m_framebuffer->UnBind();
}

void EditorViewPortCanvas::OnEvent() {}

void EditorViewPortCanvas::OnPreUpdateUI(const TimeStep& time_step)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    if(ImGui::Begin("viewport"))
    {
        ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
        m_viewport_size            = {viewport_panel_size.x, viewport_panel_size.y};
        uint64_t textureID         = m_framebuffer->GetColorAttachmentRendererId();
        ImGui::Image(reinterpret_cast<void*>(textureID),
                     ImVec2{m_viewport_size.x, m_viewport_size.y},
                     ImVec2{0, 1},
                     ImVec2{1, 0});
    }
}

void EditorViewPortCanvas::OnUpdateUI(const TimeStep& time_step)
{
    // if(ImGui::Begin("example"))
    //{
    //     ImGui::Text("sjhajkshakjh");
    // }
    // ImGui::End();
    auto& framebuffer_spec = m_framebuffer->GetSpecification();

    // ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    // if(ImGui::Begin("view port"))
    //{
    //     ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
    //     m_viewport_size            = {viewport_panel_size.x, viewport_panel_size.y};
    //     uint64_t textureID         = m_framebuffer->GetColorAttachmentRendererId();
    //     ImGui::Image(reinterpret_cast<void*>(textureID),
    //                  ImVec2{m_viewport_size.x, m_viewport_size.y},
    //                  ImVec2{0, 1},
    //                  ImVec2{1, 0});
    // }
    // ImGui::End();
    // ImGui::PopStyleVar();
}

void EditorViewPortCanvas::OnPostUpdateUI(const TimeStep& time_step)
{
    ImGui::End(); // end viewport window
    ImGui::PopStyleVar();
}

} // namespace sputnik::graphics
