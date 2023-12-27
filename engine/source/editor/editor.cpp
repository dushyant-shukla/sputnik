#include "pch.h"

#include "editor.hpp"
#include "editor_style_utility.hpp"
#include "main/application.h"
#include "core/layers/layer_stack.h"
#include "graphics/window/window_specification.h"
#include "editor_viewport.hpp"
#include "core/systems/render_system.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui_internal.h>
#include <ImGuizmo.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <memory>
#include <format>

namespace sputnik::editor
{

using namespace sputnik::core::systems;

Editor::~Editor()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

Editor* Editor::getInstance()
{
    static Editor editor;
    return &editor;
}

void Editor::beginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();
    beginDockspace();
    if(m_is_viewport_active)
    {
        m_viewport->beginFrame();
    }
}

void Editor::endFrame()
{
    if(m_is_viewport_active)
    {
        m_viewport->endFrame();
    }

    endDockspace();

    GLFWwindow* window = RenderSystem::getInstance()->getWindow()->GetNativeWindow();
    ImGuiIO&    io     = ImGui::GetIO();
    sputnik::graphics::window::WindowSpecification& window_specification =
        *(sputnik::graphics::window::WindowSpecification*)glfwGetWindowUserPointer(window);
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

void Editor::beginViewportFrame()
{
    m_viewport->beginFrame();
}

void Editor::endViewportFrame()
{
    m_viewport->endFrame();
}

void Editor::updateViewport(const core::TimeStep& time_step)
{
    m_viewport->update(time_step);
}

void Editor::update(sputnik::core::TimeStep& time_step)
{
    GLFWmonitor*       monitor      = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode         = glfwGetVideoMode(monitor);
    const float        frame_budget = 1000.0f / mode->refreshRate;
    // std::cout << "width: " << mode->width << std::endl;
    // std::cout << "height: " << mode->height << std::endl;
    // std::cout << "Red: " << mode->redBits << std::endl;
    // std::cout << "Blue: " << mode->blueBits << std::endl;
    // std::cout << "Green: " << mode->greenBits << std::endl;
    if(ImGui::Begin("System Information"))
    {
        drawWidgetText("Vendor", m_system_information.vendor.c_str(), 90.0f);
        drawWidgetText("Renderer", m_system_information.renderer.c_str(), 90.0f);
        drawWidgetText("Version", m_system_information.gl_version.c_str(), 90.0f);
        drawWidgetText("Shading Language Version", m_system_information.shading_language_version.c_str(), 90.0f);
        drawWidgetText("Display Frequency", std::to_string(mode->refreshRate).c_str(), 90.0f, "%s");
        drawWidgetText("VSync", m_system_information.is_vsync_enabled ? "ON" : "OFF", 90.0f);
        drawWidgetText("Frame Budget [ms]", frame_budget, "%.2f");
        drawWidgetText("Frame Time [ms]", time_step.GetMilliSeconds(), "%.5f");
        drawWidgetText("Frame Rate [FPS]", 1000.0f / time_step.GetMilliSeconds(), "%.2f");

        ////////////////////////////////////////////////////////////////////////////////////////////////////

        // ImGui::Text("Vendor: %s", m_system_information.vendor.c_str());
        // ImGui::Text("Renderer: %s", m_system_information.renderer.c_str());
        // ImGui::Text("Version: %s", m_system_information.gl_version.c_str());
        // ImGui::Text("Shanding Language Version: %s", m_system_information.shading_language_version.c_str());

        // std::string display_frequency = "Display Frequency: %d";
        // ImGui::Text(display_frequency.c_str(), mode->refreshRate);

        //// std::string vsync = "VSync: %s";
        //// ImGui::Text(vsync.c_str(), system_information.is_vsync_on ? "ON" : "OFF");

        // std::string frame_budget_str = "Frame Budget: %10.2f %s";
        // ImGui::Text(frame_budget_str.c_str(), frame_budget, "ms");

        // std::string frame_time = "Frame Time: %10.5f %s";
        // ImGui::Text(frame_time.c_str(), time_step.GetMilliSeconds(), "ms");

        // std::string frame_rate_str = "Frame Rate: %10.2f %s";
        // ImGui::Text(frame_rate_str.c_str(), 1000.0f / time_step.GetMilliSeconds(), "fps");
    }
    ImGui::End();

    // m_viewport->update(time_step);
    if(m_is_viewport_active)
    {
        m_viewport->update(time_step);
    }
}

void Editor::lateUpdate(sputnik::core::TimeStep& time_step)
{
    auto input_manager = sputnik::engine::api::InputManager::GetInstance();
    if(input_manager->IsKeyTriggered(KEY_E))
    {
        m_is_viewport_active  = !m_is_viewport_active;
        m_is_dockspace_active = !m_is_dockspace_active;
    }
}

void Editor::drawWidgetFloat(const std::string& label, float& value, const float& widget_width)
{
    ImGui::PushID(label.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, widget_width);

    ImGui::AlignTextToFramePadding();
    ImGui::Text(label.c_str());

    ImGui::NextColumn();
    // bool value_changed = ImGui::DragFloat("##", &value);
    ImGui::DragFloat("##", &value);

    ImGui::Columns(1);
    ImGui::PopID();

    // return value_changed;
}

void Editor::drawWidgetVec2(const std::string& label, vec2& value, const float& widget_width, float default_value) {}

void Editor::drawWidgetVec3(const std::string& label, vec3& value, const float& widget_width, float default_value)
{
    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

    ImGuiIO& io       = ImGui::GetIO();
    auto     boldFont = io.Fonts->Fonts[0]; // 0 - bold font

    ImGui::PushID(label.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, widget_width);

    ImGui::AlignTextToFramePadding();
    ImGui::Text(label.c_str());

    ImGui::NextColumn();

    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - (widget_width - 10.0f));
    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    // ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
    ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.5f);
        ImGui::PushFont(boldFont);
        if(ImGui::Button("x", buttonSize))
        {
            value.x = default_value;
        }
        ImGui::PopFont();
        ImGui::SameLine();
        ImGui::DragFloat("##x", &value.x, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();
        ImGui::PopStyleColor(3);
    }

    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
        ImGui::PushFont(boldFont);
        if(ImGui::Button("y", buttonSize))
        {
            value.y = default_value;
        }
        ImGui::PopFont();
        ImGui::SameLine();
        ImGui::DragFloat("##y", &value.y, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();
        ImGui::PopStyleColor(3);
    }

    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.9f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
        ImGui::PushFont(boldFont);
        if(ImGui::Button("z", buttonSize))
        {
            value.z = default_value;
        }
        ImGui::PopFont();
        ImGui::SameLine();
        ImGui::DragFloat("##z", &value.z, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::PopStyleColor(3);
    }
    ImGui::PopStyleVar(1);

    ImGui::Columns(1);
    ImGui::PopID();
}

void Editor::drawWidgetVec4(const std::string& label, vec4& value, const float& widget_width, float default_value)
{
    float line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

    ImGui::PushID(label.c_str());

    ImGui::Columns(2, "", true);
    ImGui::SetColumnWidth(0, widget_width);
    ImGui::AlignTextToFramePadding();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.5f);
    ImGui::Text(label.c_str());

    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(4, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    ImVec2 buttonSize = {line_height + 3.0f, line_height};
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.5f);
        if(ImGui::Button("X", buttonSize))
        {
            value.x = default_value;
        }
        ImGui::SameLine();
        ImGui::DragFloat("##x", &value.x, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();
        ImGui::PopStyleColor(3);
    }

    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
        if(ImGui::Button("Y", buttonSize))
        {
            value.y = default_value;
        }
        ImGui::SameLine();
        ImGui::DragFloat("##y", &value.y, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();
        ImGui::PopStyleColor(3);
    }

    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.9f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
        if(ImGui::Button("Z", buttonSize))
        {
            value.z = default_value;
        }
        ImGui::SameLine();
        ImGui::DragFloat("##z", &value.z, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();
        ImGui::PopStyleColor(3);
    }

    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.9f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
        if(ImGui::Button("w", buttonSize))
        {
            value.z = default_value;
        }
        ImGui::SameLine();
        ImGui::DragFloat("##w", &value.w, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::PopStyleColor(3);
    }

    ImGui::PopStyleVar();
    ImGui::Columns(1);
    ImGui::PopID();
}

void Editor::drawWidgetColor3(const std::string& label, vec3& value, const float& widget_width, float default_value)
{
    ImGui::PushID(label.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, widget_width);
    ImGui::AlignTextToFramePadding();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.5f);
    ImGui::Text(label.c_str());

    ImGui::NextColumn();

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.5f);
    if(ImGui::ColorEdit3("##Color", &value.x))
    {
    }

    ImGui::Columns(1);

    ImGui::PopID();
}

void Editor::drawWidgetColor4(const std::string& label, vec4& value, const float& widget_width, float default_value)
{
    ImGui::PushID(label.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, widget_width);
    ImGui::AlignTextToFramePadding();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.5f);
    ImGui::Text(label.c_str());

    ImGui::NextColumn();

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.5f);
    if(ImGui::ColorEdit4("##Color", &value.x))
    {
    }

    ImGui::Columns(1);

    ImGui::PopID();
}

void Editor::drawWidgetText(const std::string& label,
                            cstring            value,
                            const float&       widget_width,
                            cstring            format,
                            cstring            id)
{
    ImGui::PushID(id);

    ImGui::Columns(2);
    // ImGui::SetColumnWidth(0, widget_width);
    ImGui::AlignTextToFramePadding();
    ImGui::Text(label.c_str());

    ImGui::NextColumn();

    // ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.5f);
    if(format == nullptr)
    {
        ImGui::Text(value);
    }
    else
    {
        ImGui::Text(format, value);
    }

    ImGui::Columns(1);

    ImGui::PopID();

    // ImGui::Columns(2);
    // ImGui::AlignTextToFramePadding();
    // ImGui::Text("Display Frequency");
    // ImGui::NextColumn();
    // ImGui::Text("%d", mode->refreshRate);
    // ImGui::Columns(1);
}

Editor::Editor()
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

    // io.Fonts->AddFontFromFileTTF("../../data/fonts/OpenSans/OpenSans-Bold.ttf", 17.0f);
    // io.FontDefault = io.Fonts->AddFontFromFileTTF("../../data/fonts/OpenSans/OpenSans-Regular.ttf", 17.0f);

    io.Fonts->AddFontFromFileTTF("../../data/fonts/RobotoMono/RobotoMono-Bold.ttf", 17.0f);
    io.FontDefault = io.Fonts->AddFontFromFileTTF("../../data/fonts/RobotoMono/RobotoMono-Regular.ttf", 16.0f);

    // required my modification
    // io.Fonts->AddFontFromFileTTF("../../data/fonts/Ruda/Ruda-Bold.ttf", 17.0f);
    // io.FontDefault = io.Fonts->AddFontFromFileTTF("../../data/fonts/Ruda/Ruda-Regular.ttf", 17.0f);

    // Setup Dear ImGui style
    // ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();
    // setMayaThemecolors();
    EditorStyleUtility::sActiveTheme = EditorStyleUtility::Theme::CHERRY;
    EditorStyleUtility::sThemeFunctions[EditorStyleUtility::sActiveTheme]();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular
    // ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding              = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // SetDarkThemeColors();
    GLFWwindow* window = RenderSystem::getInstance()->getWindow()->GetNativeWindow();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    m_system_information.vendor     = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    m_system_information.renderer   = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    m_system_information.gl_version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    m_system_information.shading_language_version =
        reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));

    sputnik::graphics::window::WindowSpecification& window_specification =
        *(sputnik::graphics::window::WindowSpecification*)glfwGetWindowUserPointer(window);
    // io.DisplaySize              = ImVec2((float)window_specification.m_width, (float)window_specification.m_height);
    // const auto& [width, height] = sputnik::graphics::api::Renderer::GetViewportDimensions();
    m_viewport = std::make_unique<EditorViewport>(window_specification.m_width, window_specification.m_height);
}

void Editor::beginDockspace()
{
    if(!m_is_dockspace_active)
    {
        return;
    }

    // Note: Switch this to true to enable dockspace
    static bool               dockspaceOpen             = true;
    static bool               opt_fullscreen_persistant = true;
    bool                      opt_fullscreen            = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags           = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if(opt_fullscreen)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                        ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the
    // pass-thru hole, so we ask Begin() to not render a background.
    if(dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    if(ImGui::Begin("Editor Dockspace", &dockspaceOpen, window_flags))
        //{
        ImGui::PopStyleVar();

    if(opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO&    io          = ImGui::GetIO();
    ImGuiStyle& style       = ImGui::GetStyle();
    float       minWinSizeX = style.WindowMinSize.x;
    style.WindowMinSize.x   = 370.0f;
    if(io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    style.WindowMinSize.x = minWinSizeX;

    renderMenuBar();
}

void Editor::endDockspace()
{
    if(!m_is_dockspace_active)
    {
        return;
    }
    ImGui::End();
}

void Editor::renderMenuBar()
{
    if(ImGui::BeginMenuBar())
    {
        if(ImGui::BeginMenu("File"))
        {
            if(ImGui::MenuItem("New", "Ctrl+N"))
            {
                // NewSceneFile();
            }
            if(ImGui::MenuItem("Open...", "Ctrl+O"))
            {
                // OpenSceneFile();
            }
            if(ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
            {
                // SaveSceneFile();
            }
            if(ImGui::MenuItem("Close", "Alt+F4"))
            {
                // Application::Get().Close();
            }
            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("Options"))
        {
            if(ImGui::BeginMenu("Themes"))
            {
                for(auto& x : EditorStyleUtility::sThemes)
                {
                    EditorStyleUtility::Theme theme     = x.first;
                    std::string               themeName = x.second;
                    if(ImGui::MenuItem(themeName.c_str(),
                                       NULL,
                                       EditorStyleUtility::sActiveTheme == theme,
                                       EditorStyleUtility::sActiveTheme != theme))
                    {
                        EditorStyleUtility::sActiveTheme = theme;
                        EditorStyleUtility::sThemeFunctions[theme]();
                    }
                }

                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    // if(ImGui::BeginMenuBar())
    //{
    //     if(ImGui::BeginMenu("Options"))
    //     {
    //         if(ImGui::BeginMenu("Themes"))
    //         {
    //             for(auto& x : EditorStyleUtility::sThemes)
    //             {
    //                 EditorStyleUtility::Theme theme     = x.first;
    //                 std::string               themeName = x.second;
    //                 if(ImGui::MenuItem(themeName.c_str(),
    //                                    NULL,
    //                                    EditorStyleUtility::sActiveTheme == theme,
    //                                    EditorStyleUtility::sActiveTheme != theme))
    //                 {
    //                     EditorStyleUtility::sActiveTheme = theme;
    //                     EditorStyleUtility::sThemeFunctions[theme]();
    //                 }
    //             }

    //            ImGui::EndMenu();
    //        }
    //        ImGui::EndMenu();
    //    }
    //    ImGui::EndMenuBar();
    //}
}

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

// void Editor::setMayaThemecolors()
//{
//     ImGuiStyle* style                            = &ImGui::GetStyle();
//     style->Colors[ImGuiCol_Text]                 = ImVec4(0.73f, 0.73f, 0.73f, 1.00f);
//     style->Colors[ImGuiCol_TextDisabled]         = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
//     style->Colors[ImGuiCol_WindowBg]             = ImVec4(0.26f, 0.26f, 0.26f, 0.95f);
//     style->Colors[ImGuiCol_ChildBg]              = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
//     style->Colors[ImGuiCol_PopupBg]              = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
//     style->Colors[ImGuiCol_Border]               = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
//     style->Colors[ImGuiCol_BorderShadow]         = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
//     style->Colors[ImGuiCol_FrameBg]              = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
//     style->Colors[ImGuiCol_FrameBgHovered]       = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
//     style->Colors[ImGuiCol_FrameBgActive]        = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
//     style->Colors[ImGuiCol_TitleBg]              = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
//     style->Colors[ImGuiCol_TitleBgCollapsed]     = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
//     style->Colors[ImGuiCol_TitleBgActive]        = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
//     style->Colors[ImGuiCol_MenuBarBg]            = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
//     style->Colors[ImGuiCol_ScrollbarBg]          = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
//     style->Colors[ImGuiCol_ScrollbarGrab]        = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
//     style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
//     style->Colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
//     style->Colors[ImGuiCol_CheckMark]            = ImVec4(0.78f, 0.78f, 0.78f, 1.00f);
//     style->Colors[ImGuiCol_SliderGrab]           = ImVec4(0.74f, 0.74f, 0.74f, 1.00f);
//     style->Colors[ImGuiCol_SliderGrabActive]     = ImVec4(0.74f, 0.74f, 0.74f, 1.00f);
//     style->Colors[ImGuiCol_Button]               = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
//     style->Colors[ImGuiCol_ButtonHovered]        = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
//     style->Colors[ImGuiCol_ButtonActive]         = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
//     style->Colors[ImGuiCol_Header]               = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
//     style->Colors[ImGuiCol_HeaderHovered]        = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
//     style->Colors[ImGuiCol_HeaderActive]         = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
//     style->Colors[ImGuiCol_Separator]            = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
//     style->Colors[ImGuiCol_SeparatorHovered]     = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
//     style->Colors[ImGuiCol_SeparatorActive]      = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
//     style->Colors[ImGuiCol_ResizeGrip]           = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
//     style->Colors[ImGuiCol_ResizeGripHovered]    = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
//     style->Colors[ImGuiCol_ResizeGripActive]     = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
//     style->Colors[ImGuiCol_PlotLines]            = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
//     style->Colors[ImGuiCol_PlotLinesHovered]     = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
//     style->Colors[ImGuiCol_PlotHistogram]        = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
//     style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
//     style->Colors[ImGuiCol_TextSelectedBg]       = ImVec4(0.32f, 0.52f, 0.65f, 1.00f);
//     style->Colors[ImGuiCol_ModalWindowDimBg]     = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);
//     CreateDefaultTabColorsFor(*style, ImGuiCol_ResizeGrip, 0.7f, ImVec2(0.25f, 0.8f), ImVec2(0.1f, 0.6f));
// }

} // namespace sputnik::editor
