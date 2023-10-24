#include "pch.h"
#include "editor_viewport_canvas.h"
#include "graphics/api/renderer.h"
#include "editor_camera.h"
#include "graphics/api/camera.h"

#include <vector2.h>
#include <imgui.h>
#include <imgui_internal.h> // For GImGui->CurrentWindow;
#include <ImGuizmo.h>

namespace sputnik::graphics
{

EditorViewPortCanvas::EditorViewPortCanvas(unsigned int width, unsigned int height)
    : Layer("EditorViewportCanvas")
    , m_window_width(width)
    , m_window_height(height)
    , m_viewport_size(width, height)
{
    glcore::FrameBufferSpecification framebuffer_spec;
    framebuffer_spec.width         = m_window_width;
    framebuffer_spec.height        = m_window_height;
    framebuffer_spec.m_attachments = {glcore::FrameBufferTextureFormat::RGBA8, glcore::FrameBufferTextureFormat::DEPTH};
    m_framebuffer                  = std::make_shared<glcore::FrameBuffer>(framebuffer_spec);
}

EditorViewPortCanvas::~EditorViewPortCanvas() {}

void EditorViewPortCanvas::BeginFrame()
{
    m_framebuffer->Bind();
    api::Renderer::Clear();
}

void EditorViewPortCanvas::EndFrame()
{
    m_framebuffer->UnBind();
}

void EditorViewPortCanvas::OnAttach() {}

void EditorViewPortCanvas::OnDetach() {}

void EditorViewPortCanvas::OnPreUpdate(const TimeStep& time_step)
{
    if(ShouldResizeFrameBuffer())
    {
        m_framebuffer->Resize(static_cast<uint32_t>(m_viewport_size.first),
                              static_cast<uint32_t>(m_viewport_size.second));
        graphics::api::EditorCamera::GetInstance()->SetViewportSize(m_viewport_size.first, m_viewport_size.second);
        graphics::api::Camera::GetInstance()->SetViewportSize(m_viewport_size.first, m_viewport_size.second);
    }

    //m_framebuffer->Bind();
    //api::Renderer::Clear();

    graphics::api::EditorCamera::GetInstance()->Update(time_step);
    graphics::api::Camera::GetInstance()->Update(time_step, m_block_camera_update);
}

void EditorViewPortCanvas::OnUpdate(const TimeStep& time_step) {}

void EditorViewPortCanvas::OnPostUpdate(const TimeStep& time_step)
{
    //m_framebuffer->UnBind();
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
        m_viewport_focused    = ImGui::IsWindowFocused();
        m_viewport_hovered    = ImGui::IsWindowHovered();
        m_block_camera_update = !m_viewport_focused && !m_viewport_hovered;
        // std::cout << "block camera update: " << m_block_camera_update << std::endl;

        ImVec2 viewport_min_region = ImGui::GetWindowContentRegionMin();
        ImVec2 viewport_max_region = ImGui::GetWindowContentRegionMax();
        ImVec2 viewport_offset     = ImGui::GetWindowPos();
        m_viewport_bounds[0] = {viewport_min_region.x + viewport_offset.x, viewport_min_region.y + viewport_offset.y};
        m_viewport_bounds[1] = {viewport_max_region.x + viewport_offset.x, viewport_max_region.y + viewport_offset.y};

        // viewport bounds are the same as the following min and max values (need imgui_internal.h)
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
    ImGui::End(); // end "viewport" window
    ImGui::PopStyleVar();
}

void EditorViewPortCanvas::OnUpdateUI(const TimeStep& time_step) {}

void EditorViewPortCanvas::OnPostUpdateUI(const TimeStep& time_step)
{
    // ImGui::End(); // end "viewport" window
    // ImGui::PopStyleVar();
}

} // namespace sputnik::graphics
