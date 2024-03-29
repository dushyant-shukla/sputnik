#include "pch.h"
#include "editor_viewport_canvas.h"
#include "graphics/api/renderer.h"
#include "editor_camera.h"

#include <vector2.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <ImGuizmo.h>

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
    glcore::FrameBufferSpecification framebuffer_spec;
    framebuffer_spec.width         = m_window_width;
    framebuffer_spec.height        = m_window_height;
    framebuffer_spec.m_attachments = {glcore::FrameBufferTextureFormat::RGBA8, glcore::FrameBufferTextureFormat::DEPTH};
    m_framebuffer                  = std::make_shared<glcore::FrameBuffer>(framebuffer_spec);
}

void EditorViewPortCanvas::OnDetach() {}

void EditorViewPortCanvas::OnPreUpdate(const TimeStep& time_step)
{
    if(ShouldResizeFrameBuffer())
    {
        m_framebuffer->Resize(static_cast<uint32_t>(m_viewport_size.first),
                              static_cast<uint32_t>(m_viewport_size.second));
        graphics::api::EditorCamera::GetInstance()->SetViewportSize(m_viewport_size.first, m_viewport_size.second);
    }

    m_framebuffer->Bind();
    api::Renderer::Clear();

    graphics::api::EditorCamera::GetInstance()->Update(time_step);
}

void EditorViewPortCanvas::OnUpdate(const TimeStep& time_step) {}

void EditorViewPortCanvas::OnPostUpdate(const TimeStep& time_step)
{
    m_framebuffer->UnBind();
}

void EditorViewPortCanvas::OnEvent() {}

bool EditorViewPortCanvas::ShouldResizeFrameBuffer()
{
    glcore::FrameBufferSpecification framebuffer_spec = m_framebuffer->GetSpecification();
    if(m_viewport_size.first > 0.0f && m_viewport_size.second > 0.0f &&
       (framebuffer_spec.width != m_viewport_size.first || framebuffer_spec.height != m_viewport_size.second))
    {
        return true;
    }
    return false;
}

void EditorViewPortCanvas::OnPreUpdateUI(const TimeStep& time_step)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    if(ImGui::Begin("viewport"))
    {
        ImVec2 viewport_min_region = ImGui::GetWindowContentRegionMin();
        ImVec2 viewport_max_region = ImGui::GetWindowContentRegionMax();
        ImVec2 viewport_offset     = ImGui::GetWindowPos();
        m_viewport_bounds[0] = {viewport_min_region.x + viewport_offset.x, viewport_min_region.y + viewport_offset.y};
        m_viewport_bounds[1] = {viewport_max_region.x + viewport_offset.x, viewport_max_region.y + viewport_offset.y};

        // viewport bounds are the same as following min and max values (need imgui_internal.h)
        ImGuiWindow* window = GImGui->CurrentWindow;
        ImVec2       min    = window->ContentRegionRect.Min;
        ImVec2       max    = window->ContentRegionRect.Max;

        ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
        m_viewport_size            = {viewport_panel_size.x, viewport_panel_size.y};

        uint64_t textureID = m_framebuffer->GetColorAttachmentRendererId();
        ImGui::Image(reinterpret_cast<void*>(textureID),
                     ImVec2{m_viewport_size.first, m_viewport_size.second},
                     ImVec2{0, 1},
                     ImVec2{1, 0});

        // ImGuizmo setup
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();
        ImGuiIO& io = ImGui::GetIO();
        ImGuizmo::SetRect(m_viewport_bounds[0].first,
                          m_viewport_bounds[0].second,
                          m_viewport_bounds[1].first - m_viewport_bounds[0].first,
                          m_viewport_bounds[1].second - m_viewport_bounds[0].second);
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
    ImGui::End(); // end "viewport" window
    ImGui::PopStyleVar();
}

} // namespace sputnik::graphics
