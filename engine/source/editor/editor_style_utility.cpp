#include "pch.h"
#include "editor_style_utility.hpp"

#include <imgui.h>

namespace sputnik::editor
{

std::unordered_map<EditorStyleUtility::Theme, std::string> EditorStyleUtility::sThemes = {
    {EditorStyleUtility::Theme::DARK, "Dark"},
    {EditorStyleUtility::Theme::LIGHTER_DARK, "Lighter Dark"},
    {EditorStyleUtility::Theme::DEEP_DARK, "Deep Dark"},
    {EditorStyleUtility::Theme::LIGHT, "Light"},
    {EditorStyleUtility::Theme::CHERRY, "Cherry"},
    {EditorStyleUtility::Theme::GOLDEN_BLACK, "Golden Black"},
    {EditorStyleUtility::Theme::STREAMING_LIFE, "Streaming Life"},
    {EditorStyleUtility::Theme::SOFT_LIFE, "Soft Life"},
    {EditorStyleUtility::Theme::WHITE, "White"},
    {EditorStyleUtility::Theme::MAYA, "Maya"}};

std::unordered_map<EditorStyleUtility::Theme, void (*)()> EditorStyleUtility::sThemeFunctions = {
    {EditorStyleUtility::Theme::DARK, &SetDarkThemeColors},
    {EditorStyleUtility::Theme::LIGHTER_DARK, &SetLighterDarkThemeColors},
    {EditorStyleUtility::Theme::DEEP_DARK, &SetDeepDarkThemeColors},
    {EditorStyleUtility::Theme::LIGHT, &SetLightThemeColors},
    {EditorStyleUtility::Theme::CHERRY, &SetCherryThemeColors},
    {EditorStyleUtility::Theme::GOLDEN_BLACK, &SetGoldenBlackThemeColors},
    {EditorStyleUtility::Theme::STREAMING_LIFE, &SetStreamingLifeThemeColors},
    {EditorStyleUtility::Theme::SOFT_LIFE, &SetSoftLifeThemeColors},
    {EditorStyleUtility::Theme::WHITE, &SetWhiteThemeColors},
    {EditorStyleUtility::Theme::MAYA, &SetMayaThemecolors}};

EditorStyleUtility::Theme EditorStyleUtility::sActiveTheme = EditorStyleUtility::Theme::MAYA;

/* clone of ImLerp(...) in imgui_internal.h */
static inline ImVec4 ImColorLerp(const ImVec4& a, const ImVec4& b, float t)
{
    return ImVec4(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t);
}
static inline void CreateDefaultTabColorsFor(ImGuiStyle&   style,
                                             ImGuiCol      baseHoveredColor  = ImGuiCol_HeaderHovered,
                                             float         hoveredLerp       = 0.5f,
                                             const ImVec2& tabsLerps         = ImVec2(0.15f, 0.8f),
                                             const ImVec2& unfocusedTabsLerp = ImVec2(0.1f, 0.5f))
{
    IM_ASSERT(baseHoveredColor + 1 < ImGuiCol_COUNT);
    style.Colors[ImGuiCol_TabHovered] =
        ImColorLerp(style.Colors[baseHoveredColor], style.Colors[baseHoveredColor + 1], hoveredLerp);

    style.Colors[ImGuiCol_Tab] =
        ImColorLerp(style.Colors[ImGuiCol_WindowBg], style.Colors[ImGuiCol_TabHovered], tabsLerps.x);
    style.Colors[ImGuiCol_TabActive] =
        ImColorLerp(style.Colors[ImGuiCol_TitleBgActive], style.Colors[ImGuiCol_TabHovered], tabsLerps.y);

    style.Colors[ImGuiCol_TabUnfocused] =
        ImColorLerp(style.Colors[ImGuiCol_WindowBg], style.Colors[ImGuiCol_Tab], unfocusedTabsLerp.x);
    style.Colors[ImGuiCol_TabUnfocusedActive] =
        ImColorLerp(style.Colors[ImGuiCol_WindowBg], style.Colors[ImGuiCol_TabActive], unfocusedTabsLerp.y);
}

void EditorStyleUtility::SetDarkThemeColors()
{
    ImGuiStyle* style                             = &ImGui::GetStyle();
    style->Colors[ImGuiCol_Text]                  = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style->Colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    style->Colors[ImGuiCol_WindowBg]              = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    style->Colors[ImGuiCol_ChildBg]               = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style->Colors[ImGuiCol_PopupBg]               = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
    style->Colors[ImGuiCol_Border]                = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
    style->Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
    style->Colors[ImGuiCol_FrameBg]               = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    style->Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    style->Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    style->Colors[ImGuiCol_TitleBg]               = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    style->Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
    style->Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    style->Colors[ImGuiCol_CheckMark]             = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    style->Colors[ImGuiCol_SliderGrab]            = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    style->Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    style->Colors[ImGuiCol_Button]                = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    style->Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    style->Colors[ImGuiCol_ButtonActive]          = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    style->Colors[ImGuiCol_Header]                = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    style->Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
    style->Colors[ImGuiCol_HeaderActive]          = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
    style->Colors[ImGuiCol_Separator]             = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    style->Colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    style->Colors[ImGuiCol_SeparatorActive]       = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    style->Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    style->Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    style->Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    style->Colors[ImGuiCol_Tab]                   = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    style->Colors[ImGuiCol_TabHovered]            = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    style->Colors[ImGuiCol_TabActive]             = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
    style->Colors[ImGuiCol_TabUnfocused]          = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    style->Colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    style->Colors[ImGuiCol_PlotLines]             = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogram]         = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    style->Colors[ImGuiCol_TableBorderStrong]     = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    style->Colors[ImGuiCol_TableBorderLight]      = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    style->Colors[ImGuiCol_TableRowBg]            = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style->Colors[ImGuiCol_TableRowBgAlt]         = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    style->Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    style->Colors[ImGuiCol_DragDropTarget]        = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    style->Colors[ImGuiCol_NavHighlight]          = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
    style->Colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
    style->Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

#ifdef IMGUI_HAS_DOCK
    style->Colors[ImGuiCol_DockingPreview] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    style->Colors[ImGuiCol_DockingEmptyBg] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
#endif
}

void EditorStyleUtility::SetLighterDarkThemeColors()
{
    ImGui::StyleColorsDark();

    auto& colors              = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImVec4{0.1f, 0.105f, 0.11f, 1.0f};
    colors[ImGuiCol_ChildBg]  = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);

    // Headers
    colors[ImGuiCol_Header]        = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
    colors[ImGuiCol_HeaderHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
    colors[ImGuiCol_HeaderActive]  = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

    // Buttons
    colors[ImGuiCol_Button]        = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
    colors[ImGuiCol_ButtonHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
    colors[ImGuiCol_ButtonActive]  = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

    // Frame BG
    colors[ImGuiCol_FrameBg]        = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
    colors[ImGuiCol_FrameBgHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
    colors[ImGuiCol_FrameBgActive]  = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

    // Tabs
    colors[ImGuiCol_Tab]                = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TabHovered]         = ImVec4{0.38f, 0.3805f, 0.381f, 1.0f};
    colors[ImGuiCol_TabActive]          = ImVec4{0.28f, 0.2805f, 0.281f, 1.0f};
    colors[ImGuiCol_TabUnfocused]       = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};

    // Title
    colors[ImGuiCol_TitleBg]          = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TitleBgActive]    = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
}

void EditorStyleUtility::SetDeepDarkThemeColors()
{
    ImGuiStyle* style = &ImGui::GetStyle();

    style->Colors[ImGuiCol_Text]                 = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
    style->Colors[ImGuiCol_TextDisabled]         = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_WindowBg]             = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_ChildBg]              = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_PopupBg]              = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_Border]               = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
    style->Colors[ImGuiCol_BorderShadow]         = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
    style->Colors[ImGuiCol_FrameBg]              = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_FrameBgHovered]       = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_FrameBgActive]        = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_TitleBg]              = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed]     = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
    style->Colors[ImGuiCol_TitleBgActive]        = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_MenuBarBg]            = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarBg]          = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrab]        = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_CheckMark]            = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_SliderGrab]           = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_SliderGrabActive]     = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_Button]               = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ButtonHovered]        = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_ButtonActive]         = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    // style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_Header]               = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    style->Colors[ImGuiCol_HeaderHovered]        = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_HeaderActive]         = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_ResizeGrip]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style->Colors[ImGuiCol_ResizeGripHovered]    = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ResizeGripActive]     = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_PlotLines]            = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style->Colors[ImGuiCol_PlotLinesHovered]     = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogram]        = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_TextSelectedBg]       = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
    // style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
    style->Colors[ImGuiCol_ModalWindowDimBg]   = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
    style->Colors[ImGuiCol_Tab]                = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    style->Colors[ImGuiCol_TabHovered]         = ImVec4{0.38f, 0.3805f, 0.381f, 1.0f};
    style->Colors[ImGuiCol_TabActive]          = ImVec4{0.28f, 0.2805f, 0.281f, 1.0f};
    style->Colors[ImGuiCol_TabUnfocused]       = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    style->Colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
}

// unused
void EditorStyleUtility::SetGreenThemeColors()
{
    ImGuiStyle* style                            = &ImGui::GetStyle();
    style->Colors[ImGuiCol_Text]                 = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style->Colors[ImGuiCol_TextDisabled]         = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
    style->Colors[ImGuiCol_WindowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_ChildBg]              = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
    style->Colors[ImGuiCol_PopupBg]              = ImVec4(0.47f, 0.77f, 0.83f, 1.00f);
    style->Colors[ImGuiCol_Border]               = ImVec4(0.31f, 0.31f, 1.00f, 1.00f);
    style->Colors[ImGuiCol_BorderShadow]         = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_FrameBg]              = ImVec4(0.00f, 0.32f, 0.27f, 1.00f);
    style->Colors[ImGuiCol_FrameBgHovered]       = ImVec4(0.11f, 0.74f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_FrameBgActive]        = ImVec4(0.07f, 0.70f, 0.01f, 1.00f);
    style->Colors[ImGuiCol_TitleBg]              = ImVec4(0.00f, 0.43f, 0.24f, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed]     = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
    style->Colors[ImGuiCol_TitleBgActive]        = ImVec4(0.07f, 0.67f, 0.01f, 1.00f);
    style->Colors[ImGuiCol_MenuBarBg]            = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarBg]          = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrab]        = ImVec4(0.00f, 0.43f, 0.24f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.19f, 0.76f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4(0.03f, 0.70f, 0.06f, 1.00f);
    style->Colors[ImGuiCol_CheckMark]            = ImVec4(0.00f, 0.71f, 0.39f, 1.00f);
    style->Colors[ImGuiCol_SliderGrab]           = ImVec4(0.02f, 0.59f, 0.38f, 1.00f);
    style->Colors[ImGuiCol_SliderGrabActive]     = ImVec4(0.00f, 0.49f, 0.51f, 1.00f);
    style->Colors[ImGuiCol_Button]               = ImVec4(0.00f, 0.43f, 0.24f, 1.00f);
    style->Colors[ImGuiCol_ButtonHovered]        = ImVec4(0.08f, 0.77f, 0.03f, 1.00f);
    style->Colors[ImGuiCol_ButtonActive]         = ImVec4(0.08f, 0.72f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_Header]               = ImVec4(0.11f, 0.64f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_HeaderHovered]        = ImVec4(0.03f, 0.70f, 0.01f, 1.00f);
    style->Colors[ImGuiCol_HeaderActive]         = ImVec4(0.05f, 0.72f, 0.01f, 1.00f);
    style->Colors[ImGuiCol_ResizeGrip]           = ImVec4(0.47f, 0.77f, 0.83f, 1.00f);
    style->Colors[ImGuiCol_ResizeGripHovered]    = ImVec4(0.00f, 0.55f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ResizeGripActive]     = ImVec4(0.00f, 0.71f, 0.01f, 1.00f);
    style->Colors[ImGuiCol_PlotLines]            = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
    style->Colors[ImGuiCol_PlotLinesHovered]     = ImVec4(0.05f, 0.64f, 0.01f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogram]        = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.04f, 0.70f, 0.02f, 1.00f);
    style->Colors[ImGuiCol_TextSelectedBg]       = ImVec4(0.02f, 0.57f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_ModalWindowDimBg]     = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
    style->Colors[ImGuiCol_Tab]                  = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
    style->Colors[ImGuiCol_TabHovered]           = ImVec4(0.05f, 0.64f, 0.01f, 1.00f);
    style->Colors[ImGuiCol_TabActive]            = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
    style->Colors[ImGuiCol_TabUnfocused]         = ImVec4(0.04f, 0.70f, 0.02f, 1.00f);
    style->Colors[ImGuiCol_TabUnfocusedActive]   = ImVec4(0.02f, 0.57f, 0.00f, 1.00f);
}

void EditorStyleUtility::SetLightThemeColors()
{
    ImGui::StyleColorsLight();
}

void EditorStyleUtility::SetCherryThemeColors()
{
// cherry colors, 3 intensities
#define CHERRY_HI(v) ImVec4(0.502f, 0.075f, 0.256f, v)
#define CHERRY_MED(v) ImVec4(0.455f, 0.198f, 0.301f, v)
#define CHERRY_LOW(v) ImVec4(0.232f, 0.201f, 0.271f, v)
    // backgrounds (@todo: complete with CHERRY_BG_MED, CHERRY_BG_LOW)
#define CHERRY_BG(v) ImVec4(0.200f, 0.220f, 0.270f, v)
    // text
#define CHERRY_TEXT(v) ImVec4(0.860f, 0.930f, 0.890f, v)

    ImGuiStyle* style                            = &ImGui::GetStyle();
    style->Colors[ImGuiCol_Text]                 = CHERRY_TEXT(0.78f);
    style->Colors[ImGuiCol_TextDisabled]         = CHERRY_TEXT(0.28f);
    style->Colors[ImGuiCol_WindowBg]             = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
    style->Colors[ImGuiCol_ChildBg]              = CHERRY_BG(0.58f);
    style->Colors[ImGuiCol_PopupBg]              = CHERRY_BG(0.9f);
    style->Colors[ImGuiCol_Border]               = ImVec4(0.31f, 0.31f, 1.00f, 0.00f);
    style->Colors[ImGuiCol_BorderShadow]         = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style->Colors[ImGuiCol_FrameBg]              = CHERRY_BG(1.00f);
    style->Colors[ImGuiCol_FrameBgHovered]       = CHERRY_MED(0.78f);
    style->Colors[ImGuiCol_FrameBgActive]        = CHERRY_MED(1.00f);
    style->Colors[ImGuiCol_TitleBg]              = CHERRY_LOW(1.00f);
    style->Colors[ImGuiCol_TitleBgActive]        = CHERRY_HI(1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed]     = CHERRY_BG(0.75f);
    style->Colors[ImGuiCol_MenuBarBg]            = CHERRY_BG(0.47f);
    style->Colors[ImGuiCol_ScrollbarBg]          = CHERRY_BG(1.00f);
    style->Colors[ImGuiCol_ScrollbarGrab]        = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = CHERRY_MED(0.78f);
    style->Colors[ImGuiCol_ScrollbarGrabActive]  = CHERRY_MED(1.00f);
    style->Colors[ImGuiCol_CheckMark]            = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
    style->Colors[ImGuiCol_SliderGrab]           = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
    style->Colors[ImGuiCol_SliderGrabActive]     = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
    style->Colors[ImGuiCol_Button]               = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
    style->Colors[ImGuiCol_ButtonHovered]        = CHERRY_MED(0.86f);
    style->Colors[ImGuiCol_ButtonActive]         = CHERRY_MED(1.00f);
    style->Colors[ImGuiCol_Header]               = CHERRY_MED(0.76f);
    style->Colors[ImGuiCol_HeaderHovered]        = CHERRY_MED(0.86f);
    style->Colors[ImGuiCol_HeaderActive]         = CHERRY_HI(1.00f);
    style->Colors[ImGuiCol_Separator]            = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
    style->Colors[ImGuiCol_SeparatorHovered]     = CHERRY_MED(0.78f);
    style->Colors[ImGuiCol_SeparatorActive]      = CHERRY_MED(1.00f);
    style->Colors[ImGuiCol_ResizeGrip]           = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
    style->Colors[ImGuiCol_ResizeGripHovered]    = CHERRY_MED(0.78f);
    style->Colors[ImGuiCol_ResizeGripActive]     = CHERRY_MED(1.00f);
    style->Colors[ImGuiCol_PlotLines]            = CHERRY_TEXT(0.63f);
    style->Colors[ImGuiCol_PlotLinesHovered]     = CHERRY_MED(1.00f);
    style->Colors[ImGuiCol_PlotHistogram]        = CHERRY_TEXT(0.63f);
    style->Colors[ImGuiCol_PlotHistogramHovered] = CHERRY_MED(1.00f);
    style->Colors[ImGuiCol_TextSelectedBg]       = CHERRY_MED(0.43f);
    // [...]
    style->Colors[ImGuiCol_ModalWindowDimBg] = CHERRY_BG(0.73f);
    style->Colors[ImGuiCol_Border]           = ImVec4(0.539f, 0.479f, 0.255f, 0.162f);

#undef CHERRY_HI
#undef CHERRY_MED
#undef CHERRY_LOW
#undef CHERRY_BG
#undef CHERRY_TEXT

    CreateDefaultTabColorsFor(*style, ImGuiCol_HeaderHovered, 0.65f, ImVec2(0.4f, 0.6f), ImVec2(0.2f, 0.4f));
}

void EditorStyleUtility::SetGoldenBlackThemeColors()
{
    ImGuiStyle* style                             = &ImGui::GetStyle();
    style->Colors[ImGuiCol_Text]                  = ImVec4(0.92f, 0.92f, 0.92f, 1.00f);
    style->Colors[ImGuiCol_TextDisabled]          = ImVec4(0.44f, 0.44f, 0.44f, 1.00f);
    style->Colors[ImGuiCol_WindowBg]              = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    style->Colors[ImGuiCol_ChildBg]               = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style->Colors[ImGuiCol_PopupBg]               = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    style->Colors[ImGuiCol_Border]                = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
    style->Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style->Colors[ImGuiCol_FrameBg]               = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
    style->Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
    style->Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
    style->Colors[ImGuiCol_TitleBg]               = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
    style->Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    style->Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.06f, 0.06f, 0.06f, 0.53f);
    style->Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.81f, 0.83f, 0.81f, 1.00f);
    style->Colors[ImGuiCol_CheckMark]             = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
    style->Colors[ImGuiCol_SliderGrab]            = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    style->Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    style->Colors[ImGuiCol_Button]                = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
    style->Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    style->Colors[ImGuiCol_ButtonActive]          = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
    style->Colors[ImGuiCol_Header]                = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
    style->Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    style->Colors[ImGuiCol_HeaderActive]          = ImVec4(0.93f, 0.65f, 0.14f, 1.00f);
    style->Colors[ImGuiCol_Separator]             = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
    style->Colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    style->Colors[ImGuiCol_SeparatorActive]       = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
    style->Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
    style->Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    style->Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
    style->Colors[ImGuiCol_Tab]                   = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
    style->Colors[ImGuiCol_TabHovered]            = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
    style->Colors[ImGuiCol_TabActive]             = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
    style->Colors[ImGuiCol_TabUnfocused]          = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
    style->Colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
    style->Colors[ImGuiCol_PlotLines]             = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    style->Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    style->Colors[ImGuiCol_DragDropTarget]        = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    style->Colors[ImGuiCol_NavHighlight]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style->Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    style->Colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    style->Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

void EditorStyleUtility::SetStreamingLifeThemeColors()
{
    ImGuiStyle* style                             = &ImGui::GetStyle();
    style->Colors[ImGuiCol_Text]                  = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style->Colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    style->Colors[ImGuiCol_WindowBg]              = ImVec4(0.29f, 0.34f, 0.26f, 1.00f);
    style->Colors[ImGuiCol_ChildBg]               = ImVec4(0.29f, 0.34f, 0.26f, 1.00f);
    style->Colors[ImGuiCol_PopupBg]               = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
    style->Colors[ImGuiCol_Border]                = ImVec4(0.54f, 0.57f, 0.51f, 0.50f);
    style->Colors[ImGuiCol_BorderShadow]          = ImVec4(0.14f, 0.16f, 0.11f, 0.52f);
    style->Colors[ImGuiCol_FrameBg]               = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
    style->Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.27f, 0.30f, 0.23f, 1.00f);
    style->Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.30f, 0.34f, 0.26f, 1.00f);
    style->Colors[ImGuiCol_TitleBg]               = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
    style->Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.29f, 0.34f, 0.26f, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    style->Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.28f, 0.32f, 0.24f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.25f, 0.30f, 0.22f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.23f, 0.27f, 0.21f, 1.00f);
    style->Colors[ImGuiCol_CheckMark]             = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
    style->Colors[ImGuiCol_SliderGrab]            = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
    style->Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.54f, 0.57f, 0.51f, 0.50f);
    style->Colors[ImGuiCol_Button]                = ImVec4(0.29f, 0.34f, 0.26f, 0.40f);
    style->Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
    style->Colors[ImGuiCol_ButtonActive]          = ImVec4(0.54f, 0.57f, 0.51f, 0.50f);
    style->Colors[ImGuiCol_Header]                = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
    style->Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.35f, 0.42f, 0.31f, 0.6f);
    style->Colors[ImGuiCol_HeaderActive]          = ImVec4(0.54f, 0.57f, 0.51f, 0.50f);
    style->Colors[ImGuiCol_Separator]             = ImVec4(0.14f, 0.16f, 0.11f, 1.00f);
    style->Colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.54f, 0.57f, 0.51f, 1.00f);
    style->Colors[ImGuiCol_SeparatorActive]       = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
    style->Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.19f, 0.23f, 0.18f, 0.00f); // grip invis
    style->Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.54f, 0.57f, 0.51f, 1.00f);
    style->Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
    style->Colors[ImGuiCol_Tab]                   = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
    style->Colors[ImGuiCol_TabHovered]            = ImVec4(0.54f, 0.57f, 0.51f, 0.78f);
    style->Colors[ImGuiCol_TabActive]             = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
    style->Colors[ImGuiCol_TabUnfocused]          = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
    style->Colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.35f, 0.42f, 0.31f, 1.00f);
    style->Colors[ImGuiCol_PlotLines]             = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    style->Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogram]         = ImVec4(1.00f, 0.78f, 0.28f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
    style->Colors[ImGuiCol_DragDropTarget]        = ImVec4(0.73f, 0.67f, 0.24f, 1.00f);
    style->Colors[ImGuiCol_NavHighlight]          = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
    style->Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    style->Colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    style->Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

#ifdef IMGUI_HAS_DOCK
    style->Colors[ImGuiCol_DockingPreview] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
    style->Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
#endif
}

void EditorStyleUtility::SetSoftLifeThemeColors()
{
    ImGuiStyle* style                             = &ImGui::GetStyle();
    style->Colors[ImGuiCol_Text]                  = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style->Colors[ImGuiCol_TextDisabled]          = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
    style->Colors[ImGuiCol_WindowBg]              = ImVec4(0.37f, 0.49f, 0.41f, 1.00f);
    style->Colors[ImGuiCol_ChildBg]               = ImVec4(0.40f, 0.52f, 0.44f, 1.00f);
    style->Colors[ImGuiCol_PopupBg]               = ImVec4(0.45f, 0.60f, 0.47f, 1.00f);
    style->Colors[ImGuiCol_Border]                = ImVec4(0.25f, 0.28f, 0.25f, 0.50f);
    style->Colors[ImGuiCol_BorderShadow]          = ImVec4(0.51f, 0.74f, 0.54f, 0.52f);
    style->Colors[ImGuiCol_FrameBg]               = ImVec4(0.54f, 0.73f, 0.57f, 0.54f);
    style->Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.54f, 0.70f, 0.56f, 1.00f);
    style->Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.42f, 0.55f, 0.45f, 1.00f);
    style->Colors[ImGuiCol_TitleBg]               = ImVec4(0.36f, 0.47f, 0.37f, 0.82f);
    style->Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.39f, 0.57f, 0.54f, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.36f, 0.47f, 0.37f, 0.53f);
    style->Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.41f, 0.55f, 0.43f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.32f, 0.44f, 0.37f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.51f, 0.68f, 0.55f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.51f, 0.70f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.44f, 0.57f, 0.49f, 1.00f);
    style->Colors[ImGuiCol_CheckMark]             = ImVec4(0.25f, 0.42f, 0.13f, 1.00f);
    style->Colors[ImGuiCol_SliderGrab]            = ImVec4(0.32f, 0.44f, 0.36f, 1.00f);
    style->Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.32f, 0.36f, 0.33f, 0.50f);
    style->Colors[ImGuiCol_Button]                = ImVec4(0.26f, 0.35f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.32f, 0.43f, 0.36f, 0.40f);
    style->Colors[ImGuiCol_ButtonActive]          = ImVec4(0.51f, 0.69f, 0.55f, 0.50f);
    style->Colors[ImGuiCol_Header]                = ImVec4(0.39f, 0.59f, 0.55f, 1.00f);
    style->Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.49f, 0.75f, 0.70f, 0.71f);
    style->Colors[ImGuiCol_HeaderActive]          = ImVec4(0.75f, 1.00f, 0.93f, 0.57f);
    style->Colors[ImGuiCol_Separator]             = ImVec4(0.48f, 0.69f, 0.50f, 1.00f);
    style->Colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.28f, 0.31f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_SeparatorActive]       = ImVec4(0.29f, 0.49f, 0.15f, 1.00f);
    style->Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.52f, 0.66f, 0.59f, 0.00f);
    style->Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.32f, 0.35f, 0.32f, 1.00f);
    style->Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.34f, 0.59f, 0.18f, 1.00f);
    style->Colors[ImGuiCol_Tab]                   = ImVec4(0.43f, 0.58f, 0.48f, 1.00f);
    style->Colors[ImGuiCol_TabHovered]            = ImVec4(0.38f, 0.43f, 0.40f, 0.78f);
    style->Colors[ImGuiCol_TabActive]             = ImVec4(0.34f, 0.59f, 0.18f, 1.00f);
    style->Colors[ImGuiCol_TabUnfocused]          = ImVec4(0.54f, 0.73f, 0.57f, 1.00f);
    style->Colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.43f, 0.58f, 0.48f, 1.00f);
    style->Colors[ImGuiCol_PlotLines]             = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    style->Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.34f, 0.59f, 0.18f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.64f, 0.91f, 0.25f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(0.60f, 0.88f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.28f, 0.48f, 0.15f, 1.00f);
    style->Colors[ImGuiCol_DragDropTarget]        = ImVec4(0.37f, 0.62f, 0.20f, 1.00f);
    style->Colors[ImGuiCol_NavHighlight]          = ImVec4(0.28f, 0.49f, 0.15f, 1.00f);
    style->Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.90f, 0.90f, 0.90f, 0.70f);
    style->Colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.71f, 0.71f, 0.71f, 0.20f);
    style->Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.68f, 0.68f, 0.68f, 0.35f);

#ifdef IMGUI_HAS_DOCK
    style->Colors[ImGuiCol_DockingPreview] = ImVec4(0.59f, 0.54f, 0.18f, 1.00f);
    style->Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
#endif
}

void EditorStyleUtility::SetGreenBlueThemeColors()
{
    ImGuiStyle* style                             = &ImGui::GetStyle();
    style->Colors[ImGuiCol_Text]                  = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style->Colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    style->Colors[ImGuiCol_WindowBg]              = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
    style->Colors[ImGuiCol_ChildBg]               = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style->Colors[ImGuiCol_PopupBg]               = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    style->Colors[ImGuiCol_Border]                = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    style->Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style->Colors[ImGuiCol_FrameBg]               = ImVec4(0.44f, 0.44f, 0.44f, 0.60f);
    style->Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.57f, 0.57f, 0.57f, 0.70f);
    style->Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.76f, 0.76f, 0.76f, 0.80f);
    style->Colors[ImGuiCol_TitleBg]               = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    style->Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.00f, 0.00f, 0.00f, 0.60f);
    style->Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    style->Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    style->Colors[ImGuiCol_CheckMark]             = ImVec4(0.13f, 0.75f, 0.55f, 0.80f);
    style->Colors[ImGuiCol_SliderGrab]            = ImVec4(0.13f, 0.75f, 0.75f, 0.80f);
    style->Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
    style->Colors[ImGuiCol_Button]                = ImVec4(0.13f, 0.75f, 0.55f, 0.40f);
    style->Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.13f, 0.75f, 0.75f, 0.60f);
    style->Colors[ImGuiCol_ButtonActive]          = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
    style->Colors[ImGuiCol_Header]                = ImVec4(0.13f, 0.75f, 0.55f, 0.40f);
    style->Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.13f, 0.75f, 0.75f, 0.60f);
    style->Colors[ImGuiCol_HeaderActive]          = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
    style->Colors[ImGuiCol_Separator]             = ImVec4(0.13f, 0.75f, 0.55f, 0.40f);
    style->Colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.13f, 0.75f, 0.75f, 0.60f);
    style->Colors[ImGuiCol_SeparatorActive]       = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
    style->Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.13f, 0.75f, 0.55f, 0.40f);
    style->Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.13f, 0.75f, 0.75f, 0.60f);
    style->Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
    style->Colors[ImGuiCol_Tab]                   = ImVec4(0.13f, 0.75f, 0.55f, 0.80f);
    style->Colors[ImGuiCol_TabHovered]            = ImVec4(0.13f, 0.75f, 0.75f, 0.80f);
    style->Colors[ImGuiCol_TabActive]             = ImVec4(0.13f, 0.75f, 1.00f, 0.80f);
    style->Colors[ImGuiCol_TabUnfocused]          = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
    style->Colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.36f, 0.36f, 0.36f, 0.54f);
    style->Colors[ImGuiCol_PlotLines]             = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    style->Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    style->Colors[ImGuiCol_TableBorderStrong]     = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
    style->Colors[ImGuiCol_TableBorderLight]      = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
    style->Colors[ImGuiCol_TableRowBg]            = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style->Colors[ImGuiCol_TableRowBgAlt]         = ImVec4(1.00f, 1.00f, 1.00f, 0.07f);
    style->Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    style->Colors[ImGuiCol_DragDropTarget]        = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    style->Colors[ImGuiCol_NavHighlight]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style->Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    style->Colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    style->Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

#ifdef IMGUI_HAS_DOCK
    style->Colors[ImGuiCol_DockingPreview] = ImVec4(0.13f, 0.75f, 0.55f, 0.80f);
    style->Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.13f, 0.13f, 0.13f, 0.80f);
#endif
}

void EditorStyleUtility::SetWhiteThemeColors()
{
    ImGuiStyle* style                    = &ImGui::GetStyle();
    style->Colors[ImGuiCol_Text]         = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
    style->Colors[ImGuiCol_WindowBg]     = ImVec4(0.92f, 0.92f, 0.92f, 1.00f);
    style->Colors[ImGuiCol_ChildBg]      = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
    style->Colors[ImGuiCol_PopupBg]      = ImVec4(1.00f, 1.00f, 1.00f, 0.92f);
    // style.Colors[ImGuiCol_ComboBg]               = ImVec4(0.96f, 0.96f, 0.96f, 0.92f);
    style->Colors[ImGuiCol_Border]               = ImVec4(0.73f, 0.73f, 0.73f, 0.65f);
    style->Colors[ImGuiCol_BorderShadow]         = ImVec4(0.65f, 0.65f, 0.65f, 0.31f);
    style->Colors[ImGuiCol_FrameBg]              = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
    style->Colors[ImGuiCol_FrameBgHovered]       = ImVec4(0.98f, 0.98f, 0.98f, 0.88f);
    style->Colors[ImGuiCol_FrameBgActive]        = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style->Colors[ImGuiCol_TitleBg]              = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed]     = ImVec4(0.94f, 0.94f, 0.94f, 0.20f);
    style->Colors[ImGuiCol_TitleBgActive]        = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style->Colors[ImGuiCol_MenuBarBg]            = ImVec4(0.95f, 0.95f, 0.95f, 0.92f);
    style->Colors[ImGuiCol_ScrollbarBg]          = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrab]        = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.67f, 0.67f, 0.67f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
    style->Colors[ImGuiCol_CheckMark]            = ImVec4(0.72f, 0.72f, 0.72f, 1.00f);
    style->Colors[ImGuiCol_SliderGrab]           = ImVec4(0.57f, 0.57f, 0.57f, 0.34f);
    style->Colors[ImGuiCol_SliderGrabActive]     = ImVec4(0.24f, 0.24f, 0.24f, 0.34f);
    style->Colors[ImGuiCol_Button]               = ImVec4(0.97f, 0.97f, 0.97f, 1.00f);
    style->Colors[ImGuiCol_ButtonHovered]        = ImVec4(0.93f, 0.93f, 0.93f, 1.00f);
    style->Colors[ImGuiCol_ButtonActive]         = ImVec4(0.84f, 0.84f, 0.84f, 1.00f);
    style->Colors[ImGuiCol_Header]               = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style->Colors[ImGuiCol_HeaderHovered]        = ImVec4(0.92f, 0.92f, 0.92f, 1.00f);
    style->Colors[ImGuiCol_HeaderActive]         = ImVec4(0.84f, 0.84f, 0.84f, 1.00f);
    style->Colors[ImGuiCol_Separator]            = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    style->Colors[ImGuiCol_SeparatorHovered]     = ImVec4(0.70f, 0.60f, 0.60f, 1.00f);
    style->Colors[ImGuiCol_SeparatorActive]      = ImVec4(0.90f, 0.70f, 0.70f, 1.00f);
    style->Colors[ImGuiCol_ResizeGrip]           = ImVec4(0.00f, 0.00f, 0.00f, 0.30f);
    style->Colors[ImGuiCol_ResizeGripHovered]    = ImVec4(0.00f, 0.00f, 0.00f, 0.37f);
    style->Colors[ImGuiCol_ResizeGripActive]     = ImVec4(0.00f, 0.00f, 0.00f, 0.47f);
    /*style.Colors[ImGuiCol_CloseButton]           = ImVec4(0.00f, 0.00f, 0.00f, 0.18f);
    style.Colors[ImGuiCol_CloseButtonHovered]    = ImVec4(0.10f, 0.10f, 0.10f, 0.12f);
    style.Colors[ImGuiCol_CloseButtonActive]     = ImVec4(0.30f, 0.30f, 0.30f, 0.08f);*/
    style->Colors[ImGuiCol_PlotLines]            = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    style->Colors[ImGuiCol_PlotLinesHovered]     = ImVec4(0.69f, 0.56f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogram]        = ImVec4(0.64f, 0.50f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.37f, 0.22f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_TextSelectedBg]       = ImVec4(0.46f, 0.61f, 1.00f, 0.61f);
    style->Colors[ImGuiCol_ModalWindowDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

    CreateDefaultTabColorsFor(*style, ImGuiCol_Header, 0.f, ImVec2(0.0f, 1.f), ImVec2(0.0f, 0.6f));
}

void EditorStyleUtility::SetMayaThemecolors()
{
    ImGuiStyle* style                            = &ImGui::GetStyle();
    style->Colors[ImGuiCol_Text]                 = ImVec4(0.73f, 0.73f, 0.73f, 1.00f);
    style->Colors[ImGuiCol_TextDisabled]         = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    style->Colors[ImGuiCol_WindowBg]             = ImVec4(0.26f, 0.26f, 0.26f, 0.95f);
    style->Colors[ImGuiCol_ChildBg]              = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
    style->Colors[ImGuiCol_PopupBg]              = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
    style->Colors[ImGuiCol_Border]               = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
    style->Colors[ImGuiCol_BorderShadow]         = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
    style->Colors[ImGuiCol_FrameBg]              = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
    style->Colors[ImGuiCol_FrameBgHovered]       = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
    style->Colors[ImGuiCol_FrameBgActive]        = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
    style->Colors[ImGuiCol_TitleBg]              = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed]     = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
    style->Colors[ImGuiCol_TitleBgActive]        = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
    style->Colors[ImGuiCol_MenuBarBg]            = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarBg]          = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrab]        = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
    style->Colors[ImGuiCol_CheckMark]            = ImVec4(0.78f, 0.78f, 0.78f, 1.00f);
    style->Colors[ImGuiCol_SliderGrab]           = ImVec4(0.74f, 0.74f, 0.74f, 1.00f);
    style->Colors[ImGuiCol_SliderGrabActive]     = ImVec4(0.74f, 0.74f, 0.74f, 1.00f);
    style->Colors[ImGuiCol_Button]               = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
    style->Colors[ImGuiCol_ButtonHovered]        = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
    style->Colors[ImGuiCol_ButtonActive]         = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
    style->Colors[ImGuiCol_Header]               = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
    style->Colors[ImGuiCol_HeaderHovered]        = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
    style->Colors[ImGuiCol_HeaderActive]         = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
    style->Colors[ImGuiCol_Separator]            = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
    style->Colors[ImGuiCol_SeparatorHovered]     = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style->Colors[ImGuiCol_SeparatorActive]      = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style->Colors[ImGuiCol_ResizeGrip]           = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
    style->Colors[ImGuiCol_ResizeGripHovered]    = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style->Colors[ImGuiCol_ResizeGripActive]     = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style->Colors[ImGuiCol_PlotLines]            = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
    style->Colors[ImGuiCol_PlotLinesHovered]     = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogram]        = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_TextSelectedBg]       = ImVec4(0.32f, 0.52f, 0.65f, 1.00f);
    style->Colors[ImGuiCol_ModalWindowDimBg]     = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);
    CreateDefaultTabColorsFor(*style, ImGuiCol_ResizeGrip, 0.7f, ImVec2(0.25f, 0.8f), ImVec2(0.1f, 0.6f));
}

} // namespace sputnik::editor