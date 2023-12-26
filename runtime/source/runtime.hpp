#pragma once

// #include <main/entry_point.h>
#include <main/application.h>
#include <core/time_step.h>
#include <graphics/glcore/gl_shader.h>
#include <graphics/api/model.h>
#include <imgui.h>

namespace sputnik::runtime
{
using namespace sputnik::graphics::gl;
using namespace sputnik::graphics::api;

class Runtime : public sputnik::main::Application
{

public:
    Runtime(const std::string& name);

    // void Run() override;

    ~Runtime();

    virtual void createShaderProgram();

    virtual void DrawModel(sputnik::graphics::api::Model* model) override;
    virtual void DrawModel() override;
    virtual void LoadModel(const std::string& path) override;

private:
    std::shared_ptr<OglShaderProgram> m_static_program;
    std::shared_ptr<Model>            m_animated_model;
    std::shared_ptr<OglTexture2D>     m_diff_texture;
    std::shared_ptr<OglShaderProgram> m_shader_program;
};

} // namespace sputnik::runtime

// sputnik::main::Application* sputnik::main::CreateApplication()
//{
//     return new sputnik::runtime::Runtime("Sputnik Engine Runtime");
// }

#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#else
#pragma comment(linker, "/SUBSYSTEM:console")
#endif

typedef void (*InitializeFunction)(sputnik::main::Application* const);
InitializeFunction initialize = NULL;

typedef void (*ShutdownFunction)();
ShutdownFunction shutdown = NULL;

typedef void (*UpdateFunction)(float);
UpdateFunction update = NULL;

int main(int argc, char** argv)
{
    std::cout << "Runtime initializing...\n";

    HINSTANCE dll = NULL;
    // Load the DLL
    char path[MAX_PATH];
    std::memset(path, 0, MAX_PATH);
    DWORD result = GetModuleFileNameA(NULL, path, MAX_PATH);
    if(result == 0)
    {
        std::cout << "Failed to get module file name\n";
        return 1;
    }
    std::string dll_path = std::string(path);
    // dll_path             = dll_path.substr(0, dll_path.find_last_of("\\/")) + "\\editor.dll";
    dll_path = dll_path.substr(0, dll_path.find_last_of("\\/")) + "\\graphics-sandbox.dll";
    std::cout << "Loading DLL: " << dll_path << "\n";

    dll = LoadLibraryA("graphics-sandbox.dll");
    if(dll == NULL)
    {
        std::cout << "Failed to load DLL\n";
        return 1;
    }
    std::cout << "DLL loaded\n";

    // sputnik::main::Application* app = new sputnik::runtime::Runtime("Sputnik Engine Runtime");
    // app->Run();
    // delete app;

    // Get the function pointers
    initialize = (InitializeFunction)GetProcAddress(dll, "initialize");
    shutdown   = (ShutdownFunction)GetProcAddress(dll, "shutdown");
    update     = (UpdateFunction)GetProcAddress(dll, "update");

    // Initialize the DLL plugin
    initialize(nullptr);
    update(0.0);

    return 0;
}
