#include "pch.h"
#include "gl_ui_layer.h"
#include "graphics/window/window_specification.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace sputnik::graphics::glcore
{

GlUiLayer::GlUiLayer(GLFWwindow* window) : core::Layer("UiLayer"), m_window(window) {}

GlUiLayer::~GlUiLayer() {}

ImVec4 ImColorLerp(const ImVec4& a, const ImVec4& b, float t)
{
    return ImVec4(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t);
}

void CreateDefaultTabColorsFor(ImGuiStyle&   style,
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

void SetMayaThemecolors()
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

void GlUiLayer::OnAttach()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
    // io.ConfigViewportsNoAutoMerge = true;
    // io.ConfigViewportsNoTaskBarIcon = true;

    // io.Fonts->AddFontFromFileTTF("assets/fonts/OpenSans/OpenSans-Bold.ttf", 18.0f);
    // io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/OpenSans/OpenSans-Regular.ttf", 18.0f);

    // io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/RobotoMono/RobotoMono-Regular.ttf", 15.0f);

    // required my modification
    // io.Fonts->AddFontFromFileTTF("assets/fonts/Ruda/Ruda-Bold.ttf", 15.0f);
    // io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/Ruda/Ruda-Regular.ttf", 15.0f);

    // Setup Dear ImGui style
    // ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();
    SetMayaThemecolors();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular
    // ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding              = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // SetDarkThemeColors();

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    m_system_information.vendor     = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    m_system_information.renderer   = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    m_system_information.gl_version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    m_system_information.shading_language_version =
        reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void GlUiLayer::OnDetach()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GlUiLayer::OnUpdate(const core::TimeStep& time_step) {}

void GlUiLayer::OnEvent() {}

void GlUiLayer::OnUpdateUI(const core::TimeStep& time_step)
{
    GLFWmonitor*       monitor      = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode         = glfwGetVideoMode(monitor);
    const float        frame_budget = 1000.0f / mode->refreshRate;
    // std::cout << "width: " << mode->width << std::endl;
    // std::cout << "height: " << mode->height << std::endl;
    // std::cout << "red: " << mode->redBits << std::endl;
    // std::cout << "Blue: " << mode->blueBits << std::endl;
    // std::cout << "green: " << mode->greenBits << std::endl;
    if(ImGui::Begin("System Information"))
    {
        ImGui::Text("Vendor: %s", m_system_information.vendor.c_str());
        ImGui::Text("Renderer: %s", m_system_information.renderer.c_str());
        ImGui::Text("Version: %s", m_system_information.gl_version.c_str());
        ImGui::Text("Shanding Language Version: %s", m_system_information.shading_language_version.c_str());

        std::string display_frequency = "Display Frequency: %d";
        ImGui::Text(display_frequency.c_str(), mode->refreshRate);

        // std::string vsync = "VSync: %s";
        // ImGui::Text(vsync.c_str(), system_information.is_vsync_on ? "ON" : "OFF");

        std::string frame_budget_str = "Frame Budget: %10.2f %s";
        ImGui::Text(frame_budget_str.c_str(), frame_budget, "ms");
    }
    ImGui::End();
}

void GlUiLayer::Begin()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // ImGui::ShowDemoWindow();
}

void GlUiLayer::End()
{
    ImGuiIO&                     io = ImGui::GetIO();
    window::WindowSpecification& window_specification =
        *(window::WindowSpecification*)glfwGetWindowUserPointer(m_window);
    io.DisplaySize = ImVec2((float)window_specification.m_width, (float)window_specification.m_height);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

} // namespace sputnik::graphics::glcore