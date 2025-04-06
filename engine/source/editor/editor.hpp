#pragma once

#include "core/core.h"
#include "core/system_information.h"

#include <vector.hpp>

#include <imgui.h>

namespace sputnik::editor
{

class EditorViewport;

using namespace sputnik::core;
using namespace ramanujan::experimental;

class Editor
{
public:
    virtual ~Editor();

    static Editor* getInstance();

    virtual void beginFrame();
    virtual void endFrame();
    virtual void update(sputnik::core::TimeStep& time_step);
    virtual void lateUpdate(sputnik::core::TimeStep& time_step);

    static void drawWidgetFloat(const std::string& label, float& value, const float& widget_width);

    static void
    drawWidgetVec3(const std::string& label, vec3& value, const float& widget_width, float default_value = 0.0f);

    static void
    drawWidgetVec4(const std::string& label, vec4& value, const float& widget_width, float default_value = 0.0f);

    static void
    drawWidgetVec2(const std::string& label, vec2& value, const float& widget_width, float default_value = 0.0f);

    static void
    drawWidgetColor3(const std::string& label, vec3& value, const float& widget_width, float default_value = 0.0f);

    static void
    drawWidgetColor4(const std::string& label, vec4& value, const float& widget_width, float default_value = 0.0f);

    static void drawWidgetText(const std::string& label,
                               cstring            value,
                               const float&       widget_width,
                               cstring            format = nullptr,
                               cstring            id     = "##");

    template <typename T>
    static void drawWidgetText(const std::string& label, T value, cstring format = nullptr, cstring id = "##");

    static bool drawWidgetCheckbox(const std::string& label, bool& value, const float& widget_width, cstring id = "##");

    // static void setMayaThemecolors();

    void beginViewportFrame();
    void endViewportFrame();

    bool isViewportActive() { return m_is_viewport_active; }

protected:
    Editor();

    void beginDockspace();
    void endDockspace();
    void renderMenuBar();

    void updateViewport(const core::TimeStep& time_step);

public:
    std::unique_ptr<EditorViewport> m_viewport;
    SystemInformation               m_system_information;

    inline static bool m_is_viewport_active{true};
    inline static bool m_is_dockspace_active{true};
};

template <typename T>
inline void Editor::drawWidgetText(const std::string& label, T value, cstring format, cstring id)
{
    // ImGui::Text(label.c_str());
    // ImGui::SameLine();
    // ImGui::Text(id);
    // ImGui::SameLine();
    // ImGui::Text(format, value);

    ImGui::PushID(id);

    ImGui::Columns(2);
    ImGui::AlignTextToFramePadding();
    // auto h1 = ImGui::GetCursorPosY();
    ImGui::Text(label.c_str());

    ImGui::NextColumn();

    // auto h2 = ImGui::GetCursorPosY();
    // ImGui::SetCursorPosY(ImGui::GetCursorPosY());
    ImGui::AlignTextToFramePadding();
    if(format == nullptr)
    {
        ImGui::Text(std::to_string(value).c_str());
    }
    else
    {
        ImGui::Text(format, value);
    }

    ImGui::Columns(1);

    ImGui::PopID();
}

} // namespace sputnik::editor
