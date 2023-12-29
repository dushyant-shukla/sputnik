#include "pch.h"
#include "editor_viewport.hpp"

#include "graphics/glcore/gl_framebuffer.h"
#include "core/systems/render_system.h"

#include <imgui.h>
#include <imgui_internal.h> // For GImGui->CurrentWindow;
#include <ImGuizmo.h>
#include <glad/glad.h>

namespace sputnik::editor
{

using namespace sputnik::core::systems;

EditorViewport::EditorViewport(const u32& width, const u32& height)
    : m_width(width)
    , m_height(height)
    , m_viewport_size{width, height}
    , m_viewport_bounds{std::make_pair(0.0f, 0.0f), std::make_pair(0.0f, 0.0f)}
    , m_viewport_focused{false}
    , m_viewport_hovered{false}
    , m_block_camera_update{false}
{
    FramebufferSpecification           framebuffer_spec;
    FramebufferAttachmentSpecification color_attachment_spec;
    color_attachment_spec.attachment_format = TextureFormat::RGBA8;

    FramebufferAttachmentSpecification depth_attachment_spec;
    depth_attachment_spec.attachment_format = TextureFormat::Depth24Stencil8;

    framebuffer_spec.attachments = {color_attachment_spec, depth_attachment_spec};
    framebuffer_spec.width       = m_width;
    framebuffer_spec.height      = m_height;
    m_framebuffer                = std::make_unique<OglFramebuffer>(framebuffer_spec);
}

EditorViewport::~EditorViewport() {}

void EditorViewport::beginFrame()
{
    m_framebuffer->bind();
    m_framebuffer->clear({1.0f, 1.0f, 1.0f, 1.0f});
    // graphics::api::Renderer::Clear();

    // graphics::api::Renderer::RenderAtmoshericScattering();
}

void EditorViewport::endFrame()
{
    m_framebuffer->unbind();
}

void EditorViewport::update(const core::TimeStep& timestep)
{
    resizeFramebuffer();
    renderEditorViewport();
}

void EditorViewport::resizeFramebuffer(const bool& force_resize)
{
    if(shouldResizeFramebuffer() || force_resize)
    {
        m_framebuffer->resize(static_cast<u32>(m_viewport_size.first), static_cast<u32>(m_viewport_size.second));
        graphics::api::EditorCamera::GetInstance()->SetViewportSize(m_viewport_size.first, m_viewport_size.second);
        graphics::api::Camera::GetInstance()->SetViewportSize(m_viewport_size.first, m_viewport_size.second);
    }
}

void EditorViewport::renderEditorViewport()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(3.0f, 3.0f));
    if(ImGui::Begin("viewport"))
    {
        m_viewport_focused    = ImGui::IsWindowFocused();
        m_viewport_hovered    = ImGui::IsWindowHovered();
        m_block_camera_update = !m_viewport_focused && !m_viewport_hovered;
        // graphics::api::Renderer::BlockCameraUpdate(m_block_camera_update);
        RenderSystem::getInstance()->blockCameraUpdate(m_block_camera_update);
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

        // glEnable(GL_BLEND);
        ////glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA); // This gives best results
        //  uint64_t textureID = m_framebuffer->GetColorAttachmentRendererId();
        uint64_t textureID = m_framebuffer->getColorAttachmentId();
        ImGui::Image(reinterpret_cast<void*>(textureID),
                     ImVec2{m_viewport_size.first, m_viewport_size.second},
                     ImVec2{0, 1},
                     ImVec2{1, 0});
        // glDisable(GL_BLEND);

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

    if(ImGui::Begin("Depth Buffer"))
    {
        // uint64_t textureID = m_framebuffer->GetColorAttachmentRendererId(1);
        uint64_t textureID = m_framebuffer->getDepthAttachmentId();
        ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{250, 250}, ImVec2{0, 1}, ImVec2{1, 0});
    }
    ImGui::End();
}

bool EditorViewport::shouldResizeFramebuffer()
{
    const auto& framebuffer_spec = m_framebuffer->getSpecification();
    if(m_viewport_size.first > 0.0f && m_viewport_size.second > 0.0f &&
       (framebuffer_spec.width != m_viewport_size.first || framebuffer_spec.height != m_viewport_size.second))
    {
        return true;
    }
    return false;
}

} // namespace sputnik::editor
