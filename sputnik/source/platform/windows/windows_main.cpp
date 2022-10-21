#include "windows_main.h"
#include "main/application.h"

#include <glad/glad.h>
#undef APIENTRY
#include <iostream>

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

#ifdef SPUTNIK_DEBUG
int main(int argc, const char** argv)
{
    return WinMain(GetModuleHandle(NULL), NULL, GetCommandLineA(), SW_SHOWDEFAULT);
}
#else
#endif

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
    gp_application = sputnik::main::CreateApplication();
    WNDCLASSEX wndclass;
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
    RegisterClassEx(&wndclass);

    int screenWidth  = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int clientWidth  = 800;
    int clientHeight = 600;

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
    HDC  hdc  = GetDC(hwnd);

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
    int pixelFormat  = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, pixelFormat, &pfd);

    HGLRC tempRC = wglCreateContext(hdc);
    wglMakeCurrent(hdc, tempRC);
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

    HGLRC hglrc = wglCreateContextAttribsARB(hdc, 0, attribList);

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(tempRC);
    wglMakeCurrent(hdc, hglrc);

    if(!gladLoadGL())
    {
        std::cout << "Could not initialize GLAD\n";
    }
    else
    {
        std::cout << "OpenGL Version " << GLVersion.major << "." << GLVersion.minor << " loaded\n";
    }

    PFNWGLGETEXTENSIONSSTRINGEXTPROC _wglGetExtensionsStringEXT =
        (PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");
    bool swapControlSupported = strstr(_wglGetExtensionsStringEXT(), "WGL_EXT_swap_control") != 0;

    int vsynch = 0;
    if(swapControlSupported)
    {
        PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT =
            (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
        PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT =
            (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");

        if(wglSwapIntervalEXT(1))
        {
            std::cout << "Enabled vsynch\n";
            vsynch = wglGetSwapIntervalEXT();
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

    glGenVertexArrays(1, &g_vertex_array_object);
    glBindVertexArray(g_vertex_array_object);

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
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

            glClearColor(0.5f, 0.6f, 0.7f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            float aspect = (float)clientWidth / (float)clientHeight;
            gp_application->Render(aspect);
        }
        if(gp_application != 0)
        {
            SwapBuffers(hdc);
            if(vsynch != 0)
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

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    switch(iMsg)
    {
    case WM_CLOSE:
        if(gp_application != 0)
        {
            gp_application->Shutdown();
            gp_application = 0;
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