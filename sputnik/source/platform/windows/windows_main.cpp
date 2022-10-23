#include "windows_main.h"
#include "main/application.h"

#include <glad/glad.h>
#undef APIENTRY
#include <iostream>

#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_opengl3.h>

#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_FLAGS_ARB 0x2094
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126

// A modern OpenGL context is created through the function wglCreateContextAttribsARB.
// This typedef defines a function pointer type for wglCreatecontextAttribsARB.
typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC, HGLRC, const int*);

// Function pointers for enabling vsynch
typedef const char*(WINAPI* PFNWGLGETEXTENSIONSSTRINGEXTPROC)(void);
typedef BOOL(WINAPI* PFNWGLSWAPINTERVALEXTPROC)(int);
typedef int(WINAPI* PFNWGLGETSWAPINTERVALEXTPROC)(void);

extern sputnik::main::Application* sputnik::main::CreateApplication();

sputnik::main::Application* gp_application        = 0;
GLuint                      g_vertex_array_object = 0;
float                       gScaleFactor          = 1.0f;
float                       gInvScaleFactor       = 1.0f;
int                         g_display_w           = 800;
int                         g_display_h           = 600;
int                         g_vsync               = 0;

void Fatal(const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    vprintf(msg, args);
    va_end(args);
    ExitProcess(1);
}

bool IsWGLExtensionSupported(const char* extension_name)
{
    PFNWGLGETEXTENSIONSSTRINGEXTPROC _wglGetExtensionsStringEXT =
        (PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");
    if(strstr(_wglGetExtensionsStringEXT(), extension_name) != 0)
    {
        return true;
    }
    return false;
}

// 0 - No Interval, 1 - Sync whit VSYNC, n - n times Sync with VSYNC
void EnableVsync(int interval)
{
    if(IsWGLExtensionSupported("WGL_EXT_swap_control"))
    {
        PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT =
            (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
        PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT =
            (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");

        if(wglSwapIntervalEXT(1))
        {
            std::cout << "Enabled vsynch\n";
            g_vsync = wglGetSwapIntervalEXT();
        }
        else
        {
            std::cout << "Could not enable vsynch\n";
        }
    }
    else
    { // !swapControlSupported
        std::cout << "WGL_EXT_swap_control not supported\n";
    }
}

void InitializeOpenGLForWindows(HDC device_context)
{
    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize        = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion     = 1;
    pfd.dwFlags      = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
    pfd.iPixelType   = PFD_TYPE_RGBA;
    pfd.cColorBits   = 24;
    pfd.cDepthBits   = 32;
    pfd.cStencilBits = 8;
    pfd.iLayerType   = PFD_MAIN_PLANE;
    int pixelFormat  = ChoosePixelFormat(device_context, &pfd);
    SetPixelFormat(device_context, pixelFormat, &pfd);

    HGLRC legacy_context = wglCreateContext(device_context);
    wglMakeCurrent(device_context, legacy_context);
    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
    wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

    const int attribList[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB,
        4,
        WGL_CONTEXT_MINOR_VERSION_ARB,
        6,
        WGL_CONTEXT_FLAGS_ARB,
        0,
        WGL_CONTEXT_PROFILE_MASK_ARB,
        WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0,
    };

    HGLRC gl_rendering_context = wglCreateContextAttribsARB(device_context, 0, attribList);

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(legacy_context);
    wglMakeCurrent(device_context, gl_rendering_context);

    if(!gladLoadGL())
    {
        std::cout << "Could not initialize GLAD\n";
    }
    else
    {
        std::cout << "OpenGL Version " << GLVersion.major << "." << GLVersion.minor << " loaded\n";
    }
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

void InitializeImGui(HWND hwnd)
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

    // io.Fonts->AddFontFromFileTTF("assets/fonts/Ruda/Ruda-Bold.ttf", 15.0f);
    // io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/Ruda/Ruda-Regular.ttf", 15.0f);

    // Setup Dear ImGui style
    //ImGui::StyleColorsDark();
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
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void ShutdownImGui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void BeginImGuiFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void EndImGuiFrame(HWND hwnd)
{
    ImGuiIO& io    = ImGui::GetIO();
    io.DisplaySize = ImVec2(static_cast<float>(g_display_w), static_cast<float>(g_display_h));

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        HDC   device_context    = GetDC(hwnd);
        HGLRC gl_render_context = wglGetCurrentContext();
        // GLFWwindow* backup_current_context = glfwGetCurrentContext();
        //wglMakeCurrent(device_context, gl_render_context);
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        wglMakeCurrent(device_context, gl_render_context);
        // glfwMakeContextCurrent(backup_current_context);
    }
}

// Todo:: Not sure, viewports and docking is not working.
void EnableDockSpace()
{
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
    if(ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags))
    {
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
    }
    ImGui::End(); // DockSpace Demo
}

#ifdef SPUTNIK_DEBUG
int main(int argc, const char** argv)
{
    return WinMain(GetModuleHandle(NULL), NULL, GetCommandLineA(), SW_SHOWDEFAULT);
}
#else
#endif

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
    gp_application         = sputnik::main::CreateApplication();
    WNDCLASSEX wndclass    = {};
    wndclass.cbSize        = sizeof(WNDCLASSEX);
    wndclass.style         = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc   = WndProc;
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = 0;
    wndclass.hInstance     = hInstance;
    wndclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wndclass.lpszMenuName  = 0;
    wndclass.lpszClassName = "Win32 Game Window";
    if(!RegisterClassEx(&wndclass))
    {
        Fatal("ERROR: Couldn't register window class!");
    }

    int screenWidth  = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int clientWidth  = g_display_w;
    int clientHeight = g_display_h;

    UINT DPI        = GetDpiForSystem();
    gScaleFactor    = (float)DPI / 96.0f;
    gInvScaleFactor = 1.0f / gScaleFactor;
    clientWidth     = (int)((float)clientWidth * gScaleFactor);
    clientHeight    = (int)((float)clientHeight * gScaleFactor);

    RECT windowRect;
    SetRect(&windowRect,
            (screenWidth / 2) - (clientWidth / 2),
            (screenHeight / 2) - (clientHeight / 2),
            (screenWidth / 2) + (clientWidth / 2),
            (screenHeight / 2) + (clientHeight / 2));

    DWORD style =
        (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX); // WS_THICKFRAME to resize
    AdjustWindowRectEx(&windowRect, style, FALSE, 0);
    HWND hwnd = CreateWindowEx(0,
                               wndclass.lpszClassName,
                               "Sputnik",
                               style,
                               windowRect.left,
                               windowRect.top,
                               windowRect.right - windowRect.left,
                               windowRect.bottom - windowRect.top,
                               NULL,
                               NULL,
                               hInstance,
                               lpCmdLine);
    if(!hwnd)
    {
        Fatal("ERROR: Couldn't create window!");
    }

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    HDC device_context = GetDC(hwnd);

    InitializeOpenGLForWindows(device_context);

    // This should ideally be handled by the renderer
    glGenVertexArrays(1, &g_vertex_array_object);
    glBindVertexArray(g_vertex_array_object);

    InitializeImGui(hwnd); // Initialize ImGui

    gp_application->Initialize();

    ULONGLONG lastTick = GetTickCount64();
    MSG       msg;
    while(true)
    {
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if(msg.message == WM_QUIT)
            {
                break;
            }
            BeginImGuiFrame();
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        ULONGLONG thisTick  = GetTickCount64();
        float     deltaTime = float(thisTick - lastTick) * 0.001f;
        lastTick            = thisTick;
        if(gp_application != 0)
        {
            gp_application->Update(deltaTime);
        }
        if(gp_application != 0)
        {
            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            clientWidth  = clientRect.right - clientRect.left;
            clientHeight = clientRect.bottom - clientRect.top;
            glViewport(0, 0, clientWidth, clientHeight);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);

            glPointSize(5.0f);
            glLineWidth(1.5f * gScaleFactor);

            glBindVertexArray(g_vertex_array_object);

            glClearColor(0.16f, 0.16f, 0.16f, 1.00f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            float aspect = (float)clientWidth / (float)clientHeight;
            gp_application->Render(aspect);
        }
        if(gp_application != 0)
        {
            EndImGuiFrame(hwnd);
            SwapBuffers(device_context);
            if(g_vsync != 0)
            {
                glFinish();
            }
        }
    } // End of game loop

    if(gp_application != 0)
    {
        std::cout << "Expected application to be null on exit\n";
        delete gp_application;
    }

    return (int)msg.wParam;
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    if(ImGui_ImplWin32_WndProcHandler(hwnd, iMsg, wParam, lParam))
    {
        return true;
    }

    switch(iMsg)
    {
    case WM_SIZE:
        if(wParam != SIZE_MINIMIZED)
        {
            g_display_w = (UINT)LOWORD(lParam);
            g_display_h = (UINT)HIWORD(lParam);
        }
        return 0;
    case WM_CLOSE:
        if(gp_application != 0)
        {
            gp_application->Shutdown();
            gp_application = 0;
            ShutdownImGui();
            DestroyWindow(hwnd);
        }
        else
        {
            std::cout << "Already shut down!\n";
        }
        break;
    case WM_DESTROY:
        if(g_vertex_array_object != 0)
        {
            HDC   hdc   = GetDC(hwnd);
            HGLRC hglrc = wglGetCurrentContext();

            glBindVertexArray(0);
            glDeleteVertexArrays(1, &g_vertex_array_object);
            g_vertex_array_object = 0;

            wglMakeCurrent(NULL, NULL);
            wglDeleteContext(hglrc);
            ReleaseDC(hwnd, hdc);

            PostQuitMessage(0);
        }
        else
        {
            std::cout << "Got multiple destroy messages\n";
        }
        break;
    case WM_PAINT:
    case WM_ERASEBKGND:
        return 0;
    }

    return DefWindowProc(hwnd, iMsg, wParam, lParam);
}